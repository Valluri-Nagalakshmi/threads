// Thread Returning a Value

#include <stdio.h>
#include <pthread.h>

void* compute_square(void* arg) {
    int num = *(int*)arg;
    int* result = malloc(sizeof(int));
    *result = num * num;
    return result;
}

int main() {
    pthread_t thread;
    int num = 6;
    int* square;

    pthread_create(&thread, NULL, compute_square, &num);
    pthread_join(thread, (void**)&square);

    printf("Square of %d is %d\n", num, *square);
    free(square);
    return 0;
}

