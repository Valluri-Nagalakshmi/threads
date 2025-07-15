# Threads
## What is a Process?
- A process is an independent program in execution.
- Has its own memory space and system resources.
- Can contain multiple threads inside it.
- Each process is managed by the OS separately.
> The operating system keeps track of each process using a Process Control Block (PCB).

## What is a Thread?
- A thread is the smallest unit of execution within a process.
- It’s sometimes called a lightweight process.
- Threads within the same process share:
  - Code section
  - Data section
  - OS resources (files, signals)
- Each thread has its own:
  - Program Counter (PC)
  - Register set
  - Stack (for function calls)

## What is a Function?
- A function is a block of code designed to perform a specific task.
- It’s used to organize code and avoid repetition.
- Functions are called by the program to execute certain logic, and they return control back to the caller once done.

## Difference Between Functions and Threads

| Feature      | Function                                | Thread                                       |
|--------------|------------------------------------------|----------------------------------------------|
| Purpose      | Organizes code into reusable blocks      | Executes parts of a program concurrently     |
| Execution    | Runs within a single flow of control     | Runs concurrently with other threads         |
| Memory       | Shares stack with main flow unless recursion happens | Has its own stack and registers, shares code/data |
| Parallelism  | Cannot provide parallel execution        | Supports parallelism (on multi-core CPUs)    |
| Context Switch | No context switching needed            | Needs context switching for concurrent execution |

### If tasks are small and/or sequential
- Functions are faster
- Threads add overhead for creation, context switching, and synchronization

### If tasks are independent and CPU/I/O heavy
- Threads are faster (due to parallel execution)
- Threads run on multiple cores → simultaneous execution.

## Difference Between Process and Thread

| Feature          | Process                     | Thread                                          |
|------------------|-----------------------------|-------------------------------------------------|
| Memory Space     | Separate (isolated) memory  | Shares memory with other threads in same process|
| Dependency       | Independent                 | Dependent on the process (cannot exist alone)   |
| Creation Cost    | Higher (more overhead)      | Lower (lightweight)                             |
| Communication    | Complex (needs IPC mechanisms) | Easy (shares memory space)                    |
| Parallelism      | True parallelism possible (multi-process) | Parallelism within a process (multi-threading) |

### In modern Linux systems, threads ARE provided by the Linux kernel itself.
- Linux does provide threads, but they are treated as lightweight processes internally.
- In Linux, threads are implemented using kernel-level mechanisms.
- Specifically, Linux uses the `clone()` system call to create threads.
- You don't directly call `clone()` in most applications.
- Instead, you use a thread library like pthreads (POSIX Threads).
- `pthread` provides easy APIs like `pthread_create()` to create threads.
- Internally, pthreads uses the Linux `clone()` system call to create threads.

## Why We Don't Directly Use `clone()` for Threads:

### 1. Complexity of `clone()` API:
- `clone()` is a low-level system call that requires complex parameters:
  - You must manually set:
    - Function pointers
    - Stack locations
    - Flags for resource sharing
- You must carefully allocate and manage the thread stack manually.

# What is pthread Library?
- `pthread` stands for POSIX Threads.
- It is a threading library for C/C++ in Unix-like operating systems (Linux, macOS).
- Used for multi-threaded programming.

## What does pthread provide?
- Functions to:
  - Create and terminate threads (`pthread_create`, `pthread_exit`)
  - Synchronize threads (`pthread_join`, mutexes, condition variables)
  - Manage thread attributes (stack size, detach state, etc.)

## Why Use pthread?
- To write parallel or concurrent programs.
- Makes efficient use of multi-core CPUs.
- Handles background tasks, I/O, computation-heavy tasks in separate threads.
- Used in system software, networking apps, embedded systems, etc.

## Example pthread Functions

| Function             | Purpose                                                                           |
|----------------------|-----------------------------------------------------------------------------------|
| `pthread_create()`   | Creates a new thread and starts its execution.                                    |
| `pthread_join()`     | Waits for a specific thread to finish execution.                                  |
| `pthread_exit()`     | Terminates the calling thread and optionally returns a value.                     |
| `pthread_detach()`   | Marks a thread as detached; its resources are released automatically after it terminates (no join needed). |
| `pthread_mutex_*()`  | Provides mutex (mutual exclusion) operations to protect shared resources from concurrent access. |
| `pthread_cond_*()`   | Manages condition variables for thread synchronization (signal/wait mechanisms).  |
| `pthread_attr_*()`   | Sets thread attributes like stack size, scheduling policy, and priority before creating threads. |
| `pthread_self()`, `pthread_equal()` | Gets the calling thread’s ID and compares thread IDs.             |

