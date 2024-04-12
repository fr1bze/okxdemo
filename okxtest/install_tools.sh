apt-get update
apt-get upgrade
apt-get install -y \
    wget \
    tar \
    g++ \
    make \
    git \
    cmake \
    build-essential \
    && rm -rf /var/lib/apt/lists/*