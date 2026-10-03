# Free Space Management

- In C, we can use `malloc` to allocate the contiguos memory and return the pointer to the allocated memory. If we write `malloc(12)` - then it will allocate 12 bytes, no type. S
- **Internal fragmentation**: Internal libraries for managing the memory will sometimes allocate memory more than asked, so the extra memory will cause internal fragmentation

## Assumptions

- We are only concerned about external fragmentation
- When malloc allocated memory and gives to the user, then it cannot take back that memory until these memory are freed by the user. No compaction of free space is possible
- Allocator manages continuos space of bytes while allocating the memory


## Low Level Mechanims
- For maintaining what part of heap is free, allocator need to maintain a free list. Which tells which part is free in the heap.
- Let say, we have heap of 30 bytes - 

```js
0         10        20        30
|---------|---------|---------|
   FREE       USED       FREE
  10 bytes  10 bytes  10 bytes
```
- The free list will look like-
```js
head
 ↓
+-----------+      +-----------+
| addr = 0  | ---> | addr = 20 |
| len  = 10 |      | len  = 10 |
+-----------+      +-----------+
                       ↓
                      NULL
```

### Splitting 

- Now let say user requested for 1 byte of memory in heap. Then allocator will try to find the 1 byte, and it will find that we have 10 bytes free at address 20. Then it will split the 10 bytes at address 10 and then assign 1 byte to user program. This is **splitting**
- So heap will look like-
```cpp
0         10  20 21         30
|---------|---|--|-----------|
   FREE       USED   FREE
   10B        1B      9B
```
- But if user asks for `malloc(20)`, that is 20 bytes, the allocator will not be able to give this much space, as we don't have contiguos space.

### Coalescing

- Now let say user free the 10 bytes using `free(ptr)`, where `ptr` is the potiner pointing to that 10 bytes of memory. Now the allocator will free the memory and it will have to add a node to the free list.
- So if allocator do something like this -
```cpp
[addr=10, len=10]
       ↓
[addr=0, len=10]
       ↓
[addr=20, len=10]
``` 
- Then allocator won't be able to assign 30 bytes of memory, as it will think that we don't have 30 continuogous 30 bytes.
- So this is the problem, and for solving this it will check the nearby ranges and check if we can merge them. So here address - `0-9`, `10-19` and `20-29` can be merged. This is called **coealescing**
- So after coealescing, the free list will look like- 
```cpp
+-----------+
| addr = 0  | ---> NULL
| len  = 30 |
+-----------+  
```

### Tracking size of the allocated regions

- Allocators also store a extra data to track the allocated memory on heap. 
```c
void *ptr = malloc(20);
free(ptr);
```
- So here in free, we don't pass any size to deallocate. So `malloc` stores a header before the assigned `ptr`. So it essentially looks like - 
```cpp
malloc(20)
    ↓

+------------------+----------------------+
| Header           | User allocation      |
| size = 20        | 20 bytes             |
|                  |                      |
+------------------+----------------------+
                   ↑
                   ptr
```
- Then `free` will use this header to free the memory. So it will free more memory than assigned, N(user assigned) + header_size


### Embedding a free list 
- The allocator has to track the memory provided by the OS, to track the memory we use free list. Now but free list has to live somewhere, so it won't in different memory. The free list will be stored in the same memory that OS has provided for the user program. 
- So let say for one program the heap memory provided by OS is 4096 bytes, then the free list use that 4096 bytes that was provided by the OS. The actual available memory for the user program will be `4096 - sizeof(freelist)`.
- So if the node of free list is -
```c
struct __node_t {
       int              size;
       struct __node_t *next;
} node_t;
```
- So currently the memory is empty, so the free list has to represent that. So it will store it's first node in same 4kb of space. 
- So the first few bytes will be metadata of the free space itself.
- Let say user requested for 100 bytes of memory, then the allocator will allocate the memory of the user requested space + header space and return a pointer to the requested space memory to the user. 
- We also have to move the free node as the space changed.
- If user allocated three times a 100 bytes, then -
```js
16384
 ↓

┌────────┬────────────┬────────┬────────────┬────────┬────────────┬───────────────┐
│ header │ 100 bytes  │ header │ 100 bytes  │ header │ 100 bytes  │ free region   │
│  8 B   │            │  8 B   │            │  8 B   │            │   3764 B      │
└────────┴────────────┴────────┴────────────┴────────┴────────────┴───────────────┘
                                                                    ↑
                                                                    │
                                                                   head
```

- Now when user free any memory part, then after freeing we allocate the free node to the same space. So in above example if user frees 2nd allocated part, then we'll have to add a free list describing that physical memory.
- And if we just leave that after freeing the memory, we'll have problem of external fragmentation. So we need to coealasce the remaining nodes by checking if the free list contains the adjacent memmory addresses.

## Growing The heap
- When memory provided by the OS fills up then allocator requests more memory from OS to grow the heap. Most allocator start with small memory and they request when it fills up.
- To effectively manage the memory, there are some strategies that allocator uses:

### Basic approaches

|Strategy|What id does?| Cons|
|---|---|---|
|Best Fit  | It will find the smallest available memory for the requested memory from user| Exhaustive searching in free list, leaves too many small memory spaces|
|Worst Fit| It will find the largest available memory and then split that memory, and it will keep the larger chunk|Exhaustive search, more memory fragmentation|
|First Fit| It will find the first free memory which is greater than requested memory, it's fast | pollutes the beginning of the free list with small objects|
|Next Fit| It's first fit, but it remembers the last position which was matched earlier and starts search from that position| avoids polluting the start of the free list, spreads the search|

- Example, suppose we have free list-
```c
head -> 10 -> 30 -> 20 -> NULL
```
- Requested memory: 15
  - Best fit: it will choose 20
  - Worst fit: It will choose 30
  - First Fit:  It will also choose 30
  - Next Fit:  It will also choose 30


### Other Approaches

#### 1. Segregated Lists
- Manage two allocators, a one for generalised purpose and one to manage the popular-sized object of the memory by that program.
- Pros: less fragmentation, allocation and free can be served quickly
- Problem: How much sized memory pool should we allocate for these fixed sized data.
- Kernel allocates some `cached object` while boot time. They are frequently used by the kernel, such as file syste, locks, inodes etc.
- Thus, these caches are the segregated free list of the object caches and they server requests quickly.
- When the reference count to the obejct goes to 0, then a general allocator may reclaim these memory from specialized allocator, which often needs whem VM system needs more memory. 


#### 2. Buddy Allocation (aka Binary Buddy Allocation)
- This appraoch tries to make the coalescing simple.
- So it will represent the memory in the $2^N$ form.
- When requested for memory, the search will happen from the big chunk and it will divide it in half and find the memory that we can accomodate.
- For example, let's say we have 16 bytes of the memory, and we need to allocate for the 7 bytes, then it will divide the memory like- 
```c
                    16 bytes
                 [0 -------- 15]
                  /           \
                 /             \
          8 bytes               8 bytes
        [0---7]                 [8---15]
          / \                     / \
         /   \                   /   \
       4B     4B               4B     4B
      0-3    4-7              8-11   12-15
```
- Then it will allocate the memory to the last block. 
- One problem is that it can have internal fragmentation as we are dividing in $2^n$ memory
- To free the memory, it will check for the other half of the memory ('buddy') and see if it's free then it will coealesce both memory.
- Now to find the other half, we just need to check the address bit of the memory that is being freed and the size of the block. As the starting address of the each block will differ by the one bit.
- So finding the buddy address -
```c
buddy_address = address ^ size_of_the_block,
Where, ^ is XOR operator
```
