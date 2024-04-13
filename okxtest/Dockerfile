FROM ubuntu:18.04

RUN apt-get update && \
    apt-get install -y \
    git \
    wget \
    make \
    gcc \
    perl \
    build-essential \
    libwebsocketpp-dev \
    nlohmann-json-dev \
    libfindbin-libs-perl

RUN wget https://www.openssl.org/source/openssl-1.1.1l.tar.gz && \
    tar -xzvf openssl-1.1.1l.tar.gz && \
    cd openssl-1.1.1l && \
    ./config && \
    make && \
    make install && \
    ldconfig && \
    cd .. && \
    rm -rf openssl-1.1.1l.tar.gz openssl-1.1.1l

RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.68.0/source/boost_1_68_0.tar.gz
RUN tar -xvzf boost_1_68_0.tar.gz
WORKDIR /boost_1_68_0
RUN ./bootstrap.sh
RUN ./b2 install


WORKDIR /app
COPY okxtest_client.cpp .

# Compile your code with the necessary includes and linking
RUN g++ -std=c++11 -o okxtest okxtest_client.cpp -I/usr/local/include/websocketpp -pthread -DBOOST_ERROR_CODE_HEADER_ONLY -L/usr/local/lib -lssl -lcrypto -lboost_system -lboost_thread

CMD ["./okxtest"]