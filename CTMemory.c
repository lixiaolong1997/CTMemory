#include "CTMemory.h"

struct CTMemoryRecord {
    int retainCount;
    void (* deallocMethod)(void *address);
    pthread_mutex_t *memoryRecordMutex;
};

void _createMemoryRecordTree(void);
struct CTAVLTreeRoot *_sharedMemoryRecordTree(void);

void _createMemoryRecordTreeMutex(void);
pthread_mutex_t *_sharedMemoryRecordTreeMutex(void);

struct CTAVLTreeRoot *__memoryRecordTree = NULL;
static pthread_mutex_t *__memoryRecordTreeMutex = NULL;

void * ctAlloc(size_t size, void (* deallocMethod)(void *address))
{
    void * allocedMemeory = malloc(size + sizeof(struct CTMemoryRecord) + sizeof(struct CTAVLTreeNode) + sizeof(pthread_mutex_t));

    if (allocedMemeory == NULL) {
        return NULL;
    }

    struct CTMemoryRecord *memoryRecord = (struct CTMemoryRecord *)(((uint64_t)allocedMemeory) + size);
    memoryRecord->retainCount = 1;
    memoryRecord->deallocMethod = deallocMethod;
    memoryRecord->memoryRecordMutex = (pthread_mutex_t *)((uint64_t)memoryRecord + sizeof(struct CTMemoryRecord));
    if (pthread_mutex_init(memoryRecord->memoryRecordMutex, NULL)) {
        free(allocedMemeory);
        return NULL;
    }

    struct CTAVLTreeNode *recordNode = (struct CTAVLTreeNode *)((uint64_t)memoryRecord + sizeof(struct CTMemoryRecord) + sizeof(pthread_mutex_t));
    recordNode->key = allocedMemeory;
    recordNode->value = (void *)memoryRecord;

    struct CTAVLTreeRoot *root = _sharedMemoryRecordTree();

    pthread_mutex_lock(_sharedMemoryRecordTreeMutex());
    insertCTAVLTreeNode(recordNode, root);
    pthread_mutex_unlock(_sharedMemoryRecordTreeMutex());

    return allocedMemeory;
}

void ctRetain(void *memory)
{
    struct CTAVLTreeNode *node = findCTAVLTreeNode(memory, _sharedMemoryRecordTree());
    struct CTMemoryRecord *memoryRecord = (struct CTMemoryRecord *)node->value;
    pthread_mutex_lock(memoryRecord->memoryRecordMutex);
    memoryRecord->retainCount++;
    pthread_mutex_unlock(memoryRecord->memoryRecordMutex);
}

void ctRelease(void *memory)
{
    struct CTAVLTreeNode *node = findCTAVLTreeNode(memory, _sharedMemoryRecordTree());
    struct CTMemoryRecord *memoryRecord = (struct CTMemoryRecord *)node->value;

    pthread_mutex_lock(memoryRecord->memoryRecordMutex);
    memoryRecord->retainCount--;
    pthread_mutex_unlock(memoryRecord->memoryRecordMutex);

    if (memoryRecord->retainCount == 0) {
        if (memoryRecord->deallocMethod != NULL) {
            memoryRecord->deallocMethod(memory);
        }
        deleteCTAVLTreeNode(memory, _sharedMemoryRecordTree());
        free(memory);
    }
}

/************ Private Methods **************/
void _createMemoryRecordTree()
{
    __memoryRecordTree = (struct CTAVLTreeRoot *)malloc(sizeof(struct CTAVLTreeRoot));
    __memoryRecordTree->rootNode = NULL;
    __memoryRecordTree->compare = NULL;
}

struct CTAVLTreeRoot *_sharedMemoryRecordTree()
{
    if (__memoryRecordTree == NULL) {
        static pthread_once_t onceToken = PTHREAD_ONCE_INIT;
        pthread_once(&onceToken, _createMemoryRecordTree);
    }
    return __memoryRecordTree;
}

void _createMemoryRecordTreeMutex()
{
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
    __memoryRecordTreeMutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(__memoryRecordTreeMutex, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);
}

pthread_mutex_t *_sharedMemoryRecordTreeMutex()
{
    if (__memoryRecordTreeMutex == NULL) {
        static pthread_once_t onceToken = PTHREAD_ONCE_INIT;
        pthread_once(&onceToken, _createMemoryRecordTreeMutex);
    }
    return __memoryRecordTreeMutex;
}
