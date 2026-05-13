# 简介 
这是一个基于RISC-V架构所编写的操作系统内核

但是有一个关键漏洞

## 环境配置

### Ubuntu / Debian
```bash
sudo apt-get install -y \
    gcc-riscv64-unknown-elf \
    binutils-riscv64-unknown-elf \
    qemu-system-riscv64

# 验证版本
riscv64-unknown-elf-gcc --version
qemu-system-riscv64 --version  # 需要 >= 7.2
```

### Arch Linux
```bash
sudo pacman -S riscv64-elf-gcc riscv64-elf-binutils qemu-system-riscv
```

## 目录结构

├── Makefile          # 构建脚本，make qemu 一键启动
├── README.md         # 本文件
├── kernel/
│   └── kernel        # 预编译的 xv6 内核（不可修改）
├── mkfs/
│   └── mkfs          # 文件系统构建工具
├── user/
│   ├── attack.c      # 你需要编写的攻击程序
│   ├── user.h        # 用户态系统调用接口
│   ├── user.ld       # 链接脚本
│   ├── ulib.o        # 用户态运行库（预编译）
│   ├── usys.o        # 系统调用桩（预编译）
│   ├── printf.o      # 格式化输出（预编译）
│   └── umalloc.o     # 内存分配（预编译）
└── utils/            # 预编译的用户程序（init、sh、secret 等）

**这个题目无需反编译/反汇编**

## 漏洞原理

正常的操作系统在将物理内存页分配给进程之前，会先将页面内容清零，防止进程读取到其他进程的残留数据。xv6 在 `kernel/vm.c` 的 `uvmalloc()` 中通过 `memset(mem, 0, sz)` 实现这一清零操作。

本挑战的内核在编译时通过宏跳过了以下三处清零操作：

- `kernel/vm.c` 中 `uvmalloc()` 的 `memset`（新分配页清零）
- `kernel/kalloc.c` 中 `kalloc()` 的 `memset`（分配时清零）
- `kernel/kalloc.c` 中 `kfree()` 的 `memset`（释放时填充垃圾值）

## 实验背景

内核启动后，`secret` 程序会自动运行并将一段 flag 字符串写入其内存，随后退出。进程退出时，其占用的物理页被归还给内核的空闲页链表（freelist），但由于清零操作被跳过，flag 数据仍然残留在这些页面中。

当你运行 `attack` 程序时，通过 `sbrk()` 申请内存，有可能拿到 `secret` 用过的那些物理页，从而读取到其中残留的 flag。
```
secret 写入 flag → exit() → 物理页归还 freelist（数据未清零）
                                        ↓
attack 调用 sbrk() → 分配到同一物理页 → 读取残留数据 → 输出 flag
```


## 所需信息

可审计的内核源码见 `kernel/vm.c`，重点关注 `uvmalloc()` 函数中被条件编译跳过的部分。

`kernel/kalloc.c`中是分配物理内存的逻辑，可供参考

## Usage

编辑 `user/attack.c`，实现窃取 flag 的逻辑，然后：
```bash
make qemu
```

进入 shell 后运行：
```
$ secret   #将flag写入物理页并释放
$ attack   #攻击获得flag
```
按 `Ctrl-A X` 退出 QEMU。
