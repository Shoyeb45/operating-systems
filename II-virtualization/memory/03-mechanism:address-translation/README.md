# Mechanism: Address Translation

- How do we efficiently provide memory abstraction, while giving the necessary access of the memory to the process but retaining the control over memory? How do we do this safely, so that process A won't be able to access process B's memory?

## Hardware Based Address Translation or Address Translation

- Every **virtual** address provided by the instruction gets translated to the **physical** (real) address by the hardware. So any reference by the application process gets converted into the **physical** address by the hardware. 
- OS has job to manage the memory, as hardware alone won't be able to keep track of everything. Hardware just provides a mechanism to tranlsate the address


### Few Assumptions
- user address space <= physical address space   
- user address space is continguos
- each address space is of same size

## Base And Bound or Dynamic Relocaion
- The process have illusion that it has entire memory starting from 0.
- For each cpu, the memory will have to define two values in the register - `base` and `bound`
    - `base`: The actual starting memory address of the user program
    - `bounds`: The actual ending memory address of the user program
- How hardware translates the address:
    - `physical_address` = `base_address` + `virtual_address`
- Any virtual address which is negativce or $\gt$ `bound` will raise a cpu execution

## Hardware Support Needed
- A single bit *process status bit*, which tells us in which mode the cpu is running - **user mode** or **kernel mode**.
- A register to store *base and bound* values - these are the part of the **Memory Management Unit** (MMU) of the cpu
- Also a hardware should provide a logical circuitory to check the base and bounds of the address
- It should also provide a mechanis to change the *nbase and bound values*, by increasing the privilege of the running process without wreaking havoc on the other processes

## OS issues

- OS should maintain a *free list* of the available memory spaces, also os should handle when the process grafully exited or terminated
- On context switch of the processes, the os must save the base and bounds value of that process in some register to continue executing the process (in Process Control Bloc or Process Structure)
- When process is not runnning, the OS must deschedule the process and copy the address spaces into new address space   
- OS must provide exceptional handler, it should be installed in boot time in the trap table - if the process tries to access the memory which is out of bounds, then the OS must take action in this exception, maybe by killing the process
