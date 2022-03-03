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

    g++ \$build_options collide-1d.cpp \$(pkg-config --cflags --libs cairomm-png-1.0 mpi-cxx) -o build/collide-1d

    rm -f build/out-tmp.mp4 build/*.png
    mpiexec -v -n 4 build/collide-1d build 10

    echo 'Post-processing'

    for index in \$(find build -name '00000000-*.png' | sed -e 's/^build\/00000000-//' -e 's/\.png$//' | sort)
    do
      echo gm convert build/*-\$index.png +append build/\$index.png;
    done | parallel -j20

    ffmpeg -i build/%08d.png -c:v libx264 -vf fps=25 -pix_fmt yuv420p build/out-tmp.mp4 >/dev/null 2>&1
    mv build/out-tmp.mp4 build/out.mp4
"""
