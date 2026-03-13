# Concurrency 1

## Intro
- Concurrency means executing a multiple piece of code within the same program simultaneously. 
- The operating system is one of the first concurrent piece of software
- Threads allow us to execute the piece of code simultaneously.
- Threads shares the heap data, but they have different stacks each.
- Run the program: [program-which-will-not-give-correct-output](./code/i-sharing-data.cpp)
- The program output can be random, this is because of how thread works and how instructions are executed, there might be the case that thread A and thread B, both have updated the variable with +1, so overall value should have increased by 2, but due to the instructions in low level, the update might be only 1 or 2.
- If we share data across theads then there might be possible race condition, we need to lock that piece of code when certain thread is executing it, in this way our variable won't get operated by multiple threads.
- The piece of code which executes by multiple threads is called critical section.
- We need to implement mutual exclusion property on critical section.


## Thread in C

- We use posix library, `pthread` to create and manage thread in c.
- To create a thread:
```c
pthread_t p;
pthread_create(&p, NULL, task, &args); // task is routine to be executed with arguments args
```
- See program: [thread-in-c-and-return-value](./code//ii-thread-return-value.c)

## Locks

- For mutual exclusion, we need to have lock some part of the code, and that can be done by using `posix` locks.

```c
// define lock variable
pthread_mutex_t lock;

pthread_mutex_lock(&lock);
/*
    critical section
*/
pthread_mutex_unlock(&lock);
```

### Initialization of lock

1.  ```c
    pthread_lock_t lock = PTHREAD_MUTEX_INITIALIZER;
    ```
2.  ```c
    void init_lock(pthread_lock_t *lock) {
        rc = pthread_mutex_init(lock, NULL);
        assert(rc == 0); // handle error
    }
    ```

- See program which uses lock to solve race condition: [mutex-lock](./code/iii-lock.c)

## Conditional Variables

- These variables are important if there is relation between the threads, and some kind of signalling is involved within them.
- For example, if one thread needs to proceed only if the other thread will do it's work. Such as shared buffer, where one thread will produce something and one thread will consume something.
- To use these variables we need locks.
- See program for more clarity: [conditional-variable](./code/iv-conditional-variable.c)