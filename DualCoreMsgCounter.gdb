set pagination off
file mt-tacle.riscv
target extended-remote localhost:3333
set remotetimeout 30000
load
b mt-tacle_main.c:224
c
thread apply 1 c
thread 1
set {int}0x2008 = 0
x/8dw 0x200c
set {int}0x2008 = 1
x/8dw 0x200c
set {int}0x2008 = 2
x/8dw 0x200c
set {int}0x2008 = 3
x/8dw 0x200c
set {int}0x2008 = 4
x/8dw 0x200c