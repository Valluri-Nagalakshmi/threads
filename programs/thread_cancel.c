// Thread Cancellation

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* long_task(void* arg) {
    for (int i = 0; i < 10; i++) {
        printf("Working %d...\n", i);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t thread;

    pthread_create(&thread, NULL, long_task, NULL);
    sleep(3);  // Let it run for a while
    pthread_cancel(thread);
    pthread_join(thread, NULL);

    printf("Thread was cancelled\n");
    return 0;
}

