#!/bin/sh

# Determine the path to .modfwango
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

# Determine the number of CPU cores
CORES=$(grep -c ^processor /proc/cpuinfo 2>/dev/null || sysctl -n hw.ncpu || \
  echo 1)

# Change to ${DIR}
cd ${DIR}

# Fetch the latest commit and Makefile
git pull origin master
git submodule update --init

# Rebuild Modfwango
make -j${CORES} clean all
