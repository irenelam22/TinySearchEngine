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
echo "Currently using -DTEST flag to see progress indicators (e.g. file ID and URL)."
echo "(Please comment out for brevity, see Makefile in common to delete the flag)"
echo "Please use valgrind to check for memory leaks"

echo "=========================================================="

echo "Level 0 Testing: Parameter Checks"

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
if [ $? -eq 0 ]; then
    echo >&2 "Error caught: Crawler ignores and continues on an invalid server"
fi

echo "Error-handling: Testing crawler on a valid server but non-existent page"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/indexx.html ../garbage/ 2
if [ $? -eq 0 ]; then
    echo >&2 "Error caught: Crawler ignores and continues on nonexistent pages"
fi

echo "Error-handling: Testing crawler on invalid depth (negative depth)"
./crawler http://old-www.cs.dartmouth.edu/index.html ../garbage/ -1
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Crawler does not accept invalid depths"
fi

echo "Error-handling: Testing crawler on invalid depth (greater than 10 depth)"
./crawler http://old-www.cs.dartmouth.edu/index.html ../garbage/ 15
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Crawler does not accept invalid depths"
fi

echo "Error-handling: Testing crawler on invalid directory (NULL directory)"
./crawler http://old-www.cs.dartmouth.edu/index.html ../NULL/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Crawler does not accept NULL directory"
fi

mkdir ../garbage.1

echo "Error-handling: Testing crawler on directories without backslash"
./crawler http://old-www.cs.dartmouth.edu/index.html ../garbage.1 2
if [ $? -eq 0 ]; then
    echo >&2 "Crawler adds a backslash for you!"
fi

mkdir ../garbage.2

echo "Error-handling: Testing crawler on directories with a backslash"
./crawler http://old-www.cs.dartmouth.edu/index.html ../garbage.2/ 2
if [ $? -eq 0 ]; then
    echo >&2 "Crawler continues if there already is a backslash"
fi

echo "Error-handling: Testing crawler on invalid directory (doesn't exist)"
./crawler http://old-www.cs.dartmouth.edu/index.html ../newdirectory/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Crawler does not accept nonexistent directories"
fi

mkdir ../garbageWithoutWrite
chmod -w ../garbageWithoutWrite

echo "Error-handling: Testing crawler on invalid directory (no write permission)"
./crawler http://old-www.cs.dartmouth.edu/index.html ../garbageWithoutWrite/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Directory must be writable"
fi

chmod +w ../garbageWithoutWrite 

echo "=========================================================="

mkdir -p ../data1

echo "Level 1 Testing: Simple tests & webpages with cycles"

echo "Testing crawler on simple html file"
./crawler http://old-www.cs.dartmouth.edu/index.html ../data1/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on simple html failed"
    exit 1
fi

echo "Expect 1 files. Crawled files: "
ls ../data1/ | wc -l

mkdir -p ../data2

echo "Crawling a simple, closed set of cross-linked web pages"
echo "Testing on letters html at depth 4 (contains loops)"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ../data2/ 4
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on letters html failed"
    exit 1
fi

echo "Expect 8 files. Crawled files: "
ls ../data2/ | wc -l

echo "Manual verification of files -- no pages are missing or duplicated"

echo "=========================================================="

echo "Level 2 Testing: Exploring at depths 0, 1, 2, 3, 4, 5 and verifying"

mkdir -p ../data3.0
echo "Now crawling Letters html at Depth 0"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ../data3.0/ 0
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on letters html Depth 0 failed"
    exit 1
fi

echo "Expect 1 file from Letters html at Depth 0. Crawled files: "
ls ../data3.0/ | wc -l

mkdir -p ../data3.1
echo "Now crawling Letters html at Depth 1"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ../data3.1/ 1
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on letters html Depth 1 failed"
    exit 1
fi

echo "Expect 2 files from Letters html at Depth 1. Crawled files: "
ls ../data3.1/ | wc -l

mkdir -p ../data3.2
echo "Now crawling Letters html at Depth 2"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ../data3.2/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on letters html Depth 2 failed"
    exit 1
fi

echo "Expect 3 files from Letters html at Depth 2. Crawled files: "
ls ../data3.2/ | wc -l

mkdir -p ../data3.3
echo "Now crawling Letters html at Depth 3"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ../data3.3/ 3
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on letters html Depth 3 failed"
    exit 1
fi

echo "Expect 6 files from Letters html at Depth 3. Crawled files: "
ls ../data3.3/ | wc -l

