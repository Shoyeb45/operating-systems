# Paging: Faster Translation TLB
- Translating address only using Page Table is slow, as we need to lookup a memory many times.
- To speed this up OS needs help from the hardware.
- To speed up the translation, the hardware is going to provide **Translation-Lookaside Buffer**, which comes with MMU and it's a cache to translate between virtual to physical address.

## TLB Basic Algorithm

- The functionality of the translation stays the same. We extract the VPN from the virtual address.
- Now earlier we used to find the page table and then find the mapping between VPN and PFN.
- But now before that first the hardware will check in the TLB. And if found then it's a **TLB hit**.
- The tlb contains the address translation and then we just need to add the offset and we found the address without ever referencing the Page Table

## Handling TLB Miss

### Hardware Handles TLB Miss
- In older cpu's, the hardware didn't trust OS much, so they had a complex instruction to handle the TLB miss. Which is called **CISC(Complex Instruction Set Computers)**.
To look up for the page table, it have special register called **Page Table Base Register**, which helps to find the location of the Page Table.

### Software Handles TLB Miss

- In modern architecture, we have **Software Managed TLB**. 
- On TLB miss they simply raise the exception.(`RaiseException(TLBMiss)`)
- The exception pauses the current instruction execution, raise the privillege to kernel mode and jumps to trap handler.
- In trap handler, the code is written to manage handle the TLBMiss, and through the special instruction we update the TLB and then return from the trap. And while looking again in TLB it finds the address in TLB table, and hence success.
- This return-from-trap should be little different, as it have to resume the execution of the instructions from where it raised the exception.
- Also while running the TLB miss code, the OS has to extra careful not to cause infinite loop of exceptions

## TLB Contents

- A TLB entry might look like this - 
    ```c
            VPN | PFN | Other bits
    ```
- In hardware terms, the TLB is know as *fully associative* bits.
- The other bits:
    - **Valid Bit**: This says if the entry has a valid translation or not
    - **Protection bit**: How page can be accessed, for example a code segment page might only have *read and execute* permission but heap can have *read and write* permission

## TLB Issues

### 1. Context Switches

- As the TLB's are useful for only the currently running process, as it contains the address translation for the running process. In context switch, the hardware and OS must have to be carefull not to mismatch the TLBs of the processes
- For handling the context switch the OS and hardware must have something to distinguish between processes, approaches -

#### Flush the TLB
- Flush the TLB on the context switch, thus emptying it before running a new process.
- So with the help of the hardware and using privileged instructions, we can flush the TLB.
- The downside is that, the process will have many TLB miss initially, and if there are process which are switching often, then the performance can be really slow.
- To solve the performance issue many hardware adds a **Address Space Identifier** to identify the entry of the process.
- So we can accomodate the entries for the different processes without flushing the TLB.


### 2. Replacement Policy
- When we add new entry to the TLB, we need to remove an existing TLB entry to make the space for new one, so which one to remove such that TLB miss can be minimizwed and TLB hit can be increased?
- One of the policy is to remove the Least recently used (i.e., LRU).
- Another is simple, just randomly remove any one of the entry.

