#ifndef __CTMEMORY_H__
#define __CTMEMORY_H__

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * every dynamic alloc struct should have a pointer named "memoryControl" refer to this struct
 * */
struct CTMemoryControl {
    void (*dealloc)(void *item); // the dealloc function of your specific item.
    pthread_mutex_t memoryMutexLock;
    int retainCount;
};

/*
 *
 * initialize a CTMemoryControl
 *
 * */
int CTMemoryControlMake(struct CTMemoryControl *memoryControl, void (*deallocFunction)(void *item));

/*
 *
 * dealloc a CTMemoryControl
 *
 * */
int CTMemoryControlDealloc(struct CTMemoryControl *memoryControl);

/*
 *
 * every struct that want to use this function should always have a pointer refer to CTMemoryControl
 *
 * */
void ctRetain(void *item);

/*
 *
 * every struct that want to use this function should always have a pointer refer to CTMemoryControl
 *
 * */
void ctRelease(void *item);

/*
 *
 * every struct that want to use this function should always have a pointer refer to CTMemoryControl
 *
 * */
int ctAlloc(size_t size);

#endif
