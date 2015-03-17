#define	JEMALLOC_HUGE_C_
#include "jemalloc/internal/jemalloc_internal.h"

/******************************************************************************/

static extent_node_t *
huge_node_get(const void *ptr)
{
	extent_node_t *node;

	node = chunk_lookup(ptr);
	assert(node->size != 0);

	return (node);
}

static bool
huge_node_set(const void *ptr, extent_node_t *node)
{

	assert(node->addr == ptr);
	assert(node->size != 0);
	return (chunk_register(ptr, node));
}

static void
huge_node_unset(const void *ptr, const extent_node_t *node)
{

	chunk_deregister(ptr, node);
}

void *
huge_malloc(tsd_t *tsd, arena_t *arena, size_t size, bool zero,
    tcache_t *tcache)
{
	size_t usize;

	usize = s2u(size);
	if (usize == 0) {
		/* size_t overflow. */
		return (NULL);
	}

	return (huge_palloc(tsd, arena, usize, chunksize, zero, tcache));
}

void *
huge_palloc(tsd_t *tsd, arena_t *arena, size_t usize, size_t alignment,
    bool zero, tcache_t *tcache)
{
	void *ret;
	extent_node_t *node;
	bool is_zeroed;

	/* Allocate one or more contiguous chunks for this request. */

	/* Allocate an extent node with which to track the chunk. */
	node = ipallocztm(tsd, CACHELINE_CEILING(sizeof(extent_node_t)),
	    CACHELINE, false, tcache, true, arena);
	if (node == NULL)
		return (NULL);

	/*
	 * Copy zero into is_zeroed and pass the copy to chunk_alloc(), so that
	 * it is possible to make correct junk/zero fill decisions below.
	 */
	is_zeroed = zero;
	arena = arena_choose(tsd, arena);
	if (unlikely(arena == NULL) || (ret = arena_chunk_alloc_huge(arena,
	    usize, alignment, &is_zeroed)) == NULL) {
		idalloctm(tsd, node, tcache, true);
		return (NULL);
	}

	node->addr = ret;
	node->size = usize;
	node->zeroed = is_zeroed;
	node->arena = arena;

	if (huge_node_set(ret, node)) {
		arena_chunk_dalloc_huge(arena, ret, usize);
		idalloctm(tsd, node, tcache, true);
		return (NULL);
	}

	/* Insert node into huge. */
	malloc_mutex_lock(&arena->huge_mtx);
	ql_elm_new(node, ql_link);
	ql_tail_insert(&arena->huge, node, ql_link);
	malloc_mutex_unlock(&arena->huge_mtx);

	if (zero || (config_fill && unlikely(opt_zero))) {
		if (!is_zeroed)
			memset(ret, 0, usize);
	} else if (config_fill && unlikely(opt_junk_alloc))
		memset(ret, 0xa5, usize);

	return (ret);
}

#ifdef JEMALLOC_JET
#undef huge_dalloc_junk
#define	huge_dalloc_junk JEMALLOC_N(huge_dalloc_junk_impl)
#endif
static void
huge_dalloc_junk(void *ptr, size_t usize)
{

	if (config_fill && have_dss && unlikely(opt_junk_free)) {
		/*
		 * Only bother junk filling if the chunk isn't about to be
		 * unmapped.
		 */
		if (!config_munmap || (have_dss && chunk_in_dss(ptr)))
			memset(ptr, 0x5a, usize);
	}
}
#ifdef JEMALLOC_JET
#undef huge_dalloc_junk
#define	huge_dalloc_junk JEMALLOC_N(huge_dalloc_junk)
huge_dalloc_junk_t *huge_dalloc_junk = JEMALLOC_N(huge_dalloc_junk_impl);
#endif

