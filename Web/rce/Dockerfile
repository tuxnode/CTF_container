FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    apache2 \
    php \
    libapache2-mod-php \
    iputils-ping \
    && apt-get clean

RUN rm /var/www/html/index.html
COPY index.php /var/www/html/

RUN echo "HCTF{PHP_Remote_Code_Execution_Success}" > /flag \
    && chmod 444 /flag

EXPOSE 80

CMD ["/usr/sbin/apache2ctl", "-D", "FOREGROUND"]
