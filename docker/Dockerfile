FROM ubuntu:14.04
MAINTAINER Trevor Pesout, tpesout@ucsc.edu
ARG git_commit=master

# update and install dependencies
RUN apt-get update && apt-get -y install git make wget autoconf g++ time zlib1g-dev

# install latest cmake
WORKDIR /tmp
RUN mkdir /opt/cmake && \
    wget https://cmake.org/files/v3.11/cmake-3.11.4-Linux-x86_64.sh && \
    sh /tmp/cmake-3.11.4-Linux-x86_64.sh --prefix=/opt/cmake --skip-license && \
    ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake

# get napper
WORKDIR /opt
RUN git clone https://github.com/kishwarshafin/napper.git && \
    cd /opt/napper && \
    git fetch && \
    git checkout $git_commit && \
    rm -rf /opt/napper/.git

# build
WORKDIR /opt/napper/build
RUN cmake .. && make

# setup entrypoint
COPY wrapper.sh /opt/
ENTRYPOINT ["bash", "/opt/wrapper.sh"]
