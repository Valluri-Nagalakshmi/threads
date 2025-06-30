//  Global Variable Access Without Mutex Lock

#include <stdio.h>
#include <pthread.h>

int global_counter = 0;

void* increment(void* arg) {
    for (int i = 0; i < 100000; i++) {
        global_counter++;  // No lock!
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final global_counter (no lock): %d\n", global_counter);
    return 0;
}

