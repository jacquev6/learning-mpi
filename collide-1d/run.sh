#!/bin/bash

set -o errexit
cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1


image=$(docker build --quiet builder)

docker run \
  --rm --interactive --tty \
  --user $(id -u):$(id -g) \
  --volume $PWD:/wd --workdir /wd \
  $image bash -c """
    set -o errexit

    mkdir -p build

    build_options='-std=c++17 -Wall -Wextra -Werror -pedantic'

    g++ \$build_options collide-1d.cpp $(pkg-config --cflags --libs mpi-cxx) -o build/collide-1d
    mpiexec -v -n 3 build/collide-1d
"""
