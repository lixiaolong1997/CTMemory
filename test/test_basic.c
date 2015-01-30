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
    CTMemoryControlDealloc(node->memoryControl);
}

/***********************************************************************/

TEST test_CTMemoryControlMake_heap_alloced()
{
    struct test_CTDerivedStruct *item = (struct test_CTDerivedStruct *)malloc(sizeof(struct test_CTDerivedStruct));
    item->data = 10;
    item->pointer = (int *)malloc(1);

    int errno = CTMemoryControlMake(item->memoryControl, deallocCTDerivedStruct);

    ASSERT(errno == 0);
    ASSERT(pthread_mutex_lock(&(item->memoryControl->memoryMutexLock)) == 0);
    ASSERT(pthread_mutex_unlock(&(item->memoryControl->memoryMutexLock)) == 0);

    PASS();
}

TEST test_CTMemoryControlMake_stack_alloced()
{
    struct test_CTDerivedStruct item;
    item.data = 10;
    item.pointer = (int *)malloc(1);

    int errno = CTMemoryControlMake(item.memoryControl, deallocCTDerivedStruct);

    ASSERT(errno == 0);
    /* ASSERT(pthread_mutex_lock(&(item.memoryControl->memoryMutexLock)) == 0); */
    /* ASSERT(pthread_mutex_unlock(&(item.memoryControl->memoryMutexLock)) == 0); */

    PASS();
}

TEST test_ctRetain()
{
    PASS();
}

TEST test_ctRetain_multi_thread()
{
    PASS();
}

TEST test_ctRelease()
{
    PASS();
}

TEST test_ctRelease_multi_thread()
{
    PASS();
}

TEST test_ctAlloc()
{
    PASS();
}

TEST test_ctAlloc_multi_thread()
{
    PASS();
}

SUITE(test_basic) {
    RUN_TEST(test_CTMemoryControlMake_heap_alloced);
    RUN_TEST(test_CTMemoryControlMake_stack_alloced);
    RUN_TEST(test_ctRetain);
    RUN_TEST(test_ctRetain_multi_thread);
    RUN_TEST(test_ctRelease);
    RUN_TEST(test_ctRelease_multi_thread);
    RUN_TEST(test_ctAlloc);
    RUN_TEST(test_ctAlloc_multi_thread);
}