---

## 1. What is pthread_create()?

`pthread_create()` is a POSIX function that creates a new thread within the calling process.

**Purpose:**  
To run a function (your logic) in parallel with the main thread.

## 2. Function Prototype

```c
int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void *(*start_routine) (void *),
                   void *arg);
````

| Parameter        | Meaning                                              |
| ---------------- | ---------------------------------------------------- |
| \*thread         | Returns thread ID (`pthread_t`)                      |
| \*attr           | Thread attributes (pass NULL for default attributes) |
| \*start\_routine | Pointer to the function to execute in the thread     |
| \*arg            | Argument passed to the thread function (can be NULL) |

## 3. What Does It Return?

* `0` → Success
* Non-zero → Error (e.g. `EINVAL`, `ENOMEM`, `EAGAIN`)

Use it like:

```c
if (pthread_create(&tid, NULL, my_function, NULL) != 0) {
    perror("Thread creation failed");
}
```

## 4. How `pthread_create()` Internally Works

Internally:

1. `pthread_create()` calls the `clone()` system call (not `fork()`).
2. It passes special flags like:
   `CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD`
   → These tell the kernel to create a lightweight process (i.e. a thread).
3. The kernel allocates:

   * A new stack for the thread
   * Its own thread control block (TCB)
4. Thread shares:

   * Address space
   * Open file descriptors
   * Signals
   * Heap

So, `pthread_create()` is just a wrapper around `clone()` with additional book keeping (like TLS, pthread ID management, etc.).

## Where Is the Thread Info Stored?

| Component            | Stored In                                       |
| -------------------- | ----------------------------------------------- |
| `pthread_t` ID       | Variable in your program                        |
| Thread stack         | Allocated by kernel or as per attributes        |
| Return value         | Can be retrieved using `pthread_join()`         |
| TCB (Thread Control) | Stored in kernel space (`task_struct` in Linux) |
| /proc info           | See `/proc/<pid>/task/<tid>` directory          |

* `pthread_create()` does not block — it returns immediately, and the thread runs independently.
* `pthread_join()` is needed to wait for the thread to finish and clean up resources.
* You can pass data using `void *arg`, cast it to whatever type you need in the thread.


---

Here's the **remaining part** of your content in `.md` (Markdown) format, continuing from where we left off:

````markdown
# pthread_join()

## 1. What is pthread_join()?
**Definition:**  
`pthread_join()` is used by a thread (usually the main thread) to wait for another thread to finish execution.

It ensures that:
- The calling thread waits until the specified thread terminates.
- Any return value from the thread can be retrieved.

## 2. Function Prototype

```c
int pthread_join(pthread_t thread, void **retval);
````

| Parameter | Description                                                                  |
| --------- | ---------------------------------------------------------------------------- |
| thread    | The thread ID to wait for (returned by `pthread_create`)                     |
| retval    | Pointer to a variable that will hold the thread's return value (can be NULL) |

## 3. What It Returns

| Return Value | Meaning                |
| ------------ | ---------------------- |
| 0            | Success                |
| ESRCH        | No such thread         |
| EINVAL       | Thread is not joinable |
| EDEADLK      | Deadlock detected      |

## 4. What pthread\_join() Does Internally

Steps:

1. Checks if thread is joinable (i.e., not detached).
2. Waits (blocks) for the thread to terminate.
3. Copies the return value (from `pthread_exit()` or function return) into `*retval`.
4. Cleans up thread resources (thread stack, descriptors).
5. Internally uses:

   * Futexes (fast userspace mutexes) for blocking/waking.
   * Waits on thread's termination flag.
6. It also interacts with kernel's `task_struct` via clone()’d thread.

## Simple Example with pthread\_create() and pthread\_join()

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

**Output:**

```
Hello from thread!
Thread finished.
```

---

# Advantages of Threads

* Faster than processes (lightweight, quick context switch).
* Share memory, so communication is easy.
* Use multiple cores → true parallel execution.
* Low memory & creation cost (efficient).
* Better responsiveness in apps (UI stays active).
* Ideal for I/O, real-time, and multitasking.

---

# Thread Failures

## 1. pthread\_create() Failure (Thread Not Created)

**Causes:**

* Not enough system resources (`EAGAIN`)
* Invalid thread attributes (`EINVAL`)
* Lack of permissions (`EPERM`)

## 2. Thread Function Crash (Inside Thread)

**Causes:**

* Dereferencing invalid pointers (Segmentation Fault)
* Stack overflow (Deep recursion)
* Use of freed memory or NULL pointers.

