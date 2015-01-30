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

    PASS();
}

TEST test_CTMemoryControlMake_stack_alloced()
{
    struct test_CTDerivedStruct item;
    item.data = 10;
    item.pointer = (int *)malloc(1);

    int errno = CTMemoryControlMake(item.memoryControl, deallocCTDerivedStruct);
    ASSERT(errno == 0);

    PASS();
}

SUITE(test_basic) {
    RUN_TEST(test_CTMemoryControlMake_heap_alloced);
    RUN_TEST(test_CTMemoryControlMake_stack_alloced);
}
