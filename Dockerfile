FROM docker.io/ubuntu:22.04
LABEL description="mosaic-artifact"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
        build-essential software-properties-common && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    add-apt-repository -y ppa:zeehio/libxp && \
    dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get install -y \
        wget \
	curl \
        git make gcc g++ \
        python3 python3-dev python3-pip python3-venv \
        graphviz \
        xxd \
        time \
        vim \
        cmake

# Switch shell to bash
SHELL ["/bin/bash", "--login", "-c"]

COPY . /mosaic-artifact

WORKDIR /mosaic-artifact/mosaic
RUN mkdir build

WORKDIR /mosaic-artifact/mosaic/build 
RUN cmake -DCMAKE_BUILD_TYPE='Release'\
    -DCMAKE_INSTALL_PREFIX="${HOME}"  \
    -DBENCHMARK_DOWNLOAD_DEPENDENCIES=ON  ../ 
RUN make



