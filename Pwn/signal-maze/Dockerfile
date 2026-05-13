FROM ubuntu:22.04

RUN apt-get update && apt-get install -y gcc openssh-server && rm -rf /var/lib/apt/lists/*

COPY challenge/chall.c /challenge/chall.c
COPY setup/entrypoint.sh /entrypoint.sh

RUN gcc -o /challenge/chall /challenge/chall.c
RUN sed -i 's/\r//' /entrypoint.sh 
RUN mkdir /var/run/sshd
RUN chmod +x /entrypoint.sh

# 不要写 EXPOSE 22，让GZCTF来管理端口映射

ENTRYPOINT ["/entrypoint.sh"]
