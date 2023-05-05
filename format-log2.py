#!/bin/python

import re
import sys

path = sys.argv[1]

with open(path) as file:
    content = file.readlines()
    content_int = []
    for line in content:
        if re.search("(\d+ ){11}\d+\n", line):
            content_int.append(list(map(int, line.split())))

opmap = [
    ["PutFullData", "PutPartialData", "ArithmeticData", "LogicalData", "Get", "Intent", "AcquireBlock", "AcquirePerm"],
    ["PutFullData", "PutPartialData", "ArithmeticData", "LogicalData", "Get", "Intent", "ProbeBlock", "ProbePerm"],
    ["AccessAcc", "AccessAckData", "HintAck", "-", "ProbeAck", "ProbeAckData", "Release", "ReleaseData"],
    ["AccessAcc", "AccessAckData", "HintAck", "-", "Grant", "GrantData", "ReleaseAck", "-"],
    ["GrantAcc", "-", "-","-","-","-","-","-"]
]

out: str = '{:5s} {:3s} {:4s} {:6s} {:4s} {:8s} {:^15s} {:5s} {:^16s} {:4s} {:4s} {:7s} {:6s}\n'.format("tick", "dir", "chan", "source", "sink", "address", "operation", "param", "data", "size", "mask", "corrupt", "denied")
out += 99 * '-' + '\n'

                 #  tick   dir  chan  source   sink   add    op   param   data    size  mask  cor    denied
chan_ab_pattern = "{:5d} {:3s} {:^4s} {:^6d} {:^4s} {:^8x} {:15s} {:^5d} {:16x} {:^4d} {:4d} {:^7d} {:^6s}\n"
chan_c_pattern =  "{:5d} {:3s} {:^4s} {:^6d} {:^4s} {:^8x} {:15s} {:^5d} {:16x} {:^4d} {:^4s} {:^7d} {:^6s}\n"
chan_d_pattern =  "{:5d} {:3s} {:^4s} {:^6d} {:^4d} {:^8s} {:15s} {:^5d} {:16x} {:^4d} {:^4s} {:^7d} {:^6d}\n"
chan_e_pattern =  "{:5d} {:3s} {:^4s} {:^6s} {:^4d} {:^8s} {:15s} {:^5s} {:>16s} {:^4s} {:^4s} {:^7s} {:^6s}\n"


for message in content_int:
    channel_name =  chr(ord('A')+message[1])
    dir = "out" if channel_name == 'B' or channel_name == 'D' else "in"
    op_name = opmap[message[1]][message[7]]
    if channel_name == 'A' or channel_name == 'B':
        message_str = chan_ab_pattern.format(message[0], dir, channel_name, message[11], '-', message[2], op_name, message[8], message[4], message[10], message[6], message[3], '-')
        out += message_str
    
    elif channel_name == 'C':
        message_str = chan_c_pattern.format(message[0], dir, channel_name, message[11], '-', message[2], op_name, message[8], message[4], message[10], '-', message[3], '-')
        out += message_str

    elif channel_name == 'D':
        message_str = chan_d_pattern.format(message[0], dir, channel_name, message[11], message[9],'-', op_name, message[8], message[4], message[10], '-', message[3], message[5])
        out += message_str

    elif channel_name == 'E':
        message_str = chan_e_pattern.format(message[0], dir, channel_name, '-', message[9],'-', '-', '-', '-', '-', '-', '-', '-')
        out += message_str
    # elif channel_name == 'C':
    #     message_str

# print("%5s %3s %4s %15s %8s %8s %8s %8s %16s %8s %8s %8s %8s\n", 
#     "tick", "dir", "chan", "op", "address", "sink", "source", "param", "data", "size", "mask", "corrupt", "denied")
# print("-----------------------------------------------------------------------------------------------------------------------\n")
# for channelId, channel in enumerate(content_int):
#     if any(channel):
#         dir = "(out)" if channelId == 1 or channelId == 3 else "(in)"
#         out += '\nChannel {:s} {:5s}:\t'.format(chr(channelId + 97),dir)
#         for opcode, count in enumerate(channel):
#             if count:
#                 out += '{:>15s}: {:5d}'.format(opmap[channelId][opcode], count)

    # print("nmsg: %d\n", nmsg);

    # for (int i = 0; i<nmsg; i++) {
    #     reg_write32(BASE+0x08, i);
    #     uint64_t tick = reg_read64(BASE+0x10);
    #     uint32_t raw_channel = reg_read64(BASE+0x10+1*0x8);
    #     char channel = raw_channel+'A';
    #     uint64_t address = reg_read64(BASE+0x10+2*0x8);
    #     uint64_t corrupt = reg_read64(BASE+0x10+3*0x8);
    #     uint64_t data = reg_read64(BASE+0x10+4*0x8);
    #     uint64_t denied = reg_read64(BASE+0x10+5*0x8);
    #     uint64_t mask = reg_read64(BASE+0x10+6*0x8);
    #     char *op = opmap[raw_channel][reg_read64(BASE+0x10+7*0x8)];
    #     uint64_t param = reg_read64(BASE+0x10+8*0x8);
    #     uint64_t sink = reg_read64(BASE+0x10+9*0x8);
    #     uint64_t size = reg_read64(BASE+0x10+10*0x8);
    #     uint64_t source = reg_read64(BASE+0x10+11*0x8);

    #     char *dir = {channel == 'B' || channel == 'D' ? "out" : "in"};

    #     if(channel == 'A' || channel == 'B') {
    #         printf("%5lu %3s    %c %15s %8lx %8s %8lx %8lu %16lx %8lu %8lu %8lu %8s\n", 
    #             tick, dir, channel, op, address, "-", source, param, data, size, mask, corrupt, "-");
    #     } else if (channel == 'C') {
    #         printf("%5lu %3s    %c %15s %8lx %8s %8lx %8lu %16lx %8lu %8s %8lu %8s\n", 
    #             tick, dir, channel, op, address, "-", source, param, data, size, "-", corrupt, "-");
    #     } else if (channel == 'D') {
    #         printf("%5lu %3s    %c %15s %8s %8lx %8lx %8lu %16lx %8lu %8s %8lu %8lu\n", 
    #             tick, dir, channel, op, "-", sink, source, param, data, size, "-", corrupt, denied);
    #     } else if (channel == 'E') {
    #         printf("%5lu %3s    %c %15s %8s %8lx %8s %8s %16s %8s %8s %8s %8s\n", 
    #             tick, dir, channel, "-", "-", sink, "-", "-", "-", "-", "-", "-", "-");
    #     }
    # }



print(out)