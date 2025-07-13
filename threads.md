
# Threads and Processes in Linux

##  What is a Process?
- A **process** is an independent program in execution.
- Has its own **memory space** and **system resources**.
- Can contain **multiple threads** inside it.
- Each process is managed by the OS separately.

The OS tracks each process using a **Process Control Block (PCB)**.

---

## What is a Thread?
- A **thread** is the smallest unit of execution within a process.
- Also known as a **lightweight process**.

### Threads within a process **share**:
- Code section
- Data section
- OS resources (e.g., files, signals)

### Each thread has its own:
- Program Counter (PC)
- Registers
- Stack

---

## What is a Function?
- A **function** is a block of code designed to perform a specific task.
- Organizes code and avoids repetition.
- Runs in the **same execution flow** as the caller.

---

## Difference: Function vs Thread

| Feature        | Function                                   | Thread                                          |
|----------------|--------------------------------------------|--------------------------------------------------|
| Purpose        | Code organization                         | Parallel/concurrent execution                    |
| Execution      | Single flow                               | Concurrent (runs with other threads)             |
| Memory         | Uses same stack                           | Own stack, shared code/data                      |
| Parallelism    | No                                         | Yes (on multi-core systems)                      |
| Context Switch | Not needed                                | Requires context switching                       |

Use **functions** for small/sequential tasks, and **threads** for parallel/independent work.

---

## Difference: Process vs Thread

| Feature          | Process                         | Thread                               |
|------------------|----------------------------------|---------------------------------------|
| Memory Space     | Separate                        | Shared with other threads             |
| Dependency       | Independent                     | Dependent on process                  |
| Creation Cost    | High                            | Low (lightweight)                     |
| Communication    | Complex (IPC needed)            | Easy (shared memory)                  |
| Parallelism      | Across processes                | Inside a single process               |

---

## Threads in Linux
- Threads in Linux are created using the **`clone()`** system call.
- Each thread is a **lightweight process** (uses shared resources).
- **`pthread` library** (POSIX threads) provides user-friendly APIs for threads.

---

## What is `pthread` Library?
- Stands for **POSIX Threads**
- Used for **multi-threaded programming** in Unix/Linux
- Internally uses `clone()` but hides complexity

### `pthread` Provides:
- Thread creation and termination: `pthread_create()`, `pthread_exit()`
- Thread synchronization: `pthread_join()`, `pthread_mutex_*`, `pthread_cond_*`
- Attribute management: `pthread_attr_*`

---
##  Example `pthread` Functions

| Function                        | Purpose                                                                 |
|----------------------------------|-------------------------------------------------------------------------|
| `pthread_create()`              | Creates a new thread and starts its execution.                          |
| `pthread_join()`                | Waits for a specific thread to finish execution.                        |
| `pthread_exit()`                | Terminates the calling thread and optionally returns a value.           |
| `pthread_detach()`              | Marks a thread as detached; resources are released automatically after it terminates (no join needed). |
| `pthread_mutex_*()`             | Provides mutex (mutual exclusion) operations to protect shared resources from concurrent access. |
| `pthread_cond_*()`              | Manages condition variables for thread synchronization (signal/wait mechanisms). |
| `pthread_attr_*()`              | Sets thread attributes like stack size, scheduling policy, and priority before creating threads. |
| `pthread_self()`, `pthread_equal()` | Gets the calling thread’s ID and compares thread IDs  |

---

## `pthread_create()`

### Prototype:
```
int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void *(*start_routine)(void *),
                   void *arg);
````

| Parameter       | Description                            |
| --------------- | -------------------------------------- |
| `thread`        | Stores thread ID                       |
| `attr`          | Thread attributes (`NULL` for default) |
| `start_routine` | Function to execute in new thread      |
| `arg`           | Argument passed to the thread function |

---

## How `pthread_create()` Works Internally

1. Uses `clone()` with flags:

   * `CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD`
2. Kernel allocates:

   * Stack
   * TCB (Thread Control Block)
3. Thread shares:

   * Address space, file descriptors, signals, heap

---

## `pthread_join()`

### Prototype:

```c
int pthread_join(pthread_t thread, void **retval);
```

* **Waits** for a specific thread to finish.
* **Returns** value from the thread function.
* Cleans up resources (stack, descriptors).

### Internally:

* Uses **futexes** (fast userspace mutexes)
* Waits on the **thread’s termination flag**
* Cleans up thread kernel data

---

## Simple Example: `pthread_create()` + `pthread_join()`

```c
#include<stdio.h>
#include<pthread.h>

