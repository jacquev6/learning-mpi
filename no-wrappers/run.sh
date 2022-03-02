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

    gcc hello.c $(pkg-config --cflags --libs mpi) -o build/hello-c
    mpiexec -v -n 4 build/hello-c

    echo

    g++ -std=c++17 hello.cpp $(pkg-config --cflags --libs mpi-cxx) -o build/hello-cpp
    mpiexec -v -n 4 build/hello-cpp
"""
