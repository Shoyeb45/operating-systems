# Locks

- In concurrent programming, to execute series of operations togethre with multiple thread and ensure safety, we need mechanism to prevent on of the threads to stop while other thread is executing. We use something called locks to implement this mechanism.

## Features of lock

- **Mutual Exclusion**: Does lock prevents other threads to enter in critical section while some other thread is executing the critical section?
- **Fairness or preventing starving**: Does each lock get a chance to enter critical section? If any thread is waiting indefinitely, then it's not fair for that thread.
- **Performance**: So when there are mutliple threads, how does a waiting thread waits and how it affects a CPU?


## Different Types of locks:

### 1. Single flag variable lock: A failed attempt(loads and stores)

- We use single variable flag. And denote the lock state by 1(lock is not availble) and 0(lock is available).
- The code is here: [single-flag-variable-lock](./code/i-single-variable-lock.c)
- But this lock is not correct and it does not meet mutual exclusion principle.
- When two thread are executing simultaneously, then there is possiblility that two threads enters the lock, checks the flag, and set flag to 1 at the same time. Thus both threads acquires the lock and executes the same critical section.
- Another con is performance, when threads wait, it checks endlessly the flag and this is known as **spin-waiting**.

> Following locks uses hardware support to implement the lock:

### 2. Test and Set instruction

- We will use cpu's atomic exchange here to implement the lock. 
- The implementation is present in: [test-and-set](./code/ii-test-and-set.c)
- This lock does provide the correctness, even if two threads calls the lock at the same time. then cpu won't allow two threads to read the same vairable, it will only execute the atomic operation by one of the threads.
- But this approach can lead to starvation problem for some threads.
- Performance: So it have spin lock problem, it wastes cpu time for multiple threads when critical section is busy. For a single cpu, this lock can kill the performance.

### 3. Compare and swap

- Here we will use CAS instruction of cpu to implement our lock. [code](./code/iii-compare-and-swp.c)
- Here again the spin lock problem arises.

### 4. Load Linked and store conditional

- We will use load-linked and store-conditional instruction to implement this lock.
- Here is the sample code: [./load-linked and store-conditional]

### 5. Fetch and add

- Here we will atomically increment value and return the old value.
- This approach solves two starvation problem by assigning a unique number to each thrad and then providing the turn to them.
- So it provides fairness to all the threads.
- A simple implementation can be found here: [fetch-and-add](./code/v-fetch-and-add.c)

> Now let's also combine operating system.

- The hardware instructions were nice, but there is problem, what if in the critical section, the context switch happens and other threads start spinning endlessly.

### 6. Test And Set and Yield

- Here we use an operating system primitive `yield()` which a thread can call when it wants to give up the CPU and let another thread run.
- This will move thread from `running` to `ready` state.
- The code is here: [TAS-with-yield](./code/vi-test-and-set-and-yield.c)
- By voluntarily giving up, we let os to schedule another thread that might give up on lock.
- But in this approach we are letting cpu decide to prefer which thread next, this might leads to starvation if cpu makes some other choices.

### 7. Queue based lock

- We will use queue and two OS routines: `park()` and `unpark(thread_id)`.
- `park` will make calling thrad to sleep and `unpark` will wake up thread with the thread_id.
- This lock solves every problem, spin lock, mutual exclusion everything.
- We are using queue here, and for controlled access to the queue we are using guard to prevent the multiple threads to access the queue at the same time.
- Whenever lock is busy and thread wants to execute the critical section, it will add itself to queue and sleep. Thus preventing spin-lock.
- Now but there is one problem here, let's say one thread-T1 came and lock was not available, so it will add itself to the queue, then the thread(T2) which had lock, it called unpark with tid of T1, but T1 never went to sleep yet. So we will remove the T1's tid from the queue but after calling unpark(t1's-tid). Then T1 will go to sleep, and this will be the reason for the starvation or **wakeup/waiting race**.
- Here we solve this by using another system call `setpark()`, this routine allows thread to tell that it's about to be parked. And hence if any other thread calls unpark then it will return immediately
```c
queue_add(lock->q, gettid());
setpark(); // I am about to go to sleep
lock->guard = 0;
```

### 8. Futex Lock

- Futex = **Fast Userspace mutex**
- This locks uses user-space lock and kernel-sleep/wakeup call.
- Here we grab the lock in user space and if we can't then we go to kernel and sleep
- See the high level rought implementation: [futex-lock](./code/viii-futex-lock.c)