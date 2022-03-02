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

    g++ -std=c++17 -lgtest_main -lgtest -DAUTOMATED_TESTS collide-1d.cpp $(pkg-config --cflags --libs mpi-cxx) -o build/collide-1d-tests
    build/collide-1d-tests

    g++ -std=c++17 collide-1d.cpp $(pkg-config --cflags --libs mpi-cxx) -o build/collide-1d
    mpiexec -v -n 8 build/collide-1d
"""
