set pagination off
file bitonic-bsort.riscv
target extended-remote localhost:3333
set remotetimeout 30000
load
b mt-main.c:34
c
thread 1
x/10dw 0x2050
# x/dw 0x2030
# x/dw 0x2040
# x/dw 0x2050
# x/dw 0x2060