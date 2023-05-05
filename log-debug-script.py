import gdb  # pyright: reportMissingModuleSource=false
import os
import re
import time

# riscv: str
# source: str
# breakpoint1: str
# log_file: str
# er_localhost: str

riscv = "/home/axel/riscv-tests/benchmarks/mt-tacle-logging.riscv"
source = "/home/axel/riscv-tests/benchmarks/mt-tacle-logging/mt-tacle_main.c"
breakpoint1 = "226"
log_file = "/home/axel/riscv-tests/debug-logs-tacle.txt"
er_localhost = "3333"

output_reg = "x/8dw 0x200c"

dir_path = os.path.dirname(source)
bench_name = os.path.basename(dir_path)

nmsg_reg_read = "x/dw 0x200c"

def set_out(i: int):
    return "set {int}0x2008 = " + str(i)

def extract_ints(command):
    raw = gdb.execute(command, from_tty=False, to_string=True)
    clean = [int(s) for s in re.findall(r'(?<=\t)\d+', raw)]
    return clean
    
def get_nmsg():
    nmsg_reg = "0x200c"
    read_int = "x/dw"
    read_nmsg_command = read_int + ' ' + nmsg_reg
    nmsg = extract_ints(read_nmsg_command)[0]
    return nmsg

def get_message(messageId):
    message_regs = "0x2010"
    read_11int = "x/12dg"
    read_msg_command = read_11int + ' ' + message_regs
    gdb.execute(set_out(messageId))
    message = extract_ints(read_msg_command)
    return message
    

# def get_channel_log(channelId):
#     gdb.execute(set_out(channelId))
#     channel_raw = gdb.execute(output_reg, from_tty=False, to_string=True)
#     channel_clean = [int(s) for s in re.findall(r'(?<=\t)\d+', channel_raw)]
#     return channel_clean

def write_logs(logstring):
    with open(log_file, 'w') as f:
        f.write(logstring)
        f.close()

gdb.execute("dir $cwd/../common")
gdb.execute("file " + riscv)
gdb.execute("set remotetimeout 30000")

sleep_period=30
for attempt in range(1, 10000):
    try:
        gdb.execute("target extended-remote localhost: " + er_localhost)
    except:
        print("Attempt " + str(attempt) + " for " + bench_name + " failed. Retrying in " + str(sleep_period) + " sec ...")
        time.sleep(sleep_period)
        if sleep_period < 600: sleep_period+=30
        continue
    break

print("Successfully connected gdb with " + bench_name + " to openocd")
# gdb.execute("monitor [target current] configure -event gdb-detach {shutdown}")
gdb.execute("load")
sourcebreakpoint = source + ':' + breakpoint1
gdb.execute("b " + sourcebreakpoint)
gdb.execute("c")

messages:str = str()

# get nmsg
nmsg = get_nmsg()
print("Total number of messages: %d" %nmsg)

for messageId in range(nmsg):
    message = get_message(messageId)
    message_str = ' '.join(list(map(str, message))) + '\n'
    messages += message_str
    print(message_str)

write_logs(messages)
gdb.execute("detach inferior 1")
gdb.execute("monitor shutdown")
gdb.execute("quit")