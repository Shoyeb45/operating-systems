# The CPU Abstraction: Process

- The process is main abstraction of CPU. It is running program in the operating system. 
- The executable file running in the cpu is called the process.
- There are various process API's provided by OS, like creating the process, cloning a process, deleting a process, or getting status of process.
- The process has few status, like 
    - READY: The process is ready to run but not yet running for some reason
    - RUNNING: The process is running in CPU
    - BLOCKED: Process is waiting for some external input (for eg., reading from file, some network request, or user input). So it's waiting for I/O. 
- So whene process is idle, the CPU can prioritize other processess while the current process is waiting for input.
- For storing all the processes, OS uses **process list**(a list of processes). Each entry contains many information about the process, for eg., process_id, parent_process, codes, status,... etc. Each entry is called Process **Control Block**.
