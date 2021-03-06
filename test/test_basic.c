#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "greatest.h"
#include "CTMemory.h"

extern struct CTMemoryRecord * _findRecord(void *recordedMemoryPointer);
extern struct CTMemoryRecordList * _sharedMemoryRecordList(void);
extern struct CTMemoryRecord * _createCTMemoryRecord(void *recordedMemoryPointer);
extern void _deleteRecord(struct CTMemoryRecord *record);

SUITE(test_basic);

struct test_CTDerivedStruct
{
    int data;
    int *pointer;
};

static void releaseCTDerivedStruct(void *object);
void releaseCTDerivedStruct(void *object){
    struct test_CTDerivedStruct *item = (struct test_CTDerivedStruct *)object;
    ctRelease(item->pointer);
}

TEST test_ctRetain_ctAlloc_ctRelease(void)
{
    struct test_CTDerivedStruct *item = (struct test_CTDerivedStruct *)ctAlloc(sizeof(struct test_CTDerivedStruct), releaseCTDerivedStruct);
    item->data = 10;
    item->pointer = (int *)ctAlloc(1, NULL);
    *(item->pointer) = 1;
    ctRelease(item);

    PASS();
}

SUITE(test_basic) {
    RUN_TEST(test_ctRetain_ctAlloc_ctRelease);
}
