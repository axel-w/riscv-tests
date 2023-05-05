#include "msg_counter.h"

#include <stdio.h>
#include "mmio.h"

#define BASE 0x2000

void start_counter() {
    reg_write32(BASE + 0x00,1);
}

void stop_counter() {
    reg_write32(BASE + 0x04,1);
}

void print_counts() {
    for(int i = 0; i<5; i++) {
        reg_write32(BASE+0x8,i);
        for(int j = 0; j<8; j++) {
            printf("%u ", reg_read32(BASE + 0xc + j * 0x4));
        }
        printf("\n");
    }
}
