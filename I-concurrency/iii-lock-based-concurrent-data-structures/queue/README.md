# Concurrent Queue

- Queue are data structure that follows FIFO(First-In-First-Out) principle, that is first value entered will be shown first.
- There is one way to implement the queue using one single global lock. We will use linked list to implememt the queue.
- We will implement the queue designed by Michael and Scott.
- The implementation is located in: [michael-scott-concurrent-queue](./concurrent-queue.c)
- The dummy node is used to perform head and tail operation separately.