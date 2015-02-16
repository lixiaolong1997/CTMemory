#CTMemory

This is a reference counter for any C project, safe in multithreaded environment.

Before Compile
--------------

- go to ./deps to install jemalloc in your system.

    ```
        autoconf
        configure
        make
        make install
    ```

- compile `CTAVLTree.c` and `CTMemory.c` in your project, remember add `-pthread` when compile.

Quick Usage
-----------

#####ctAlloc

```C
    void * ctAlloc(size_t size, void (* deallocMethod)(void *address));
```

`size`: the memory size you want to allocate. The reference count will be 1 when memory allocated.  
`deallocMethd`: a pointer refer to a function which will be called before free the memeory. You can set it to `NULL` if you don't want callback.  

sample:

```C
    struct Memory *memory = (struct Memory *)ctAlloc(sizeof(struct Memory), NULL);
```

---

#####ctRelease

```C
    void ctRelease(void *memory);
```

`memory`: the memory you want to decrease the reference count. If reference count goes to 0, the memory will be freed. If the memory is not allocated with `ctAlloc`, nothing will happen.  

sample:

```C
    ctRelease(memory);
```

---

#####ctRetain

```C
    void ctRetain(void *memory);
```

`memory`: the memory you want to increase the reference count. If the memory is not allocated with `ctAlloc`, nothing will happen.

sample:

```C
    ctRetain(memory);
```

Sample
------

```C

    #include "CTMemory.h"

    static void beforeDealloc(void *memoryAddress);
    void setDynamicMemoryToOtherStruct(struct DynamicMemeory *memory, struct OtherStruct *otherStruct);
    void operationsOnDynamicMemory(struct DynamicMemeory *memory);

    void beforeDealloc(void *memoryAddress)
    {
        struct DynamicMemeory *item = (struct DynamicMemeory *)memoryAddress;
        notifyItemWillBeDealloced(item);
        ctRelease(item->anotherDynamicMemeoryPointer);
        makeALog(item);
    }

    void setDynamicMemoryToOtherStruct(struct DynamicMemeory *memory, struct OtherStruct *otherStruct)
    {
        ctRetain(memory);
        otherStruct->customizedMemory = memory;
    }

    void multiThreadOperationsOnDynamicMemory(struct DynamicMemeory *memory)
    {
        ctRetain(memory);

        /*
            your operations on this memory.
        */

        ctRelease(memory);
    }


    int main(void){

        struct DynamicMemeory *memory = (struct DynamicMemeory *)ctAlloc(sizeof(struct DynamicMemeory), beforeDealloc);
        /* struct DynamicMemeory *memory = (struct DynamicMemeory *)ctAlloc(sizeof(struct DynamicMemeory), NULL); */
        memory->anotherDynamicMemeoryPointer = (int *)ctAlloc(1);
        ctRelease(memory);
    }

```

Acknowledgements
----------------

jemalloc: https://github.com/jemalloc/jemalloc
greatest: https://github.com/silentbicycle/greatest 
