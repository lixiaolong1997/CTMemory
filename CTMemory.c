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
}

void ctRetain(void *item)
{
}

void ctRelease(void *item)
{
}

int ctAlloc(size_t size)
{
}
