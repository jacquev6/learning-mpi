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
    mpic++ memento.cpp -o build/memento

    mpiexec -v -n 3 build/memento
    mpiexec -v -n 7 build/memento
"""
