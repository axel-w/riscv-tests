#!/bin/python

import re
import sys

path = sys.argv[1]

with open(path) as file:
    content = file.readlines()
    content_int = []
    for line in content:
        if re.search("(\d+ ){8}\n", line):
            content_int.append(list(map(int, line.split())))

opmap = [
    ["PutFullData", "PutPartialData", "ArithmeticData", "LogicalData", "Get", "Intent", "AcquireBlock", "AcquirePerm"],
    ["PutFullData", "PutPartialData", "ArithmeticData", "LogicalData", "Get", "Intent", "ProbeBlock", "ProbePerm"],
    ["AccessAcc", "AccessAckData", "HintAck", "-", "ProbeAck", "ProbeAckData", "Release", "ReleaseData"],
    ["AccessAcc", "AccessAckData", "HintAck", "-", "Grant", "GrantData", "ReleaseAck", "-"],
    ["GrantAcc", "-", "-","-","-","-","-","-"]
]

out: str = "operations count:"
for channelId, channel in enumerate(content_int):
    if any(channel):
        dir = "(out)" if channelId == 1 or channelId == 3 else "(in)"
        out += '\nChannel {:s} {:5s}:\t'.format(chr(channelId + 97),dir)
        for opcode, count in enumerate(channel):
            if count:
                out += '{:>15s}: {:5d}'.format(opmap[channelId][opcode], count)
print(out)