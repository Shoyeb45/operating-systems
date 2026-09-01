# The Abstraction: Address Space

- Now we need to virtualize the memory of computer in efficient, transparent and secure way. We need to virtualize in such a way that each process get fair amount of memory, no other process can look into different process' memory.
- To do so, the OS will divide it's memory into the segments, one particular segment belongs to a particular process. The process will think that it has entire memory, it doesn't know that the OS is giving a part of the memory which is called address space of the process.
- In the address space - the code, data, stack and heap lives.
- The memory referenced by a process is called a virtual memory, the OS will convert that memory into physical address by using hardware mechanism.
- Doing so will let OS virualize memory in :
    - efficient
    - transparent, and
    - secure manner

