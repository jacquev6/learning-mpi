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
    mpic++ -g memento.cpp -o build/memento

    # We're using Valgrind to *see* the errors, but no effort has been made to *fix* them
    rm -f build/valgrind-*.log
    mpiexec -v -n 3 valgrind --leak-check=full --log-file=build/valgrind-3.%p.log build/memento
    mpiexec -v -n 7 valgrind --leak-check=full --log-file=build/valgrind-7.%p.log build/memento
"""
