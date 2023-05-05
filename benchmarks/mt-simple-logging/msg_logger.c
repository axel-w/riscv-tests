#include "msg_logger.h"

#include <stdio.h>
#include <string.h>
#include "mmio.h"

#define BASE 0x2000

void start_logger() {
    reg_write32(BASE + 0x00,1);
}

void stop_logger() {
    reg_write32(BASE + 0x04,1);
}

void print_log() {
    char *opmap[5][8] = {
        {"PutFullData", "PutPartialData", "ArithmeticData", "LogicalData", "Get", "Intent", "AcquireBlock", "AcquirePerm"},
        {"PutFullData", "PutPartialData", "ArithmeticData", "LogicalData", "Get", "Intent", "ProbeBlock", "ProbePerm"},
        {"AccessAcc", "AccessAckData", "HintAck", "-", "ProbeAck", "ProbeAckData", "Release", "ReleaseData"},
        {"AccessAcc", "AccessAckData", "HintAck", "-", "Grant", "GrantData", "ReleaseAck", "-"},
        {"GrantAcc", "-", "-","-","-","-","-","-"}
    };

    int nmsg = reg_read32(BASE+0xc);

    printf("%5s %3s %4s %15s %8s %8s %8s %8s %16s %8s %8s %8s %8s\n", 
        "tick", "dir", "chan", "op", "address", "sink", "source", "param", "data", "size", "mask", "corrupt", "denied");
    printf("-----------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i<nmsg; i++) {
        reg_write32(BASE+0x08, i);
        uint64_t tick = reg_read64(BASE+0x10);
        uint32_t raw_channel = reg_read64(BASE+0x10+1*0x8);
        char channel = raw_channel+'A';
        uint64_t address = reg_read64(BASE+0x10+2*0x8);
        uint64_t corrupt = reg_read64(BASE+0x10+3*0x8);
        uint64_t data = reg_read64(BASE+0x10+4*0x8);
        uint64_t denied = reg_read64(BASE+0x10+5*0x8);
        uint64_t mask = reg_read64(BASE+0x10+6*0x8);
        char *op = opmap[raw_channel][reg_read64(BASE+0x10+7*0x8)];
        uint64_t param = reg_read64(BASE+0x10+8*0x8);
        uint64_t sink = reg_read64(BASE+0x10+9*0x8);
        uint64_t size = reg_read64(BASE+0x10+10*0x8);
        uint64_t source = reg_read64(BASE+0x10+11*0x8);

        char *dir = {channel == 'B' || channel == 'D' ? "out" : "in"};

        if(channel == 'A' || channel == 'B') {
            printf("%5lu %3s    %c %15s %8lx %8s %8lx %8lu %16lx %8lu %8lu %8lu %8s\n", 
                tick, dir, channel, op, address, "-", source, param, data, size, mask, corrupt, "-");
        } else if (channel == 'C') {
            printf("%5lu %3s    %c %15s %8lx %8s %8lx %8lu %16lx %8lu %8s %8lu %8s\n", 
                tick, dir, channel, op, address, "-", source, param, data, size, "-", corrupt, "-");
        } else if (channel == 'D') {
            printf("%5lu %3s    %c %15s %8s %8lx %8lx %8lu %16lx %8lu %8s %8lu %8lu\n", 
                tick, dir, channel, op, "-", sink, source, param, data, size, "-", corrupt, denied);
        } else if (channel == 'E') {
            printf("%5lu %3s    %c %15s %8s %8lx %8s %8s %16s %8s %8s %8s %8s\n", 
                tick, dir, channel, "-", "-", sink, "-", "-", "-", "-", "-", "-", "-");
        }
    }
}
