#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "greatest.h"
#include "CTMemory.h"

/* extern struct CTMemoryRecord; */
/* extern struct CTMemoryRecordList; */

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

void releaseCTDerivedStruct(struct test_CTDerivedStruct *item);
void releaseCTDerivedStruct(struct test_CTDerivedStruct *item){
    ctRelease(item->pointer);
    ctRelease(item);
}

TEST test_ctRetain_ctAlloc_ctRelease(void)
{
    struct test_CTDerivedStruct *item = (struct test_CTDerivedStruct *)ctAlloc(sizeof(struct test_CTDerivedStruct));
    item->data = 10;
    item->pointer = (int *)ctAlloc(1);
    releaseCTDerivedStruct(item);

    ASSERT(item->pointer == NULL);
    ASSERT(item == NULL);

    PASS();
}

TEST test_private_methods(void)
{
    struct CTMemoryRecordList *list = NULL;
    list = _sharedMemoryRecordList();
    ASSERT(list != NULL);

    struct test_CTDerivedStruct *item = (struct test_CTDerivedStruct *)ctAlloc(sizeof(struct test_CTDerivedStruct));
    item->data = 10;
    item->pointer = (int *)ctAlloc(1);
    releaseCTDerivedStruct(item);

    struct CTMemoryRecord *record = NULL;
    record = _findRecord(item);
    ASSERT(record != NULL);

    record = NULL;
    record = _createCTMemoryRecord((void *)0x12345);
    ASSERT(record != NULL);
    ASSERT(record->recordedMemoryPointer == 0x12345);

    _deleteRecord(record);
    ASSERT(record == NULL);

    PASS();
}

SUITE(test_basic) {
    RUN_TEST(test_ctRetain_ctAlloc_ctRelease);
    RUN_TEST(test_private_methods);
}
