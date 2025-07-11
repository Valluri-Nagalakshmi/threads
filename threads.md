# Threads
## Process
- A process is an independent program in execution.
- Has its own memory space and system resources.
- Can contain multiple threads inside it.
- Each process is managed by the OS separately.
- The operating system keeps track of each process using a **Process Control Block (PCB)**.
---
## Thread
- A thread is the smallest unit of execution within a process.
- Also known as a **lightweight process**.
- Threads within the same process share:
  - Code section
  - Data section
  - OS resources (files, signals)
- Each thread has its own:
  - Program Counter (PC)
  - Register set
  - Stack (for function calls)

---

## Function
- A function is a block of code designed to perform a specific task.
- Helps organize code and avoid repetition.
- Functions are called by programs to execute logic and return control once done.

---

##  Difference Between Functions and Threads:

| **Feature**     | **Function**                                | **Thread**                                     |
|-----------------|--------------------------------------------|------------------------------------------------|
| Purpose         | Organizes code into reusable blocks         | Executes parts of a program concurrently        |
| Execution       | Runs within a single flow of control        | Runs concurrently with other threads           |
| Memory          | Shares stack with main flow unless recursion| Has its own stack & registers, shares code/data |
| Parallelism     | Cannot provide parallel execution           | Supports parallelism (multi-core CPUs)          |
| Context Switch  | No context switching needed                 | Needs context switching for concurrency        |

---

##  Difference Between Process and Thread:

| **Feature**     | **Process**                                | **Thread**                                     |
|-----------------|--------------------------------------------|------------------------------------------------|
| Memory Space    | Separate (isolated) memory                  | Shares memory with other threads in same process |
| Dependency      | Independent                                 | Dependent on the process (cannot exist alone)   |
| Creation Cost   | Higher (more overhead)                      | Lower (lightweight)                            |
| Communication   | Complex (needs IPC mechanisms)              | Easy (shares memory space)                     |
| Parallelism     | True parallelism possible (multi-process)   | Parallelism within a process (multi-threading) |

---



###  Linux Threads:
- In modern Linux, threads **are provided by the Linux kernel**.
- Implemented using the `clone()` system call.
- Typically not used directly in apps — instead, libraries like **pthreads** are used.

---

##  Why Not Directly Use `clone()`?
1. **Complex API**:
   - Requires setting function pointers, stack locations, resource-sharing flags manually.
2. **Manual Stack Management**:
   - Developer must allocate and manage stack memory carefully.
3. **Low-level & Non-portable**:
   - Code becomes Linux-specific and difficult to maintain.
4. **Risk of Bugs**:
   - Easy to crash or corrupt memory without proper care.

---

##  What is pthread Library?
- **POSIX Threads (pthreads)**: Threading library for C/C++ on Unix-like systems.
- Used for multi-threaded programming.

###  What pthread Provides:
- Thread creation & termination:
  - `pthread_create`, `pthread_exit`
- Synchronization tools:
  - `pthread_join`, mutexes, condition variables
- Thread attributes:
  - Stack size, detach state, scheduling

### Why Use pthread?
- Efficient parallel/concurrent programming.
- Effective multi-core CPU usage.
- Used in systems programming, networking, and embedded systems.

---

##  Example pthread Functions:

| **Function**                       | **Purpose**                                      |
|------------------------------------|--------------------------------------------------|
| `pthread_create()`                 | Creates a new thread and starts its execution.   |
| `pthread_join()`                   | Waits for a thread to finish execution.          |
| `pthread_exit()`                   | Terminates the calling thread and returns value. |
| `pthread_detach()`                 | Marks a thread as detached (no `join` needed).   |
| `pthread_mutex_*()`                | Mutex operations for shared resources.           |
| `pthread_cond_*()`                 | Condition variables for thread signaling.        |
| `pthread_attr_*()`                 | Sets thread attributes (stack, scheduling, etc.).|
| `pthread_self()`, `pthread_equal()`| Gets thread ID and compares threads.             |

---

##  Detailed: `pthread_create()`

###  Purpose:
Create a new thread within a process.

###  Function Prototype:
```c
int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void *(*start_routine)(void *),
                   void *arg);
````

| **Parameter**    | **Meaning**                             |
| ---------------- | --------------------------------------- |
| \*thread         | Pointer to store thread ID (pthread\_t) |
| \*attr           | Thread attributes (NULL for defaults)   |
| \*start\_routine | Pointer to function to execute          |
| \*arg            | Argument passed to the thread function  |

###  Return Values:

* `0` → Success
* Non-zero → Error (like `EAGAIN`, `EINVAL`, etc.)

---

###  How It Works Internally:

1. Calls `clone()` with specific flags:

   * `CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD`
2. Allocates stack for the thread.
3. Sets up thread control block.
4. Shares process resources like memory and file descriptors.

---

###  Where Thread Info Is Stored:

| **Component**   | **Stored In**                        |
| --------------- | ------------------------------------ |
| pthread\_t ID   | Variable in your program             |
| Thread Stack    | Allocated by kernel or user          |
| Return Value    | Retrieved by `pthread_join()`        |
| TCB (Thread CB) | Kernel space (task\_struct in Linux) |
| /proc Info      | `/proc/<pid>/task/<tid>`             |

---

###  Notes:

* `pthread_create()` is non-blocking.
* Use `pthread_join()` to wait for thread completion.
* Arguments are passed via `void *` and casted inside thread function.

---

###  Check Threads in `/proc`:

```bash
ps -T -p <main_process_id>
ls /proc/<pid>/task/
```

---

##  Detailed: `pthread_join()`

###  Purpose:

Wait for a specific thread to finish and optionally retrieve its return value.

###  Prototype:

```c
int pthread_join(pthread_t thread, void **retval);
```

| **Parameter** | **Description**                                      |
| ------------- | ---------------------------------------------------- |
| thread        | Thread ID to wait for                                |
| retval        | Pointer to store thread’s return value (can be NULL) |

###  Return Values:

* `0` → Success
* `ESRCH` → No such thread
* `EINVAL` → Thread not joinable
* `EDEADLK` → Deadlock detected

### Internals:

* Checks thread joinability.
* Blocks caller until thread terminates.
* Cleans up thread resources (stack, descriptors).

---

###  Simple Example:

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

##  Advantages of Threads:

* Faster than processes (lightweight, quick context switch).
* Easy memory sharing (simpler communication).
* Utilize multiple cores for parallelism.
* Lower memory and creation costs.
* Ideal for I/O and real-time applications.

---

##  Common Thread Failures:

| **Failure Type**          | **Cause**                                                      |
| ------------------------- | -------------------------------------------------------------- |
| pthread\_create() Failure | Not enough resources (`EAGAIN`), invalid attributes (`EINVAL`) |
| Thread Function Crash     | Segmentation fault, stack overflow, use of invalid pointers    |
| Uncaught Exceptions       | Throwing C++ exceptions without catch                          |
| Race Conditions           | Shared data accessed without proper synchronization            |
| Deadlocks                 | Improper locking causing infinite waits                        |
| Too Many Threads          | OS limit reached (`ulimit -u`)                                 |
| Resource Leaks            | Not joining threads → Zombie threads or resource leaks         |

---

