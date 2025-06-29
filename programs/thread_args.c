// Thread with Arguments

#include <stdio.h>
#include <pthread.h>

void* thread_function(void* arg) {
    int num = *(int*)arg;
    printf("Thread received value: %d\n", num);
    return NULL;
}

int main() {
    pthread_t thread;
    int val = 42;
    pthread_create(&thread, NULL, thread_function, &val);
    pthread_join(thread, NULL);
    return 0;
}

