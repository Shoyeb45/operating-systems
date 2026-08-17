# Limited Direct Execution

- What if we let any program to run as they like. There are couple of problems, if we directly give the control of the cpu to the executable programs:
    - security, the program can do anything
    - excessive resource utilization
    - not giving other processes to run the program
- So, here the OS has to give limited to control of the CPU to the program while allowing instruction to be executed by cpu. To do so, both hardware and operating system have to work together.
- First idea is simple, run the program directly on the CPU, i.e., called Limited Direct Execution. But as mentioned above, it has problems. Problems in detail:

## 1. Restricted Operations
- If we run the program directly, it can do privilleged operation, that user programs shouldn't do, like reading some protected data from system folder. These operation should be performed in the kernel mode.
- Now to solve this we introduce, *user mode* -- so in this mode all the programs will run.
- Now for performing the kernel operations, user mode can call system call through special **trap** instructions. These instructions are special and are defined while booting the OS in **trap table**. Using these instructions the privilege is raised from user to kernel mode.
- After finishing the system call, OS will call *return-from-trap* instructions which will again lower the privilege to the user mode.

## 2. Switching between processes
- To enable the **time sharing** between processes, the OS must switch between processes when one process is waiting for I/O, then it can give control to other process to run it while it's waiting.
- The switching between the process is called *context switch*.
- A special program called *scheduler* will decide when to switch to which process.
- There is one more way hardware mechanics, using *timer interuppt* the OS will automatically halth the process. This is non-cooperative.