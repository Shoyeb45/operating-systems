#include <stdio.h>

int main() {
    const int SHIFT = 4;
    const int VPN_MASK = 0b110000;
    const int OFFSET_MASK = 0b1111;
    const int page_table[] = {3, 7, 5, 2};
    
    printf("Enter Virtual Address: ");

    int virtual_address;
    scanf("%d", &virtual_address);

    if (virtual_address >= 64) {
        printf("Invalid address, please enter address below 64 bytes\n");
        return -1;
    }

    int vpn = (virtual_address & VPN_MASK) >> SHIFT;
    printf("VPN: %d\n", vpn);

    int pfn = page_table[vpn];
    printf("PFN: %d\n", pfn);

    int offset = virtual_address & OFFSET_MASK;
    printf("Offset: %d\n", offset);

    int physical_address = (pfn << SHIFT) | offset;
    printf("Physical Address: %d\n", physical_address);

    return 0;
}