# Segmentation

- In the base and bounds, we assumed that the user address space will be contiguos, so everything logical space will live in a fixed range. But this might waster so much memory, so we need to fill the gap which are unused.

## Segmentation: Generalised base/bounds

- Instead of one pair of base/bounds, why not to have base/bounds per logical address space, i.e., stack, heap and code.
- So we can allocated only used memory in physical address, thus large amount of unused address(aka *sparse address space*) space can be accomodated.
- The segments can be identified by the address of the virtual space, the top 2 bits of 14 bit virtual address can tell us about the segment.
    - `0b00` - Code segment    
    - `0b01` - Heap segment    
    - `0b10` - Stack segment    
- The last 12 bits of the address space tells us the offset.
- Check the program: [address translation](./codes/addr-translation.c)
- By using top 2 bits, we limit the maximum size of the virtual address space. In our above example, it's 4kb
- **One Caveat**: The stack segment can grow backwards, so when we say stack starts at 28kb and has 2kb size, then we mean from 28 to 26kb, so for supporting this, the hardware needs to understand which segments grow negative or positive.
- So hardware will store a 1 for positive growth segments and 0 for negative growth segments

## Support for sharing

- For saving memory, the OS can share memory across different processors especially code segment can be shared. So, OS can reuse the same instruction from the code segment for different processors, and process will think it's their code segment, but no!!
- By doing so, the OS has to add few extra things to manage the privacy of the code segments, so it has to track which instructions are read only, write only or execute only for the process
- So if any process tries to do write on the read-only code, then OS should raise an exception level

## Fine Grained VS Coarse-Grained Segmentation

- Until now we have divided the physical memory into big segments for the process, these are known as coarse grained segment
- We can also use fine-grained, where the segments will be smaller and it will be divided into many small small parts. For this to support, OS has to manage *segment table* which maps the memory to translated between addresses.

## OS Support

- **context switch**: The segment registers must be saved and restored, the OS need to make sure that we are storing this for each process
- **growing memory**: A process may demand more memory, for an example process calls `malloc`, now here the heap needs to grow, so OS has to grow the heap and then return a pointer or if there is no physical memory available then it will reject the request
- After creating new address space, OS has to be able to find new segment, as each segment will have different size

> Now in this approach, the physical memory becomes full of little holes of free space. This is know as **external fragmentation**

- To solve this OS can compact the memory, i.e., for the running processes creating a new segment and copying data from the old to new, erasing the old segment, in this way we will have contiguos segments. But this is quite an expensive operation
