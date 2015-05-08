#!/bin/sh

# Determine the path to .modfwango
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

# Change to ${DIR}
cd ${DIR}

# Fetch the latest commit and Makefile
git pull origin master
git submodule update --init

# Rebuild Modfwango
make clean all
