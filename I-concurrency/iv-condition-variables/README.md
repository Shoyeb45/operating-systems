# Condition Variables

- Apart from the lock, we need some kind of way to tell send the signal based on some condition.

## Thread Join Problem

- Let say parent spawned one child thread, and parent wants to wait till child thread completes execuiton.
- One way is: [Inefficient Thread Join](./code/i-inefficient-thread-join.c)
- But in above approach, there is cpu wastage during spin lock. So this approach is inefficient.
- We need to make the thread sleep, when other thread is doing something.
- So for this we will use conditional variable.
- 