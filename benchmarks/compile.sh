# source=$(realpath $source)
set -x
source="/home/axel/riscv-tests/benchmarks/mt-tacle/mt-tacle_main.c"
dir=$(dirname $source)
scripts=$(dirname $(realpath "$0"))

linking=$(realpath "$scripts/common/")

GCC=riscv64-unknown-elf-gcc
CFLAGS=" -DPREALLOCATE=1 -mcmodel=medany -static -std=gnu99 -g -O0 -fno-builtin-printf"
LDLFLAGS="-static -nostdlib -T ./common/test.ld"
INCLUDES="-I./../env -I./common -I./mt-tacle"
# LDLFLAGS+="$linking/syscalls.c $linking/crt.S -nostartfiles -nostdlib -T $linking/test.ld"
C_FILES=$(grep -rLE --include "*.c" "void thread_entry\(int cid, int nc\)" $dir)
C_FILES2="$scripts/common/*.c"
C_FILES3="$scripts/common/*.S"

# echo $C_FILES
# echo $C_FILES2
# echo $C_FILES3

$GCC $INCLUDES $CFLAGS -o "test-mt-tacle.riscv" $LDLFLAGS $C_FILES $C_FILES2  $C_FILES3 $source #-o $target




# if $monitor; then
#     tests=$(realpath "$scripts/../tests/")
#     monitoring_sources=$tests/common
#     INCLUDES+=" -I$monitoring_sources"
#     CFLAGS+=" -g -O0"

#     if $logs
#     then
#         C_FILES+=" $(realpath $monitoring_sources/msg_logger.c)"
#     else
#         C_FILES+=" $(realpath $monitoring_sources/msg_counter.c)"
#     fi
# fi

# if [[ ! -f $target || $source -nt $target ]] || $force; then
#     $GCC $INCLUDES $CFLAGS $LDLFLAGS $C_FILES $source -o $target &&
#     echo Compiled to: $target ||
#     echo Failed to compile $source
# else
#     echo $target is up to date
# fi