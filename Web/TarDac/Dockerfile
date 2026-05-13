FROM ubuntu:22.04

# 安装必要工具
RUN apt-get update && apt-get install -y \
    openssh-server \
    tar \
    sudo \
    vim \
    curl \
    wget \
    && rm -rf /var/lib/apt/lists/*

# 创建普通用户 ctfuser，密码 ctfpassword
RUN useradd -m -s /bin/bash ctfuser && \
    echo "ctfuser:ctfpassword" | chpasswd

# 创建 root 密码（设复杂密码，不让直接登录）
RUN echo "root:$(openssl rand -base64 32)" | chpasswd

# 写入 flag
RUN mkdir -p /root && \
    echo "HCTF{t4r_s1u1d_d4c_pr1v3sc_ftw_2026}" > /root/flag.txt && \
    chmod 600 /root/flag.txt && \
    chown root:root /root/flag.txt

# 写入一些诱导性的提示文件
RUN echo "tar命令有一些权限配置问题哦～" > /home/ctfuser/README.txt && \
    echo "尝试找找" >> /home/ctfuser/README.txt && \
    chown ctfuser:ctfuser /home/ctfuser/README.txt

# 放一个假 flag 迷惑选手
RUN echo "flag{th1s_1s_a_fake_flag}" > /home/ctfuser/fake_flag.txt && \
    chown ctfuser:ctfuser /home/ctfuser/fake_flag.txt

# 配置 SSH 服务
RUN mkdir /var/run/sshd && \
    sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin no/' /etc/ssh/sshd_config && \
    sed -i 's/#PasswordAuthentication yes/PasswordAuthentication yes/' /etc/ssh/sshd_config && \
    sed -i 's/UsePAM yes/UsePAM no/' /etc/ssh/sshd_config

# 设置 tar SUID 权限（核心漏洞配置）
RUN chmod u+s /usr/bin/tar

# 添加启动脚本
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

EXPOSE 22

ENTRYPOINT ["/entrypoint.sh"]
