// Passing Multiple Arguments to Thread Function

#include <stdio.h>
#include <pthread.h>

typedef struct {
    int x;
    float y;
    char z;
} thread_data;

void* thread_func(void* arg) {
    thread_data* data = (thread_data*)arg;
    printf("x = %d, y = %.2f, z = %c\n", data->x, data->y, data->z);
    return NULL;
}

int main() {
    pthread_t thread;
    thread_data data = {10, 3.14, 'A'};

    pthread_create(&thread, NULL, thread_func, &data);
    pthread_join(thread, NULL);

    return 0;
}

