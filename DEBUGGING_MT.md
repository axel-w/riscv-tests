# Debugging

**Shell 1:**

source: https://chipyard.readthedocs.io/en/latest/Advanced-Concepts/Chip-Communication.html#debugging-with-jtag

    $ source /ssd/wiedemax/chipyard/env.sh
    $ cd /ssd/wiedemax/chipyard/sims/verilator/
    $ make SUB_PROJECT=customtop CONFIG=MsgCounter2BigConfig BINARY=none SIM_FLAGS="+jtag_rbb_enable=1 --rbb-port=9823" run-binary

**Shell 2:**

source: https://github.com/chipsalliance/rocket-chip#4-launch-openocd

    $ source /ssd/wiedemax/chipyard/env.sh
    $ openocd -f /ssd/wiedemax/riscv-tests/cemulator.cfg 

**Shell 3:**

source: https://github.com/chipsalliance/rocket-chip#5-launch-gdb

    $ source /ssd/wiedemax/chipyard/env.sh
    $ cd /ssd/wiedemax/riscv-tests/benchmarks/
    $ riscv64-unknown-elf-gdb -ex "set remotetimeout 30000" -ex "target extended-remote localhost: 3333" -ex load -f mt-tacle.riscv