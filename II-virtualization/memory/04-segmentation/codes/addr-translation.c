#include <stdio.h>
#include <stdlib.h>

// We are not assuming negative and positive growth

int main() {
    const int SEG_MASK = 0x3000; // 1100000000000
    const int SEG_SHIFT = 12; 
    const int OFFSET_MASK = 0xfff; // 00111111111111

    printf("Base address space and sizes\n\n");
    printf("Code Segment:\nBase: 32k, Size: 2k\n\n");
    printf("Heap Segment:\nBase: 34k, Size: 2k\n\n");
    printf("Stack Segment:\nBase: 28k, Size: 2k\n\n");

    printf("Enter Virtual Address(in bytes): \n");
    int virtual_address;
    scanf("%d", &virtual_address);

    int base[3] = {32 * 1024, 34 * 1024, 28 * 1024};
    int bounds[3];
    for (int i = 0; i < 3; i++) {
        bounds[i] = base[i] + 2000;
    }

    int segment = (virtual_address & SEG_MASK) >> SEG_SHIFT;
    int offset = (virtual_address & OFFSET_MASK);
    printf("Segment: %d\n", segment);
    printf("Offset: %d\n", offset);

    if (offset >= bounds[segment]) {
        printf("Virtual address out of bound\n");
        exit(1);
    } else {
        int physical_address = base[segment] + offset;
        printf("Physical address: %d\n", physical_address);
    }
    return 0;
}