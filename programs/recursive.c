//  Recursive Mutex Example

#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;

void recursive_function(int count) {
    if (count == 0) return;

    pthread_mutex_lock(&lock);
    printf("In recursive call: %d\n", count);
    recursive_function(count - 1);
    pthread_mutex_unlock(&lock);
}

int main() {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&lock, &attr);

    recursive_function(3);

    pthread_mutex_destroy(&lock);
    return 0;
}