void* func(void* arg) {
    printf("Hello from thread!\n");
    return NULL;
}

int main() {
    pthread_t t1;
    pthread_create(&t1, NULL, func, NULL);
    pthread_join(t1, NULL);
    printf("Thread finished.\n");
    return 0;
}
```

---

## Advantages of Threads

- Faster than processes (lightweight, quick context switch)
- Share memory → easy communication between threads
- Utilizes multiple cores → true parallel execution
- Low memory & creation cost → efficient
- Better responsiveness in applications (UI stays responsive)
- Ideal for I/O-bound, real-time, and multitasking programs

---

##  Thread Failures

### 1. `pthread_create()` Failure (Thread Not Created)
- Reason: Failure during thread creation
- Causes:
  - Not enough system resources (`EAGAIN`)
  - Invalid thread attributes (`EINVAL`)
  - Lack of permissions (`EPERM`)

---

### 2. Thread Function Crash (Inside Thread)
- Causes:
  - Dereferencing invalid pointers (Segmentation Fault)
  - Stack overflow (e.g., deep recursion)
  - Use of freed memory or NULL pointers

####  Example:
```c
void* thread_func(void* arg) {
    int *ptr = NULL;
    *ptr = 10;  // Crash: Segmentation Fault
}
````

---

### 3. Uncaught Exception (C++ Only)

* Throwing an exception inside a thread function without catching it may terminate the entire program.

---

### 4. Race Conditions / Memory Corruption

* Happens when multiple threads access shared resources without proper synchronization
* Leads to:

  * Unpredictable behavior
  * Crashes
  * Data corruption

---

### 5. Deadlocks

* A thread waits forever for a lock that is never released (held by itself or another thread)

####  Example:

```c
pthread_mutex_lock(&lock);
// Forgot to unlock → Causes deadlock
```

---

### 6. Too Many Threads

* OS limit reached (per-user process limit exceeded)
* `pthread_create()` fails due to `ulimit` exceeded

Use to check:

```bash
ulimit -u  # Shows maximum user processes (includes threads)
```

---

### 7. Resource Leaks (Zombie Threads)

* Happens when threads are created but not joined or detached
* Results in resource leakage and zombie threads

---

## Parallel Execution Example

```c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* func(void* arg) {
    for (int i = 0; i < 10; i++) {
        printf("this is thread function\n");
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t t1;
    pthread_create(&t1, NULL, func, NULL);
    // pthread_join(t1, NULL);  // Optional: uncomment to block immediately

    for (int i = 0; i < 10; i++) {
        printf("this is main function\n");
        sleep(1);
    }

    pthread_join(t1, NULL);
    return 0;
}
```

### Output:

```
this is main function
this is thread function
this is main function
this is thread function
...
this is main function
this is thread function
```

* Both the main thread and the created thread execute **in parallel**.

```

---


## Passing Arguments to Threads

### Passing a Single Integer

```c
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
```

### Passing Multiple Values (Struct)

```c
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
```

---

## Returning Value from a Thread

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* compute(void* arg) {
    int* result = malloc(sizeof(int));
    *result = 42;
    return result;
}

int main() {
    pthread_t tid;
    void* retval;
    pthread_create(&tid, NULL, compute, NULL);
    pthread_join(tid, &retval);
    printf("Returned: %d\n", *(int*)retval);
    free(retval);
    return 0;
}
```

---

## Critical Section

> A critical section is a region of code that **accesses shared resources** (variables, memory, files), which must **not be executed by more than one thread at a time**.

### Without Mutex (Race Condition)

```c
#include<stdio.h>
#include<pthread.h>

int global_counter = 0;

void* increment(void* arg) {
    for (int i = 0; i < 100000; i++) {
        global_counter++;  // Unsafe
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
```

### With Mutex (Safe)

```c
#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t lock;

void* increment(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&lock);
    printf("Final counter value: %d\n", counter);
    return 0;
}
```

---


