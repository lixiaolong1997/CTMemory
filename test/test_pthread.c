#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

static void wait()
{
    time_t start_time = time(NULL);

    while (time(NULL) == start_time) {
    }
}

static void *thread_a_func(void *vptr_args)
{
    int i = 20;
    while (i --> 0) {
        printf("a\n");
        wait();
    }
}

int main()
{
    pthread_t thread;

    if (pthread_create(&thread, NULL, thread_a_func, NULL) != 0) {
        return 1;
    }

    int i = 20;
    while (i --> 0) {
        printf("b\n");
        wait();
    }

    if (pthread_join(thread, NULL) != 0) {
        return 1;
    }

    return 0;
}
