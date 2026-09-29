# Memory API

## Types of memory:

1. Stack Memory - Short lived memory, all the local variable of the function reside in the stack, aka automatic memory, as after the function call is over, the memory will be automatically freed
2. Heap memory - Long lived memory, dynamically defined memory using calloc or malloc in c, or using new keyword. In c/c++, the programmer has to manage the memory by freeing it after allocating some memory.

## Commom Errors

1. Forgetting to allocate memory - trying to use some memory location and allocating it with something, without initializing the memory.
2. Not allocating enough memory - allocating less memory then required, it will be buffer overflow
3. Memory allocated, didn't initialize - malloc will allocate memory but it will contain garbage data, so initialize the correct values, or use calloc, which will initialize with default values
4. Forgetting to free memory - allocated memory, but didn't free after using it, it will cause memory leak, and in long running program it will bloat the hardware's memory
5. use after free - using the memory which was already free
6. double free - We free memory two times, causing undefined behaviour
7. invalid frees - free expects a pointer, passing something else will cause undefined behaviour
