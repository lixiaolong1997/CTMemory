#include "CTMemory.h"

/*
 * node of for reference count
 * */
struct CTMemoryRecord {
    void * recordedMemoryPointer;
    int retainCount;

    struct CTMemoryRecord *previous;
    struct CTMemoryRecord *next;

    pthread_mutex_t *memoryRecordListMutexRe;
};

struct CTMemoryRecord * _findRecord(void *recordedMemoryPointer);
struct CTMemoryRecordList * _sharedMemoryRecordList(void);
void _initMemoryRecordList(void);
struct CTMemoryRecord * _createCTMemoryRecord(void *recordedMemoryPointer);
void _deleteRecord(struct CTMemoryRecord *record);

/*
 * link list for reference count
 * */

struct CTMemoryRecordList {
    bool isEmpty;
    struct CTMemoryRecord *record;
};

static pthread_mutex_t memoryRecordListMutex = PTHREAD_MUTEX_INITIALIZER;

/* never use it directly in your code, use `sharedMemoryRecordList()` to get the record list. */
static struct CTMemoryRecordList *__sharedMemoryRecordList = NULL;

void* ctAlloc(size_t size)
{
    pthread_mutex_lock(&memoryRecordListMutex);

    void *allocedMemory = malloc(size);
    struct CTMemoryRecord *record = _createCTMemoryRecord(allocedMemory);
    struct CTMemoryRecordList *sharedList = _sharedMemoryRecordList();

    if (sharedList->isEmpty) {
        sharedList->record = record;
        sharedList->isEmpty = false;
    } else {
        struct CTMemoryRecord *currentRecord = sharedList->record;
        struct CTMemoryRecord *nextRecord = sharedList->record->next;

        record->previous = currentRecord;
        record->next = nextRecord;

        currentRecord->next = record;
        nextRecord->previous = record;
    }

    pthread_mutex_unlock(&memoryRecordListMutex);

    return allocedMemory;
}

void  ctRetain(void *memory)
{
    pthread_mutex_lock(&memoryRecordListMutex);
    struct CTMemoryRecord *record = _findRecord(memory);
    if (record) {
        record->retainCount++;
    }
    pthread_mutex_unlock(&memoryRecordListMutex);
}

void  ctRelease(void *memory)
{
    pthread_mutex_lock(&memoryRecordListMutex);
    struct CTMemoryRecord *record = _findRecord(memory);
    if (record) {
        record->retainCount--;
        if (record->retainCount == 0) {
            _deleteRecord(record);
        }
    }
    pthread_mutex_unlock(&memoryRecordListMutex);
}

/****************** Private Methods ******************/
struct CTMemoryRecord * _findRecord(void *recordedMemoryPointer)
{
    struct CTMemoryRecord *startRecord = (_sharedMemoryRecordList())->record;
    if (startRecord->recordedMemoryPointer == recordedMemoryPointer) {
        return startRecord;
    }

    struct CTMemoryRecord *resultRecord = NULL;
    while (startRecord->next != startRecord) {
        startRecord = startRecord->next;
        if (startRecord->recordedMemoryPointer == recordedMemoryPointer) {
            resultRecord = startRecord;
            break;
        }
    }
    return resultRecord;
}

void _deleteRecord(struct CTMemoryRecord *record)
{
    record->previous->next = record->next;
    record->next->previous = record->previous;
    free(record->recordedMemoryPointer);
    free(record);
}

struct CTMemoryRecord * _createCTMemoryRecord(void *recordedMemoryPointer)
{
    struct CTMemoryRecord *record = (struct CTMemoryRecord *)malloc(sizeof(struct CTMemoryRecord));
    record->recordedMemoryPointer = recordedMemoryPointer;
    record->retainCount = 1;
    record->previous = record;
    record->next = record;
    return record;
}

void _initMemoryRecordList()
{
    __sharedMemoryRecordList = (struct CTMemoryRecordList *)malloc(sizeof(struct CTMemoryRecordList));
    __sharedMemoryRecordList->isEmpty = true;
    __sharedMemoryRecordList->record = NULL;
}

struct CTMemoryRecordList * _sharedMemoryRecordList()
{
    if (__sharedMemoryRecordList == NULL) {
        static pthread_once_t onceToken = PTHREAD_ONCE_INIT;
        pthread_once(&onceToken, _initMemoryRecordList);
    }
    return __sharedMemoryRecordList;
}
