#!/bin/bash

# 每次启动重新设置 SUID（确保权限正确）
chmod u+s /usr/bin/tar

# 启动 SSH 服务
exec /usr/sbin/sshd -D
