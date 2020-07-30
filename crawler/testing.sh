#!/bin/bash
# testing.sh -- testing file for crawler
# Usage: make test
# Input: none
# Output: Testing results, see testing.out for more specific results
#
# Irene Lam, July 27, 2020
# Dartmouth CS50, Summer 2020

mkdir -p ../garbage
make

echo "Integration testing for crawler module."
echo "Please use -DTEST flag to see progress indicators."

echo "Error-handling: Insufficient arguments"
./crawler https://www.cs.dartmouth.edu/~cs50/ ../garbage/ 
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Insufficient arguments provided"
fi

echo "Error-handling: Too many arguments"
./crawler http://old-www.cs.dartmouth.edu/index.html ../garbage/ ../test/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Too many arguments provided"
fi

echo "Error-handling: Testing crawler on invalid site (not internal)"
./crawler https://www.cs.dartmouth.edu/~cs50/ ../garbage/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Crawler ignores sites that are not internal"
fi

echo "Error-handling: Testing crawler on invalid server (non-existent)"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letterss/index.html ../garbage/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Crawler ignores sites that are not internal"
fi

echo "Error-handling: Testing crawler on a valid server but non-existent page"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/indexx.html ../garbage/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Crawler ignores sites that are not internal"
fi

echo "Error-handling: Testing crawler on invalid depth (negative depth)"
./crawler http://old-www.cs.dartmouth.edu/index.html ../garbage/ -1
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Crawler does not accept invalid depths"
fi

echo "Error-handling: Testing crawler on invalid directory (NULL directory)"
./crawler http://old-www.cs.dartmouth.edu/index.html ../NULL/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Crawler does not accept NULL directory"
fi

echo "Error-handling: Testing crawler on invalid directory (without backslash)"
./crawler http://old-www.cs.dartmouth.edu/index.html ../garbage 2
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Must add backslash to directory"
fi

echo "Error-handling: Testing crawler on invalid directory (doesn't exist)"
./crawler http://old-www.cs.dartmouth.edu/index.html ../newdirectory/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Must add backslash to directory"
fi

chmod -w ../garbage

echo "Error-handling: Testing crawler on invalid directory (no write permission)"
./crawler http://old-www.cs.dartmouth.edu/index.html ../garbage/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Must add backslash to directory"
fi

chmod +w ../garbage 

mkdir -p ../data1

echo "Testing crawler on simple html file"
./crawler http://old-www.cs.dartmouth.edu/index.html ../data1/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on simple html failed"
    exit 1
fi

mkdir -p ../data4

echo "Crawling a simple, closed set of cross-linked web pages"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ../data4/ 4
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on letters html failed"
    exit 1
fi

echo "Expect 8 files. Crawled files: "
ls ../data4/ | wc -l

# mkdir -p ../data2

# echo "Testing crawler on toscrape html file"
# ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html ../data2/ 2
# if [ $? -ne 0 ]; then
#     echo >&2 "Crawler on toscrape html failed"
#     exit 1
# fi

# mkdir -p ../data3

# echo "Testing crawler on wikipedia html file"
# ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html ../data3/ 2
# if [ $? -ne 0 ]; then
#     echo >&2 "Crawler on wikipedia html failed"
#     exit 1
# fi

#ls ../data2/ | wc -l
