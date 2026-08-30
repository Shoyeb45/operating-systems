# Multi Level Feedback Queue

- Now, we will relax last two assumptions:
    - process can use both cpu and i/o
    - the runtime of process - not known

## The MLFQ algorithm:

1. if priority(A) > priority(B), then A runs B doesn't
2. if priority(A) == priority(B), then A and B will run in Round Robin
3. when job enters a system first, it will be placed at highest priority
4. priority change logic:
    - a. if a job uses entire time sharing of Round Robin, then the priority of the job will be reduced by 1
    - b. if job gives up cpu before completing it's time sharing, then it will stay in the same queue.

### Problems with current rules:

1. **starvation**: the long running process which will go down in queue because of their run-time, now if the small process keep coming, then those process will never get a chance to be executed, so the algorithm is not fairn for every process. Poor long running process :)
2. **game the scheduler**: A programmer can write a a program in such a way that after certain fixed time, it can issue I/O or give up cpu, in this way it will always stays in the same priority.    

So to solve this we introduce a new rule:

5. **boost processes**: after fixed time period S, we boost every process, by boosting we mean - move all the process to topmost queue again after S time

and reformulate rule -4:

4. we will reduce the priority of the job if it exhaust's the given time limit(regardless of how many times it has given up a cpu)
