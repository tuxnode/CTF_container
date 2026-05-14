FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    bash \
    cron \
    sudo \
    passwd \
    openssh-server \
    && rm -rf /var/lib/apt/lists/*

# SSH 配置
RUN mkdir -p /var/run/sshd && \
    sed -i 's/#PasswordAuthentication yes/PasswordAuthentication yes/' /etc/ssh/sshd_config && \
    sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin no/' /etc/ssh/sshd_config && \
    echo 'UseDNS no' >> /etc/ssh/sshd_config

# 创建 guest 用户
RUN useradd -m -s /bin/bash guest && \
    echo "guest:guest" | chpasswd

# 植入 flag
RUN echo "HCTF{sk3l_1nj3ct10n_is_r34l}" > /root/flag.txt && \
    chmod 600 /root/flag.txt

# 核心漏洞：/etc/skel 对 guest 可写
RUN chown -R guest:guest /etc/skel

# 定时任务：每 2 分钟创建 admin 用户（sudo 权限）并登录
RUN cat > /etc/cron.d/admin-job << 'EOF'
*/1 * * * * root userdel -r admin 2>/dev/null; useradd -m -s /bin/bash admin && cp /root/flag.txt /home/admin/.flag && chown admin:admin /home/admin/.flag && chmod 400 /home/admin/.flag && su -l admin -c 'bash --login -i -c exit'
EOF
RUN chmod 0644 /etc/cron.d/admin-job

# sudo 免密（admin 用户可无密码执行所有命令）
RUN echo 'admin ALL=(ALL) NOPASSWD: ALL' > /etc/sudoers.d/admin && \
    chmod 440 /etc/sudoers.d/admin

# 登录提示
RUN cat > /etc/motd << 'EOF'

===========================================
  CTF Challenge: Skeleton Key
===========================================
  User    : guest / guest
  Goal    : Read /root/flag.txt
  Hint    : Something happens every 1 minutes
===========================================

EOF

COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

EXPOSE 22

CMD ["/entrypoint.sh"]
