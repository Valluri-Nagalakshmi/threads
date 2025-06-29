// Detached Threads (No Join Needed)

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_func(void* arg) {
    printf("Detached thread running\n");
    sleep(1);
    printf("Detached thread done\n");
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func, NULL);
    pthread_detach(thread);
    printf("Main will not wait for thread\n");
    sleep(2);
    return 0;
}

