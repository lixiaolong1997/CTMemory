#include "CTMemory.h"

int CTMemoryControlMake(struct CTMemoryControl *memoryControl, void (*deallocFunction)(void *item))
{
    memoryControl = (struct CTMemoryControl *)malloc(sizeof(struct CTMemoryControl));
    memoryControl->retainCount = 1;
    memoryControl->dealloc = deallocFunction;

    int errno;
    pthread_mutexattr_t mutexAttr;

    errno = pthread_mutexattr_init(&mutexAttr);
    if (errno) {
        perror("[ CTMemory::CTMemoryControlMake::pthread_mutexattr_init ] ");
        return 1;
    }

    errno = pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
    if (errno) {
        perror("[ CTMemory::CTMemoryControlMake::pthread_mutexattr_settype ] ");
        return 1;
    }

    errno = pthread_mutex_init(&(memoryControl->memoryMutexLock), &mutexAttr);
    if (errno) {
        perror("[ CTMemory::CTMemoryControlMake::pthread_mutex_init ] ");
        return 1;
    }

    errno = pthread_mutexattr_destroy(&mutexAttr);
    if (errno) {
        perror("[ CTMemory::CTMemoryControlMake::pthread_mutexattr_destroy ] ");
    }

    return 0;
}

int CTMemoryControlDealloc(struct CTMemoryControl *memoryControl)
{
    memoryControl->retainCount = 0;
    return 0;
}

void ctRetain(void *item)
{
    struct CTMemoryControl *node = (struct CTMemoryControl *)item;
    node->retainCount = 0;
}

void ctRelease(void *item)
{
    struct CTMemoryControl *node = (struct CTMemoryControl *)item;
    node->retainCount = 0;
}

int ctAlloc(size_t size)
{
    size = 0;
    return 0;
}
