# CTF 发布包 Makefile
# kernel/kernel 和 utils/_* 均为预编译，选手只需编辑 user/attack.c

K = kernel
U = user

# 自动检测 TOOLPREFIX
ifndef TOOLPREFIX
TOOLPREFIX := $(shell if riscv64-unknown-elf-objdump -i 2>&1 | grep 'elf64-big' >/dev/null 2>&1; \
	then echo 'riscv64-unknown-elf-'; \
	elif riscv64-elf-objdump -i 2>&1 | grep 'elf64-big' >/dev/null 2>&1; \
	then echo 'riscv64-elf-'; \
	elif riscv64-linux-gnu-objdump -i 2>&1 | grep 'elf64-big' >/dev/null 2>&1; \
	then echo 'riscv64-linux-gnu-'; \
	elif riscv64-unknown-linux-gnu-objdump -i 2>&1 | grep 'elf64-big' >/dev/null 2>&1; \
	then echo 'riscv64-unknown-linux-gnu-'; \
	else echo "***" 1>&2; \
	echo "*** Error: Couldn't find a riscv64 version of GCC/binutils." 1>&2; \
	echo "***" 1>&2; exit 1; fi)
endif

QEMU             = qemu-system-riscv64
MIN_QEMU_VERSION = 7.2

CC      = $(TOOLPREFIX)gcc
LD      = $(TOOLPREFIX)ld
OBJDUMP = $(TOOLPREFIX)objdump

CFLAGS = -Wall -Werror -O -fno-omit-frame-pointer -ggdb -gdwarf-2
CFLAGS += -DLAB_SYSCALL
CFLAGS += -MD
CFLAGS += -mcmodel=medany
CFLAGS += -ffreestanding
CFLAGS += -fno-common -nostdlib
CFLAGS += -fno-builtin-strncpy -fno-builtin-strncmp -fno-builtin-strlen -fno-builtin-memset
CFLAGS += -fno-builtin-memmove -fno-builtin-memcmp -fno-builtin-log -fno-builtin-bzero
CFLAGS += -fno-builtin-strchr -fno-builtin-exit -fno-builtin-malloc -fno-builtin-putc
CFLAGS += -fno-builtin-free
CFLAGS += -fno-builtin-memcpy -Wno-main
CFLAGS += -fno-builtin-printf -fno-builtin-fprintf -fno-builtin-vprintf
CFLAGS += -I.
CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)

# Disable PIE when possible
ifneq ($(shell $(CC) -dumpspecs 2>/dev/null | grep -e '[^f]no-pie'),)
CFLAGS += -fno-pie -no-pie
endif
ifneq ($(shell $(CC) -dumpspecs 2>/dev/null | grep -e '[^f]nopie'),)
CFLAGS += -fno-pie -nopie
endif

LDFLAGS = -z max-page-size=4096

# 预编译的 ulib（选手不可见源码）
ULIB = $U/ulib.o $U/usys.o $U/printf.o $U/umalloc.o

# 编译 attack.c → _attack
$U/_attack: $U/attack.o $(ULIB) $U/user.ld
	$(LD) $(LDFLAGS) -T $U/user.ld -o $@ $U/attack.o $(ULIB)

$U/attack.o: $U/attack.c
	$(CC) $(CFLAGS) -c -o $U/attack.o $U/attack.c

# utils/ 下预编译 ELF，排除旧的 _attack
PREBUILT = $(filter-out utils/_attack, $(wildcard utils/_*))

# _init 必须是 mkfs 第一个参数（内核从 inode 1 启动）
fs.img: mkfs/mkfs README.md $(PREBUILT) $U/_attack
	cp $(PREBUILT) $U/
	mkfs/mkfs fs.img README.md \
	    $U/_init \
	    $(patsubst utils/%,$U/%,$(filter-out utils/_init,$(PREBUILT))) \
	    $U/_attack
	
# QEMU

CPUS = 3

QEMUOPTS  = -machine virt -bios none -kernel $K/kernel -m 128M -smp $(CPUS) -nographic
QEMUOPTS += -global virtio-mmio.force-legacy=false
QEMUOPTS += -drive file=fs.img,if=none,format=raw,id=x0
QEMUOPTS += -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0

.PHONY: qemu qemu-gdb clean check-qemu-version

qemu: check-qemu-version fs.img
	$(QEMU) $(QEMUOPTS)

clean:
	rm -f $U/attack.o $U/_attack $U/attack.d fs.img
	rm -f $(patsubst utils/%,$U/%,$(PREBUILT))

QEMU_VERSION := $(shell $(QEMU) --version | head -n 1 | sed -E 's/^QEMU emulator version ([0-9]+\.[0-9]+)\..*/\1/')
check-qemu-version:
	@if [ "$(shell echo "$(QEMU_VERSION) >= $(MIN_QEMU_VERSION)" | bc)" -eq 0 ]; then \
		echo "ERROR: Need qemu version >= $(MIN_QEMU_VERSION)"; \
		exit 1; \
	fi

-include $U/attack.d
