#########
# Build #
#########
FROM ubuntu:20.04 AS builder

# System packages
RUN set -x \
 && apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends \
      g++ `# Build tools` \
      libopenmpi-dev `# Direct dependencies` \
 && apt-get clean

WORKDIR /build
ADD io.cpp .
RUN mpic++ -std=c++17 io.cpp -o io


#######
# Run #
#######
FROM ubuntu:20.04

# System packages
RUN set -x \
 && apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends \
      openssh-server libopenmpi3 openmpi-bin `# Direct dependencies` \
 && apt-get clean

# Minimal setup to enable SSH between containers
# Inspired from https://github.com/oweidner/docker.openmpi and simplified
RUN set -x \
 && mkdir /run/sshd /root/.ssh \
 && echo "StrictHostKeyChecking no" >/root/.ssh/config \
 && echo "LogLevel ERROR" >>/root/.ssh/config \
 && ssh-keygen -f /root/.ssh/id_rsa \
 && cp /root/.ssh/id_rsa.pub /root/.ssh/authorized_keys
CMD ["/usr/sbin/sshd", "-D"]

COPY --from=builder /build/io /bin
