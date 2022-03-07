#!/bin/bash

set -o errexit
cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1


rm -f wd/file.txt

docker-compose up --build --abort-on-container-exit

cat -A wd/file.txt