mkdir -p ../data3.4
echo "Now crawling Letters html at Depth 4"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ../data3.4/ 4
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on letters html Depth 4 failed"
    exit 1
fi

echo "Expect 8 files from Letters html at Depth 4. Crawled files: "
ls ../data3.4/ | wc -l

mkdir -p ../data3.5
echo "Now crawling Letters html at Depth 5"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ../data3.5/ 5
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on letters html Depth 5 failed"
    exit 1
fi

echo "Expect 9 files from Letters html at Depth 5. Crawled files: "
ls ../data3.5/ | wc -l

echo "Manual verification of files -- files match expectations"

echo "=========================================================="

echo "Level 2 Testing cont.: Repeating with a different seed page in the same site"

mkdir -p ../data4.0
echo "Now crawling B html at Depth 0"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html ../data4.0/ 0
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on B html Depth 0 failed"
    exit 1
fi

echo "Expect 1 files from B html at Depth 0. Crawled files: "
ls ../data4.0/ | wc -l

mkdir -p ../data4.1
echo "Now crawling B html at Depth 1"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html ../data4.1/ 1
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on B html Depth 1 failed"
    exit 1
fi

echo "Expect 5 files from B html at Depth 1. Crawled files: "
ls ../data4.1/ | wc -l

mkdir -p ../data4.2
echo "Now crawling B html at Depth 2"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html ../data4.2/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on B html Depth 2 failed"
    exit 1
fi

echo "Expect 8 file from B html at Depth 2. Crawled files: "
ls ../data4.2/ | wc -l

mkdir -p ../data4.3
echo "Now crawling B html at Depth 3"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html ../data4.3/ 3
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on B html Depth 3 failed"
    exit 1
fi

echo "Expect 9 files from B html at Depth 3. Crawled files: "
ls ../data4.3/ | wc -l

mkdir -p ../data4.4
echo "Now crawling B html at Depth 4"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html ../data4.4/ 4
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on B html Depth 4 failed"
    exit 1
fi

echo "Expect 9 files from B html at Depth 4. Crawled files: "
ls ../data4.4/ | wc -l

mkdir -p ../data4.5
echo "Now crawling B html at Depth 5"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html ../data4.5/ 5
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on B html Depth 5 failed"
    exit 1
fi

echo "Expect 9 files from B html at Depth 5. Crawled files: "
ls ../data4.5/ | wc -l

echo "Manual verification of files -- files match expectations"

echo "=========================================================="

echo "Level 3 Testing: Wikipedia playground at depths 0, 1, 2"

mkdir -p ../data5.0

echo "Testing crawler on wikipedia html file at depth 0"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html ../data5.0/ 0
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on wikipedia html at depth 0 failed"
    exit 1
fi

echo "Expect 1 file from Wikipedia html at Depth 0. Crawled files: "
ls ../data5.0/ | wc -l

mkdir -p ../data5.1

echo "Testing crawler on wikipedia html file at depth 1"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html ../data5.1/ 1
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on wikipedia html at depth 1 failed"
    exit 1
fi

echo "Expect 7 files from Wikipedia html at Depth 1. Crawled files: "
ls ../data5.1/ | wc -l

mkdir -p ../data5.2

echo "Testing crawler on wikipedia html file at depth 2"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html ../data5.2/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on wikipedia html at depth 2 failed"
    exit 1
fi

echo "Expect 1675 files from Wikipedia html at Depth 2. Crawled files: "
ls ../data5.2/ | wc -l

echo "=========================================================="

echo "Level 5 Testing: Additional tests"

mkdir -p ../data6.0
echo "Testing crawler on toscrape html file at depth 0"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html ../data6.0/ 0
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on toscrape html at depth 0 failed"
    exit 1
fi
echo "Expect 1 file from toscrape html at Depth 0. Crawled files: "
ls ../data6.0/ | wc -l

mkdir -p ../data6.1
echo "Testing crawler on toscrape html file at depth 1"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html ../data6.1/ 1
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on toscrape html at depth 1 failed"
    exit 1
fi
echo "Expect 73 files from toscrape html at Depth 1. Crawled files: "
ls ../data6.1/ | wc -l

mkdir -p ../data6.2
echo "Testing crawler on toscrape html file at depth 2"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html ../data6.2/ 2
if [ $? -ne 0 ]; then
    echo >&2 "Crawler on toscrape html at depth 2 failed"
    exit 1
fi
echo "Expect 585 files from toscrape html at Depth 2. Crawled files: "
ls ../data6.2/ | wc -l