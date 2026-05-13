FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    gcc \
    openssh-server \
    sudo \
    vim\
    neovim \
    && rm -rf /var/lib/apt/lists/*

# 创建低权限用户
RUN useradd -m -s /bin/bash ctf && echo "ctf:ctf" | chpasswd

# 挑战程序
RUN cat > /tmp/check_time.c << 'EOF'
#include <stdio.h>
#include <time.h>

int main() {
    puts("=== Server Time Check ===");
    time_t t = time(NULL);
    printf("Current time: %ld\n", t);
    if (t % 2 == 0)
        puts("Even second - system is happy!");
    else
        puts("Odd second - system is grumpy!");
    return 0;
}
EOF

RUN gcc -o /usr/local/bin/check_time /tmp/check_time.c \
    && chown root:root /usr/local/bin/check_time \
    && chmod 755 /usr/local/bin/check_time

# 允许 ctf 用户以 root 身份运行 check_time，且保留环境变量
RUN echo "ctf ALL=(root) SETENV: NOPASSWD: /usr/local/bin/check_time" \
    >> /etc/sudoers

# Flag
RUN echo "flag{LD_PR3L0AD_1s_M4g1c}" > /flag \
    && chmod 400 /flag \
    && chown root:root /flag

# 给选手的文件
RUN cat > /home/ctf/README.txt << 'EOF'
=== shadowed ===

目标：读取 /flag

提示：
  - 程序位于 /usr/local/bin/check_time
  - 你可以用 sudo 运行它
  - 源码见 /home/ctf/check_time.c
EOF

RUN cp /tmp/check_time.c /home/ctf/check_time.c \
    && chown ctf:ctf /home/ctf/README.txt /home/ctf/check_time.c

# SSH 配置
RUN mkdir /var/run/sshd \
    && sed -i 's/#PasswordAuthentication yes/PasswordAuthentication yes/' /etc/ssh/sshd_config \
    && sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin no/' /etc/ssh/sshd_config

EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]
