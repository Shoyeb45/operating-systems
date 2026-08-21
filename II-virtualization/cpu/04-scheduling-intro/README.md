# Scheduling - Introduction

- For incoropating time sharing and fairness of cpu to process, we have to devise few OS policies, which describes how the processess will be executed on cpu.

## Workload
- The processes running in the system is called *workload*.

## Workload assumption
1. each job runs - same amount of time
2. arrival time of all job - same
3. each job is complete - means it will run to end
4. all job uses only cpu - no i/o
5. we know for how many seconds the job will run

    - We will relax these assumption one-by-one as we explore different algo's.

## Scheduling Metrics

- **Turnaround time**: Time taken to complete the job, in general sense:
$$T_{turnaround
} = T_{completion} - T_{arrival}$$

## Scheduling Algorithms

### 1. First In, First Out
- Execute the process which comes first.
- relax the assump-1, that is, now each process does not run for the same time.
- The problem with this algorithm occcurs when first process is of relatively much more higher job time. Then it goes through *convoy effect*.
- [FIFO Code](./codes/01-fifo.c)

> convoy effect - due to high avg turnaround time, a relatively lower time processes get queued because of the higher time processess.

### 2. Shortest Job First
- It's fifo but sort the jobs in the ascending order of their execution time and process the shortest one first.
- relax the second assumption that is not every job comes at the same time.
- [SJF Code](./codes/02-sjb.c)
- After relaxing assumption, SJF also goes through *convoy effect*.

### 3. Shortest Time To Completion First
- let's relax assumption-3, that is job must run to completion
- STCF is preemptive algorithm unlike SJF, so it's also called Preemptive Shortest Job First(PSJF).
- Everytime a new job enters it will check which of the two - a remaining currently running process or new process have shorter time and it will execute that
- [STCF Code](./codes/03-stcf.c)