static void
huge_ralloc_no_move_similar(void *ptr, size_t oldsize, size_t usize,
    size_t size, size_t extra, bool zero)
{
	size_t usize_next;
	bool zeroed;
	extent_node_t *node;
	arena_t *arena;

	/* Increase usize to incorporate extra. */
	while (usize < s2u(size+extra) && (usize_next = s2u(usize+1)) < oldsize)
		usize = usize_next;

	if (oldsize == usize)
		return;

	/* Fill if necessary (shrinking). */
	if (oldsize > usize) {
		size_t sdiff = CHUNK_CEILING(usize) - usize;
		zeroed = (sdiff != 0) ? !pages_purge((void *)((uintptr_t)ptr +
		    usize), sdiff) : true;
		if (config_fill && unlikely(opt_junk_free)) {
			memset((void *)((uintptr_t)ptr + usize), 0x5a, oldsize -
			    usize);
			zeroed = false;
		}
	} else
		zeroed = true;

	node = huge_node_get(ptr);
	arena = node->arena;
	malloc_mutex_lock(&arena->huge_mtx);
	/* Update the size of the huge allocation. */
	assert(node->size != usize);
	node->size = usize;
	/* Clear node->zeroed if zeroing failed above. */
	node->zeroed = (node->zeroed && zeroed);
	malloc_mutex_unlock(&arena->huge_mtx);

	arena_chunk_ralloc_huge_similar(arena, ptr, oldsize, usize);

	/* Fill if necessary (growing). */
	if (oldsize < usize) {
		if (zero || (config_fill && unlikely(opt_zero))) {
			if (!zeroed) {
				memset((void *)((uintptr_t)ptr + oldsize), 0,
				    usize - oldsize);
			}
		} else if (config_fill && unlikely(opt_junk_alloc)) {
			memset((void *)((uintptr_t)ptr + oldsize), 0xa5, usize -
			    oldsize);
		}
	}
}

static void
huge_ralloc_no_move_shrink(void *ptr, size_t oldsize, size_t usize)
{
	size_t sdiff;
	bool zeroed;
	extent_node_t *node;
	arena_t *arena;

	sdiff = CHUNK_CEILING(usize) - usize;
	zeroed = (sdiff != 0) ? !pages_purge((void *)((uintptr_t)ptr + usize),
	    sdiff) : true;
	if (config_fill && unlikely(opt_junk_free)) {
		huge_dalloc_junk((void *)((uintptr_t)ptr + usize), oldsize -
		    usize);
		zeroed = false;
	}

	node = huge_node_get(ptr);
	arena = node->arena;
	malloc_mutex_lock(&arena->huge_mtx);
	/* Update the size of the huge allocation. */
	node->size = usize;
	/* Clear node->zeroed if zeroing failed above. */
	node->zeroed = (node->zeroed && zeroed);
	malloc_mutex_unlock(&arena->huge_mtx);

	/* Zap the excess chunks. */
	arena_chunk_ralloc_huge_shrink(arena, ptr, oldsize, usize);
}

static bool
huge_ralloc_no_move_expand(void *ptr, size_t oldsize, size_t size, bool zero) {
	size_t usize;
	extent_node_t *node;
	arena_t *arena;
	bool is_zeroed_subchunk, is_zeroed_chunk;

	usize = s2u(size);
	if (usize == 0) {
		/* size_t overflow. */
		return (true);
	}

	node = huge_node_get(ptr);
	arena = node->arena;
	malloc_mutex_lock(&arena->huge_mtx);
	is_zeroed_subchunk = node->zeroed;
	malloc_mutex_unlock(&arena->huge_mtx);

	/*
	 * Copy zero into is_zeroed_chunk and pass the copy to chunk_alloc(), so
	 * that it is possible to make correct junk/zero fill decisions below.
	 */
	is_zeroed_chunk = zero;

	if (arena_chunk_ralloc_huge_expand(arena, ptr, oldsize, usize,
	     &is_zeroed_chunk))
		return (true);

	malloc_mutex_lock(&arena->huge_mtx);
	/* Update the size of the huge allocation. */
	node->size = usize;
	malloc_mutex_unlock(&arena->huge_mtx);

	if (zero || (config_fill && unlikely(opt_zero))) {
		if (!is_zeroed_subchunk) {
			memset((void *)((uintptr_t)ptr + oldsize), 0,
			    CHUNK_CEILING(oldsize) - oldsize);
		}
		if (!is_zeroed_chunk) {
			memset((void *)((uintptr_t)ptr +
			    CHUNK_CEILING(oldsize)), 0, usize -
			    CHUNK_CEILING(oldsize));
		}
	} else if (config_fill && unlikely(opt_junk_alloc)) {
		memset((void *)((uintptr_t)ptr + oldsize), 0xa5, usize -
		    oldsize);
	}

	return (false);
}

