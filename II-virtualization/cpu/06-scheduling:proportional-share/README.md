# Scheduling: Proportional Share

- How can we share the cpu proportionally based on the jobs
- We can use randomness to give the share of the cpu to the jobs.

## Lottery Scheduling

- We'll assign a ticket to the processes, and then we will generate a random number between $0$ to $\sum tickets_i$
- Then based on the generated number, we will assign the task to the cpu.
- We'll maintain the counter and we'll select the process, where $counter > \text{generated-num}$, and we'll keep incrementing counter with the current ticket.
- How do we assign the ticket to the jobs? well I don't know

## Stride Scheduling

- We'll modify the lottery scheduling a bit.
- We calculate the stride value based on the process tickets, $stride=\frac{ticket}{constant}$, the $constant$ can be anything, for example 10000.
- Also, we'll keep a *pass* value for each process and increment it by it's stride value
- So, we choose a next process like this: choose the process which has lowest pass value, and then run it and increment it's pass value with it's stride value
- The problem with this scheduling policy is that it has a global state, and when new process comes in between the pass calculation then what should be the pass value of the new process.

## Linux Completely Fair Scheduler

- Instead of fix time sharing, in CFS we use dynamic time sharing, the CFS does by using **virtual_runtime**(vruntime).
- We pick the process with lowest **vruntime**, and then we run it, and increment the vruntime.
- Now, for when to stop the running process, CFS uses various parameters, 
    - **schedule latency**: Usually 48
    - **min granularity**: minimum time slice
- So we divide the *sched_latency* by number of processess, so the time sharing:
    $$\text{time\_sharing} = min(\frac{\text{sched\_latency}}{\text{num\_processes}}, \text{min\_granularity})$$
- This controls the too much time sharing or too less time sharing to avoid the too much context switches or efficiency of the algorithm

### Weighting (Niceness)

- In linux os, a user can change the scheduling priority of the process by assigning the *nice* parameter of the process.
- **nice** value ranges from -20 to 19, and more negative means more priority to process and more positive means less priority to process.
- CFS maps the nice value to these weights:
```c
static const int prio_to_weight[40] = {
    /* -20 */ 88761, 71755, 56483, 46273, 36291,
    /* -15 */ 29154, 23254, 18705, 14949, 11916,
    /* -10 */ 9548, 7620, 6100, 4904, 3906,
    /* -5 */ 3121, 2501, 1991, 1586, 1277,
    /* 0 */ 1024, 820, 655, 526, 423,
    /* 5 */ 335, 272, 215, 172, 137,
    /* 10 */ 110, 87, 70, 56, 45,
    /* 15 */ 36, 29, 23, 18, 15,
};
```
- And then time slice can be calcualte as:
$$\text{time\_slice}_k = \frac{weight_k}{\sum_{i=0}^{n - 1}{weight_i}} \cdot \text{sched\_latency}$$
- vruntime is also changed based on the following formula:

$$ vruntime_i = vruntime_i  + \frac{weight_0}{weight_i} \cdot runtime_i $$
- where $runtime_i$, is actual runtime that has occured till now for the process
- For quickly finding the process with lowest runtime, linux used red-black trees for logarithmic performance.
