# 使用 Ubuntu 22.04 作为基础镜像
FROM ubuntu:22.04

# 环境变量设置，避免安装软件时产生交互提示
ENV DEBIAN_FRONTEND=noninteractive

# 安装 SSH 服务、GCC 编译器以及一些常用调试工具（方便选手做题）
RUN apt-get update && apt-get install -y \
    openssh-server \
    gcc \
    gdb \
    vim neovim binutils\
    && rm -rf /var/lib/apt/lists/*

# 配置 SSH 服务
RUN mkdir /var/run/sshd
# 允许 root 登录（可选，通常为了安全关闭，这里我们只让 ctf 用户登录）
RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin no/' /etc/ssh/sshd_config

# 创建 ctf 选手用户，并设置密码为 ctf
RUN useradd -m -s /bin/bash ctf && \
    echo 'ctf:ctf' | chpasswd

# 将宿主机的源码复制到容器内
COPY challenge.S /tmp/challenge.S

# 编译程序，并放入 ctf 用户的家目录
RUN gcc -nostdlib -no-pie /tmp/challenge.S  -o /home/ctf/challenge

# 创建 flag 文件
RUN echo "HCTF{Jump_Table_Reversing_Is_Fun}" > /flag

# 设置极其严格的权限
# 1. Flag 只有 root 可读
RUN chown root:root /flag && chmod 400 /flag
# 2. 程序所有者设为 root，并赋予 SUID 权限 (4755)
RUN chown root:root /home/ctf/challenge && chmod 4755 /home/ctf/challenge
# 3. 清理源码，防止选手直接看源码
RUN rm /tmp/challenge.S

# 暴露 SSH 端口
EXPOSE 22

# 启动 SSH 服务
CMD ["/usr/sbin/sshd", "-D"]
