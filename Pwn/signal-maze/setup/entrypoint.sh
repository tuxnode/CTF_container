#!/bin/bash
ssh-keygen -A
useradd -m -s /bin/bash ctf
echo "ctf:ctf" | chpasswd

# GZCTF通过环境变量注入flag
echo "${GZCTF_FLAG}" > /flag.txt
chmod 400 /flag.txt
chown root:root /flag.txt

cp /challenge/chall /home/ctf/chall
chown root:root /home/ctf/chall
chmod 4755 /home/ctf/chall

exec /usr/sbin/sshd -D
