# Condition Variables

- Apart from the lock, we need some kind of way to tell send the signal based on some condition.

## Thread Join Problem

- Let say parent spawned one child thread, and parent wants to wait till child thread completes execuiton.
- One way is: [Inefficient Thread Join](./code/i-inefficient-thread-join.c)
- But in above approach, there is cpu wastage during spin lock. So this approach is inefficient.
- We need to make the thread sleep, when other thread is doing something.
- So for this we will use conditional variable.

## Producer/Consumer Problem

- So this is classic problem where on one shared buffer, one threads produces item and one thread consumes the item.
- Here, we need to be careful when to allow consumer to consume and producer to produce item.
- For better efficiency, we need to make threads sleep. When buffer is empty, consumer thread should sleep and when there is one item added to the buffer, then the consumer should be woken up by producer thread. Same if the buffer is full then producer thread should sleep, and it should woken when consumer consumes any item.
- Code: [consumer-producer](./code/iii-consumer-producer.c)