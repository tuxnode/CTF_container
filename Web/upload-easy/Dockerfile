FROM php:7.4-apache

# 设置工作目录
WORKDIR /var/www/html

# 复制题目代码
COPY index.php .

# 创建上传目录并设置权限
RUN mkdir -p uploads && \
    chmod 777 uploads && \
    # 写入 flag
    echo "HCTF{uPl0ad_is_fUn_2026}" > /flag

# 修改 Apache 配置，允许解析 phtml
RUN echo "<FilesMatch \".+\.phtml$\"> \n\
    SetHandler application/x-httpd-php \n\
</FilesMatch>" >> /etc/apache2/apache2.conf

# 暴露端口
EXPOSE 80
