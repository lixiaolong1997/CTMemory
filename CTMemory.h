#ifndef __CTMEMORY_H__
#define __CTMEMORY_H__

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void* ctAlloc(size_t size);
void  ctRetain(void *memory);
void  ctRelease(void *memory);

#endif
