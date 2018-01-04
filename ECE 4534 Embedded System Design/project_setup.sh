#!/usr/bin/env bash

#get input
read -p "Enter project DIR:" dir

cd src

#h and c
find . -type f ! -name *.mhc ! -name system* -exec ln -srf {} $dir/firmware/src/{} \;
find . -type f ! -name *.mhc -name system* -exec ln -srf {} $dir/firmware/src/system_config/Main/{} \;
