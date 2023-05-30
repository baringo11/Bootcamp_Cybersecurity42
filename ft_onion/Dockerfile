FROM debian:buster

RUN apt-get update && apt-get -y install nginx
RUN apt-get -y install tor
RUN apt-get -y install openssh-server
RUN apt-get -y install curl

COPY /conf/sample.conf /etc/nginx/sites-available/default
COPY /conf/torrc /etc/tor/torrc
COPY /conf/sshd_config /etc/ssh/sshd_config
COPY /conf/web/index.html /usr/share/nginx/html/index.html
COPY /conf/web/style.css /usr/share/nginx/html/style.css
COPY /conf/web/matrix.js /usr/share/nginx/html/matrix.js

CMD service nginx start && service tor start && service ssh start && cat /var/lib/tor/hidden_service/hostname && bash