#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "greatest.h"
#include "CTMemory.h"

SUITE(test_basic);

void deallocCTDerivedStruct(void *item);

struct test_CTDerivedStruct
{
    struct CTMemoryControl *memoryControl;
    int data;
    int *pointer;
};

void deallocCTDerivedStruct(void *item){
    struct test_CTDerivedStruct *node = (struct test_CTDerivedStruct *)item;
    ctRelease(node->pointer);
    CTMemoryControlDealloc(node->memoryControl);
}

/********************************** CTMemoryControlMake *************************************/

TEST test_CTMemoryControlMake_heap_alloced()
{
    struct test_CTDerivedStruct *item = (struct test_CTDerivedStruct *)malloc(sizeof(struct test_CTDerivedStruct));
    item->data = 10;
    item->pointer = (int *)malloc(1);

    int errno = CTMemoryControlMake(&item->memoryControl, deallocCTDerivedStruct);

    ASSERT(errno == 0);
    ASSERT(item->memoryControl != NULL);
    ASSERT(item->memoryControl->memoryMutexLock != NULL);
    ASSERT(pthread_mutex_lock(item->memoryControl->memoryMutexLock) == 0);
    ASSERT(pthread_mutex_unlock(item->memoryControl->memoryMutexLock) == 0);

    PASS();
}

void * test_CTMemoryControlMake_heap_alloced_multi_thread_function(void *args)
{
    struct test_CTDerivedStruct *item = (struct test_CTDerivedStruct *)malloc(sizeof(struct test_CTDerivedStruct));
    item->data = 10;
    item->pointer = (int *)malloc(1);

    int errno = CTMemoryControlMake(&item->memoryControl, deallocCTDerivedStruct);
    pthread_exit(NULL);
    return NULL;
}

TEST test_CTMemoryControlMake_heap_alloced_multi_thread()
{
    PASS();
}

TEST test_CTMemoryControlMake_stack_alloced()
{
    struct test_CTDerivedStruct item;
    item.data = 10;
    item.pointer = (int *)malloc(1);

    int errno = CTMemoryControlMake(&item.memoryControl, deallocCTDerivedStruct);

    ASSERT(errno == 0);
    ASSERT(item.memoryControl != NULL);
    ASSERT(&(item.memoryControl->memoryMutexLock) != NULL);
    ASSERT(pthread_mutex_lock(item.memoryControl->memoryMutexLock) == 0);
    ASSERT(pthread_mutex_unlock(item.memoryControl->memoryMutexLock) == 0);

    PASS();
}

TEST test_CTMemoryControlMake_stack_alloced_multi_thread()
{
    PASS();
}

/********************************** ctRetain *************************************/

TEST test_ctRetain_stack()
{
    PASS();
}

TEST test_ctRetain_stack_multi_thread()
{
    PASS();
}

TEST test_ctRetain_heap()
{
    PASS();
}

TEST test_ctRetain_heap_multi_thread()
{
    PASS();
}

/********************************** ctRelease *************************************/

TEST test_ctRelease_stack()
{
    PASS();
}

TEST test_ctRelease_stack_muti_thread()
{
    PASS();
}

TEST test_ctRelease_heap()
{
    PASS();
}

TEST test_ctRelease_heap_multi_thread()
{
    PASS();
}

/********************************** ctAlloc *************************************/

TEST test_ctAlloc()
{
    PASS();
}

TEST test_ctAlloc_multi_thread()
{
    PASS();
}

/********************************** TEST CASE ENDS *************************************/

SUITE(test_basic) {

    RUN_TEST(test_CTMemoryControlMake_heap_alloced);
    RUN_TEST(test_CTMemoryControlMake_heap_alloced_multi_thread);
    RUN_TEST(test_CTMemoryControlMake_stack_alloced);
    RUN_TEST(test_CTMemoryControlMake_stack_alloced_multi_thread);

    RUN_TEST(test_ctRetain_stack);
    RUN_TEST(test_ctRetain_stack_multi_thread);
    RUN_TEST(test_ctRetain_heap);
    RUN_TEST(test_ctRetain_heap_multi_thread);

    RUN_TEST(test_ctRelease_heap);
    RUN_TEST(test_ctRelease_heap_multi_thread);
    RUN_TEST(test_ctRelease_stack);
    RUN_TEST(test_ctRelease_stack_muti_thread);

    RUN_TEST(test_ctAlloc);
    RUN_TEST(test_ctAlloc_multi_thread);
}
