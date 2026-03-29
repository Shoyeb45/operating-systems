# Concurrent Linked List

- Linked list is data structure which holds the value in linear fashion.
- Simple implementation, single lock: [single-lock-linked-list](./simple-linked-list/simple-linked-list.c)


## Performance

### Single lock linked list

| Threads | Total Ops | Time (sec) | Throughput (ops/sec) |
|--------:|----------:|-----------:|---------------------:|
| 1       | 100,000   | 0.004034   | 24,789,290.95        |
| 2       | 200,000   | 0.017486   | 11,437,721.61        |
| 4       | 400,000   | 0.034435   | 11,616,088.28        |
| 8       | 800,000   | 0.076900   | 10,403,120.93        |
| 16      | 1,600,000 | 0.148572   | 10,769,189.35        |
| 32      | 3,200,000 | 0.283416   | 11,290,823.38        |
| 64      | 6,400,000 | 0.556791   | 11,494,438.67        |
| 128     | 12,800,000| 1,111355   | 11,517,471.91        |

---

<p>
    <img src="./image/linked-list-perfo-1.png">
</p>