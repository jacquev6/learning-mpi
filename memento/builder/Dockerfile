FROM ubuntu:20.04

# System packages
RUN set -x \
 && apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends \
      g++ `# Build tools` \
      libopenmpi-dev `# Direct dependencies` \
      valgrind `# Debug tools` \
 && apt-get clean