bool
huge_ralloc_no_move(void *ptr, size_t oldsize, size_t size, size_t extra,
    bool zero)
{
	size_t usize;

	/* Both allocations must be huge to avoid a move. */
	if (oldsize < chunksize)
		return (true);

	assert(s2u(oldsize) == oldsize);
	usize = s2u(size);
	if (usize == 0) {
		/* size_t overflow. */
		return (true);
	}

	/*
	 * Avoid moving the allocation if the existing chunk size accommodates
	 * the new size.
	 */
	if (CHUNK_CEILING(oldsize) >= CHUNK_CEILING(usize)
	    && CHUNK_CEILING(oldsize) <= CHUNK_CEILING(size+extra)) {
		huge_ralloc_no_move_similar(ptr, oldsize, usize, size, extra,
		    zero);
		return (false);
	}

	/* Shrink the allocation in-place. */
	if (CHUNK_CEILING(oldsize) >= CHUNK_CEILING(usize)) {
		huge_ralloc_no_move_shrink(ptr, oldsize, usize);
		return (false);
	}

	/* Attempt to expand the allocation in-place. */
	if (huge_ralloc_no_move_expand(ptr, oldsize, size + extra,
	    zero)) {
		if (extra == 0)
			return (true);

		/* Try again, this time without extra. */
		return (huge_ralloc_no_move_expand(ptr, oldsize, size, zero));
	}
	return (false);
}

void *
huge_ralloc(tsd_t *tsd, arena_t *arena, void *ptr, size_t oldsize, size_t size,
    size_t extra, size_t alignment, bool zero, tcache_t *tcache)
{
	void *ret;
	size_t copysize;

	/* Try to avoid moving the allocation. */
	if (!huge_ralloc_no_move(ptr, oldsize, size, extra, zero))
		return (ptr);

	/*
	 * size and oldsize are different enough that we need to use a
	 * different size class.  In that case, fall back to allocating new
	 * space and copying.
	 */
	if (alignment > chunksize) {
		ret = huge_palloc(tsd, arena, size + extra, alignment, zero,
		    tcache);
	} else
		ret = huge_malloc(tsd, arena, size + extra, zero, tcache);

	if (ret == NULL) {
		if (extra == 0)
			return (NULL);
		/* Try again, this time without extra. */
		if (alignment > chunksize) {
			ret = huge_palloc(tsd, arena, size, alignment, zero,
			    tcache);
		} else
			ret = huge_malloc(tsd, arena, size, zero, tcache);

		if (ret == NULL)
			return (NULL);
	}

	/*
	 * Copy at most size bytes (not size+extra), since the caller has no
	 * expectation that the extra bytes will be reliably preserved.
	 */
	copysize = (size < oldsize) ? size : oldsize;
	memcpy(ret, ptr, copysize);
	isqalloc(tsd, ptr, oldsize, tcache);
	return (ret);
}

void
huge_dalloc(tsd_t *tsd, void *ptr, tcache_t *tcache)
{
	extent_node_t *node;
	arena_t *arena;

	node = huge_node_get(ptr);
	arena = node->arena;
	huge_node_unset(ptr, node);
	malloc_mutex_lock(&arena->huge_mtx);
	ql_remove(&arena->huge, node, ql_link);
	malloc_mutex_unlock(&arena->huge_mtx);

	huge_dalloc_junk(node->addr, node->size);
	arena_chunk_dalloc_huge(node->arena, node->addr, node->size);
	idalloctm(tsd, node, tcache, true);
}

arena_t *
huge_aalloc(const void *ptr)
{

	return (huge_node_get(ptr)->arena);
}

size_t
huge_salloc(const void *ptr)
{
	size_t size;
	extent_node_t *node;
	arena_t *arena;

	node = huge_node_get(ptr);
	arena = node->arena;
	malloc_mutex_lock(&arena->huge_mtx);
	size = node->size;
	malloc_mutex_unlock(&arena->huge_mtx);

	return (size);
}

prof_tctx_t *
huge_prof_tctx_get(const void *ptr)
{
	prof_tctx_t *tctx;
	extent_node_t *node;
	arena_t *arena;

	node = huge_node_get(ptr);
	arena = node->arena;
	malloc_mutex_lock(&arena->huge_mtx);
	tctx = node->prof_tctx;
	malloc_mutex_unlock(&arena->huge_mtx);

	return (tctx);
}

void
huge_prof_tctx_set(const void *ptr, prof_tctx_t *tctx)
{
	extent_node_t *node;
	arena_t *arena;

	node = huge_node_get(ptr);
	arena = node->arena;
	malloc_mutex_lock(&arena->huge_mtx);
	node->prof_tctx = tctx;
	malloc_mutex_unlock(&arena->huge_mtx);
}