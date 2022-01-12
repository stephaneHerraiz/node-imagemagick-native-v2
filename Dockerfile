FROM node:12
# https://registry.hub.docker.com/u/dockerfile/nodejs/

RUN apt-get update
RUN apt-get -y install \
    git \
    imagemagick \
    libmagick++-dev \
    ghostscript \
    node-gyp \
    emacs

# RUN cd /data && git clone https://github.com/mash/node-imagemagick-native.git
WORKDIR /data

CMD ["bash"]