```c
void* thread_func(void* arg) {
    int *ptr = NULL;
    *ptr = 10;  // Crash: Segmentation Fault
}
```

## 3. Uncaught Exception (C++ Only)

* Throwing an exception inside a thread function without catching it may terminate the program.

## 4. Race Conditions / Memory Corruption

* Occurs when multiple threads access shared resources without synchronization.
* Leads to unpredictable results, crashes, or corruption.

## 5. Deadlocks

* A thread waits forever for a lock held by itself or another thread that will never release it.

```c
pthread_mutex_lock(&lock);
// Forgot to unlock → Causes deadlock
```

## 6. Too Many Threads

* OS limit reached (per-user process limit).
* Thread creation fails due to exceeding `ulimit`.

```bash
ulimit -u  # Shows maximum user processes (includes threads)
```

## 7. Resource Leaks (Zombie Threads)

* Occurs when threads are not joined (`pthread_join()` not called), causing resource leaks or zombie threads.

---

# Parallel Execution Example

```c
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void* func(void* arg){
    int i=0;
    for(i=0;i<10;i++){
        printf("this is thread function\n");
        sleep(1);
    }
}

void main(){
    pthread_t t1;
    pthread_create(&t1,NULL,func,NULL);
    //pthread_join(t1,NULL);
    for(int i=0;i<10;i++){
        printf("this is main function\n");
        sleep(1);
    }
    pthread_join(t1,NULL);
}
```

**Output:**

```
this is main function
this is thread function
this is main function
this is thread function
....
this is main function
this is thread function
```

---

# Passing Argument to Thread

## Passing a Single Argument

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

**Output:**

```
Thread received value: 42
```

---

## Passing Multiple Arguments Using Structure

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

**Output:**

```
x = 10, y = 3.14, z = A
```

---

# Returning a Value from a Thread

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

**Output:**

```
Returned: 42
```

---

# Critical Section
-A critical section is a part of a program (usually a block of code) where shared resources (like variables, files, or devices) are accessed or modified.

## Without Lock (Race Condition)

```c
#include<stdio.h>
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
```

**Output (inconsistent):**

```
Final global_counter (no lock): 104178
```

## With Lock (Mutex)

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

**Output:**

```
Final counter value: 200000
```

---

# Thread Attributes Summary

In POSIX pthreads, thread attributes are represented using the `pthread_attr_t` structure. These attributes control the behavior of a thread when it's created using `pthread_create()`.

## 1. Detach State

```c
pthread_attr_t attr;
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
// or PTHREAD_CREATE_JOINABLE
```

* Use detached when you don’t need the thread’s return value.
* Use joinable when you need to `pthread_join()` later.

## 2. Stack Size

```c
size_t stack_size = 1024 * 1024;  // 1 MB
pthread_attr_setstacksize(&attr, stack_size);
```

* Increase if your thread does deep recursion or allocates large arrays on stack.
* Default is typically 8 MB on Linux.

## 3. Scheduling Policy

```c
pthread_attr_setschedpolicy(&attr, SCHED_RR);
```

* Policies:

  * `SCHED_OTHER`: Default
  * `SCHED_FIFO`: Real-time FIFO
  * `SCHED_RR`: Real-time Round Robin

## 4. Scheduling Priority

```c
struct sched_param param;
param.sched_priority = 20;
pthread_attr_setschedparam(&attr, &param);
```

* Requires a real-time scheduling policy (not SCHED\_OTHER).

## 5. Inherit Scheduling

```c
pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
// or PTHREAD_INHERIT_SCHED
```

## 6. Thread Name (Linux-specific)

```c
pthread_setname_np(pthread_self(), "WorkerThread");
```

* Useful for debugging (htop, gdb, logs)

## 7. Thread Group (Java-only Concept)

* Not available in C/pthreads.
* Java allows grouping of threads for management.

---

## Full Example with Thread Attributes

```c
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

void* thread_func(void* arg) {
    printf("Running thread\n");
    return NULL;
}

int main() {
    pthread_t t;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // Set detached state
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Set stack size
    pthread_attr_setstacksize(&attr, 1024 * 1024);  // 1MB

    // Set scheduling policy
    pthread_attr_setschedpolicy(&attr, SCHED_RR);

    // Set scheduling priority
    struct sched_param param;
    param.sched_priority = 10;
    pthread_attr_setschedparam(&attr, &param);

    // Set inherit scheduling to use explicit values
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    pthread_create(&t, &attr, thread_func, NULL);
    pthread_join(t, NULL);

    pthread_attr_destroy(&attr);
    return 0;
}
```

## Execution:

```bash
cc thread_attr.c
sudo ./a.out
```

**Output:**
- Running thread
---



