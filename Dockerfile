FROM debian:11

RUN apt-get update && apt-get install -y --no-install-recommends \
  build-essential \
  cmake \
  ca-certificates \
  git \
  ninja-build \
  libglfw3-dev \
  libfreetype-dev \
  libmpv-dev \
  liblzma-dev \
  pkg-config \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/*

ARG UID=1000
ARG GID=1000
ARG USER=grail
RUN groupadd -g $GID -o $USER && useradd -l -m -u "$UID" -g "$GID" -o -s /bin/bash $USER
USER $USER
RUN mkdir /home/${USER}/grail
WORKDIR /home/${USER}/grail
