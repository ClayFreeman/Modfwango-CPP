#!/bin/sh

# Determine the path to .modfwango
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

# Change to ${DIR}, fetch the latest commit, and rebuild
cd ${DIR} && git pull origin master && make clean all
