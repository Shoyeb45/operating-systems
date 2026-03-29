# Counter

- Here is the simple implementation: [simple-counter-impl](./counter/counter-with-lock.c)
- Here is the more scalable version: [scalable-version(approximate-counter)](./counter/counter-scalable);
- There is some overhead of the locks. More simple the code the better. But for larger number of threads we need scalable version.
- Here's the rough benchmark on Apple Mac M2, 8 Gb Ram

### Non-Scalable counter:

| Threads | Final Counter | Time (s) | Throughput (ops/sec) |
| ------- | ------------- | -------- | -------------------- |
| 2       | 2,000,000     | 0.026629 | **75,106,087.35**    |
| 4       | 4,000,000     | 0.088190 | **45,356,616.39**    |
| 8       | 8,000,000     | 0.225269 | **35,513,097.68**    |
| 16      | 16,000,000    | 0.547032 | 29,248,745.96        |
| 32      | 32,000,000    | 0.910503 | 35,145,408.64        |
| 64      | 64,000,000    | 1.769073 | 36,177,139.10        |
| 128     | 128,000,000   | 3.545331 | 36,103,822.18        |

### Scalable counter:

| Threads | Final Counter | Time (s) | Throughput (ops/sec) |
| ------- | ------------- | -------- | -------------------- |
| 2       | 2,000,000     | 0.040212 | 49,736,397.10        |
| 4       | 4,000,000     | 0.150828 | 26,520,274.75        |
| 8       | 8,000,000     | 0.329498 | 24,279,358.30        |
| 16      | 16,000,000    | 0.270514 | 59,146,661.54        |
| 32      | 32,000,000    | 0.824363 | 38,817,850.87        |
| 64      | 64,000,000    | 0.743424 | 86,088,154.27        |
| 128     | 128,000,000   | 0.821350 | **155,840,993.48**   |

### Reason of poor performance of the simple counter:

1. Lock Contention - Threads spend most time waiting
2. Context Switching Overhead - Suspending the thread and then waking up
3. Cache Line Bouncing 
    - Our `counter.value` is shared.

    - Each increment:

    - Core A modifies it → cache line moves to A
    - Core B wants it → cache line moves to B
    - Core C wants it → moves again

    This is called:
    > cache coherence traffic (MESI protocol)

> But whereas our scalable counter performs good and scales because we are keeping local count, different cache locations and less lock contention.