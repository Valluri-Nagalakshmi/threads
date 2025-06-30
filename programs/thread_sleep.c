// Thread Sleep and Yield

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* worker(void* arg) {
    for (int i = 0; i < 3; i++) {
        printf("Worker thread running...\n");
        sleep(1);  // simulate work
        sched_yield();  // yield to another thread
    }
    return NULL;
}

int main() {
    pthread_t thread;

    pthread_create(&thread, NULL, worker, NULL);
    pthread_join(thread, NULL);

    return 0;
}

