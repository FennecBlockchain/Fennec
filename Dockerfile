FROM ubuntu:20.04 as BUILDER

LABEL maintainer="iotapi322@vipor.net"
LABEL version="1.2.0"
LABEL description="Docker image for fennec node"





# Note can remove the opencl and ocl packages above when not building on a system for GPU/mining
# Included only for reference purposes if this container would be used for mining as well.

RUN   apt update &&  DEBIAN_FRONTEND=nointeractive  apt install --no-install-recommends -y   \
    build-essential \
    libcurl4-openssl-dev \
    software-properties-common \
    ubuntu-drivers-common \
    pkg-config \
    libtool \
    clinfo \
    autoconf \
    automake \
    libjansson-dev \
    libevent-dev \
    uthash-dev \
    nodejs \
    vim \
    libboost-chrono-dev \
    libboost-filesystem-dev \
    libboost-test-dev \
    libboost-thread-dev \
    libevent-dev \
    libminiupnpc-dev \
    libssl-dev \
    libzmq3-dev \
    help2man \
    python3 \
    libdb++-dev \
    bsdmainutils \
    wget \
     && \
    rm -rf /var/lib/apt/lists/* && \
    apt clean


# Install radiant-node
WORKDIR /root
COPY . /root/fennec-node/
WORKDIR /root/fennec-node/

RUN ./autogen.sh
#RUN ./configure  --with-gui=no --with-incompatible-bdb --disable-bench --disable-tests  --prefix=/build
RUN ./configure  --with-gui=no --with-incompatible-bdb
RUN make -j

RUN make install

FROM ubuntu:20.04


WORKDIR /usr/local/
COPY --from=BUILDER /build/ /usr/local/



RUN   apt update && DEBIAN_FRONTEND=nointeractive apt install --no-install-recommends -y   \
  libcurl4-openssl-dev \
  software-properties-common \
  ubuntu-drivers-common \
  clinfo \
  libjansson-dev \
  libevent-dev \
  uthash-dev \
  vim \
  libboost-chrono-dev \
  libboost-filesystem-dev \
  libboost-test-dev \
  libboost-thread-dev \
  libevent-dev \
  libminiupnpc-dev \
  libssl-dev \
  libzmq3-dev \
  help2man \
  python3 \
  libdb++-dev \
  wget

VOLUME ["/data","/root/fennec.conf"]

# The config file needs to have the block chain sent to /data


EXPOSE 8332 8333

ENTRYPOINT ["fennecd"]

