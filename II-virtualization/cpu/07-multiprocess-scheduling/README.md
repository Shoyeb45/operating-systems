# Multiprocessor Scheduling

- A multi cpu hardware have multiple cpu cores on their processor. The difference comes when we see the caching in cpu.
- For faster access of data, cpu keeps frequently accessed data in their cache which are closest to the CPU and hence are very fast.
- Two type of cache locality:
    - **Temporal**: when a piece of data is access it's highly likely that it will be accessed again
    - **Spatial**: when program access data at address x, it's possible that it will access the data nearby of x as well
- Single Cpu has single cache. Now what happen in multi-cpu environment.

## Cache Coherence

- Earlier some process running on cpu1, writes the data to cache and then it updates, cache gets updated but updaing memory it takes a while and at the same time that process let say moved to cpu2. Now updating it will again try to access the same data and will fetch stale data.

## Synchronization Problem
- When there are multiple program accesses same data, there is high chance that the data can get corrupt. So we need some time of locking to preserve the data. 
- Locking ensures the correctness of the data, but sometimes it becomes bottleneck and it performs worse instead of being fast

## Cache Affinity
- A process which runs on a cpu, it saves state of the cpu in the cache. Now next time when the same process comes it will be benefecial to run the process in the same CPU, because the cache already have some state saved.
- Now in multiprocessor system, if the scheduler decides to run the process in different cpu, then we will lose the cache advantage.

## Single Queue Scheduling
- We'll keep only one queue and distribute the processes to the cpu based on their availability.
- It's simple to implement, but it has two main problems:
    - Scalability - to make this algorithm work in the multi processor system, we need to implement the locking. And frequently locking and unlocking will increase the cpu contention, hence creating the overhead
    - Cache Affinity - to solve this, some kind of affinity mechanism is needed which can distribute the load evenly

## Multi queue scheduling

- Each cpu will have own queue, and in each queue, we can use round-robin or similar algorithm
- MQMS is a scalable, because as number of cpu grows - so number of queues also grows.
- Now the main problem - **load imbalance**
    - One of the cpu can have few processes or the cpu can be empty but other cpu's can have the multiple processes. 
    - To solve this we need to migrate the processes in mid run and change balance the number of processes in CPU's

## Linux MultiProcessor Scheduler
- **O(1) Scheduler** — Fast O(1) priority-based scheduling; problem: complex heuristics and poor scalability/fairness in some workloads.
- **BFS** — Simple, low-latency scheduler with a single runqueue; problem: poor scalability on large multiprocessor systems.
- **CFS** — Fairly allocates CPU using vruntime + red-black tree; problem: scheduling overhead and difficulty optimizing for all workloads/scales.
