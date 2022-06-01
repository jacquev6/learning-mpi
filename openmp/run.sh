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
    mpic++ -fopenmp openmp.cpp -o build/openmp

    for MPI_NUM_PROCESSORS in 1 2
    do
      for OMP_NUM_THREADS in 1 2 4
      do
        echo \$MPI_NUM_PROCESSORS MPI processors with \$OMP_NUM_THREADS OpenMP threads:
        time OMP_NUM_THREADS=\$OMP_NUM_THREADS mpiexec --bind-to none -v -n \$MPI_NUM_PROCESSORS build/openmp
        echo
      done
    done
"""
