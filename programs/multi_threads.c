// Multiple Threads

#include <stdio.h>
#include <pthread.h>

void* thread_func(void* arg) {
    int id = *(int*)arg;
    printf("Thread %d is running\n", id);
    return NULL;
}

int main() {
    pthread_t threads[5];
    int ids[5];

    for (int i = 0; i < 5; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_func, &ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

