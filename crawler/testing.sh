#!/bin/bash
# testing.sh -- testing file for crawler
# Usage: make test
# Input: none
# Output: Testing results, see testing.out for more specific results
#
# Irene Lam, July 27, 2020
# Dartmouth CS50, Summer 2020

pwd
mkdir -p ../garbage
make

# echo "Error-handling: Testing crawler on invalid site (not internal)"
# ./crawler https://www.cs.dartmouth.edu/~cs50/ ../garbage/ 2
# if [ $? -ne 0 ]; then
#     echo >&2 "Crawler ignores sites that are not internal"
# fi

# echo "Error-handling: Testing crawler on invalid depth (negative depth)"
# ./crawler http://old-www.cs.dartmouth.edu/index.html ../garbage/ -1
# if [ $? -ne 0 ]; then
#     echo >&2 "Crawler does not accept invalid depths"
# fi

mkdir -p ../data1

echo "Testing crawler on simple html file"
./crawler http://old-www.cs.dartmouth.edu/index.html ../data1/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on simple html failed"
    exit 1
fi

mkdir -p ../data2

echo "Testing crawler on toscrape html file"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html ../data2/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on toscrape html failed"
    exit 1
fi

ls ../data2/ | wc -l
