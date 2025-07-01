#  Linux Threads Programming Using POSIX `pthreads` in C

This repository contains a collection of C programs that demonstrate multithreading in Linux using the POSIX thread (`pthread`) API. It explains the fundamentals of threads, how they are created, synchronized, and safely managed within a process.

---

##  What Are Threads?

A **thread** is the smallest unit of execution within a process. Unlike separate processes, threads **share the same memory space**, file descriptors, and resources, making them lightweight and efficient for parallel execution.

In Linux, threads are implemented using the **POSIX Threads (pthreads)** standard, which provides system-level functions to create and manage threads in C.

---

##  Why Use Threads?

- To run multiple tasks concurrently (e.g., downloading files, processing data, handling UI)
- To better utilize multicore processors
- To reduce the overhead of creating full processes
- To write efficient, high-performance applications

---

##  Topics Covered in This Repository

| Topic                         | Description                                                                 |
|-------------------------------|-----------------------------------------------------------------------------|
| **Thread Creation**           | Using `pthread_create()` to spawn threads                                  |
| **Thread Termination**        | Gracefully exiting with `pthread_exit()`                                   |
| **Joining Threads**           | Using `pthread_join()` to wait for thread completion                       |
| **Detaching Threads**         | Letting threads clean up after themselves with `pthread_detach()`          |
| **Race Conditions**           | Understanding issues caused by unsynchronized access to shared data        |
| **Mutex Locks**               | Preventing race conditions with `pthread_mutex_*`                          |
| **Condition Variables**       | Signaling and coordination between threads using `pthread_cond_*`          |
| **Producer–Consumer Problem** | A practical use case for mutexes and condition variables                   |

---

##  POSIX Thread Functions Used

| Function                    | Purpose                                 |
|-----------------------------|-----------------------------------------|
| `pthread_create()`         | Create a new thread                     |
| `pthread_join()`           | Wait for a thread to complete           |
| `pthread_exit()`           | Exit from a thread                      |
| `pthread_detach()`         | Detach a thread (no need to `join`)     |
| `pthread_mutex_init()`     | Initialize a mutex                      |
| `pthread_mutex_lock()`     | Acquire a lock                          |
| `pthread_mutex_unlock()`   | Release a lock                          |
| `pthread_mutex_destroy()`  | Destroy a mutex                         |

---



