#!/bin/bash
# testing.sh -- testing file for indexer
# Usage: make test
# Input: none
# Output: Testing results, see testing.out for more specific results
#
# Irene Lam, August 7, 2020
# Dartmouth CS50, Summer 2020

mkdir -p ../garbage123
mkdir -p ../garbage

echo "Integration testing for indexer module."
echo "PLEASE ensure you have tse-output downloading before running the below script."
echo "For details on how to download tse-output, please see the general README."
echo "Currently using -DTEST flag to see progress indicators (e.g. current file ID)."
echo "(Please comment out for brevity, see Makefile in common to delete the flag)"
echo "Note, using -qy tags for diff, meaning that only if there are differences will they be displayed"
echo "Please use valgrind to check for memory leaks"

echo "=========================================================="

echo "Testing indexer (error-handling)"

echo "Error-handling: Insufficient arguments"
./indexer ../tse-output/letters-depth-0
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Insufficient arguments provided"
fi

echo "Error-handling: Too many arguments"
./indexer ../tse-output/letters-depth-0 ../garbage/ ../tse-output/
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Too many arguments provided"
fi

echo "Error-handling: Testing indexer on invalid pagedirectory (not crawler-produced)"
./indexer ../garbage123/ ../garbage/new
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Indexer requires a crawler-produced directory"
fi

echo "Error-handling: Testing crawler on invalid pagedirectory (non-existent)"
./indexer ../tse-outputt/ ../garbage/new
if [ $? -eq 0 ]; then
    echo >&2 "Error caught: Indexer ignores a non-existent pagedirectory"
fi

echo "Error-handling: Testing indexer on directories with a backslash"
./indexer ../tse-output/letters-depth-0/ ../garbage/letters-index-00
if [ $? -eq 0 ]; then
    echo >&2 "Indexer continues if there already is a backslash"
fi

echo "Error-handling: Testing indexer on directories without a backslash"
./indexer ../tse-output/letters-depth-0 ../garbage/letters-index-01
if [ $? -eq 0 ]; then
    echo >&2 "Indexer adds a backslash if doesn't already exist"
fi

mkdir ../garbageWithoutWrite2
chmod -w ../garbageWithoutWrite2

echo "Error-handling: Testing indexer on invalid directory (no write permission)"
./indexer ../tse-output/letters-depth-0 ../garbageWithoutWrite2/letters-index-0
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Directory must be writable"
fi

chmod +w ../garbageWithoutWrite2

echo "=========================================================="
mkdir -p ../tse-output-new

echo "Testing indexer on letters at depth 0, 1, 2, 3, 4, 5"
echo "Note: These are simple closed sets of cross-linked web pages"

echo "Testing indexer on letters at depth 0 file"
./indexer ../tse-output/letters-depth-0/ ../tse-output-new/letters-index-0
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on letters at depth 0 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/letters-index-0) <(sort ../tse-output-new/letters-index-0)

echo "Testing indexer on letters at depth 1 file"
./indexer ../tse-output/letters-depth-1/ ../tse-output-new/letters-index-1
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on letters at depth 1 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/letters-index-1) <(sort ../tse-output-new/letters-index-1)

echo "Testing indexer on letters at depth 2 file"
./indexer ../tse-output/letters-depth-2/ ../tse-output-new/letters-index-2
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on letters at depth 2 failed"
    exit 1
fi

diff -qy <(sort ../tse-output/letters-index-2) <(sort ../tse-output-new/letters-index-2)

echo "Testing indexer on letters at depth 3 file"
./indexer ../tse-output/letters-depth-3/ ../tse-output-new/letters-index-3
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on letters at depth 3 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/letters-index-3) <(sort ../tse-output-new/letters-index-3)


echo "Testing indexer on a simple, closed set of cross-linked web pages"
echo "Testing on letters html at depth 4 (contains loops)"
./indexer ../tse-output/letters-depth-4/ ../tse-output-new/letters-index-4
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on letters html failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/letters-index-4) <(sort ../tse-output-new/letters-index-4)

echo "Testing indexer on letters at depth 5 file"
./indexer ../tse-output/letters-depth-5/ ../tse-output-new/letters-index-5
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on letters at depth 5 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/letters-index-5) <(sort ../tse-output-new/letters-index-5)

echo "Manual verification of files -- no words are missing or duplicated"

echo "=========================================================="
echo "Testing indexer on Wikipedia playground at depth 0, 1, 2"

echo "Testing indexer on Wikipedia at depth 0 file"
./indexer ../tse-output/wikipedia-depth-0/ ../tse-output-new/wikipedia-index-0
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on wikipedia at depth 0 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/wikipedia-index-0) <(sort ../tse-output-new/wikipedia-index-0)

echo "Testing indexer on Wikipedia at depth 1 file"
./indexer ../tse-output/wikipedia-depth-1/ ../tse-output-new/wikipedia-index-1
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on wikipedia at depth 1 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/wikipedia-index-1) <(sort ../tse-output-new/wikipedia-index-1)

echo "Testing indexer on Wikipedia at depth 2 file"
./indexer ../tse-output/wikipedia-depth-2/ ../tse-output-new/wikipedia-index-2
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on wikipedia at depth 2 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/wikipedia-index-2) <(sort ../tse-output-new/wikipedia-index-2)

echo "=========================================================="
echo "Testing indexer on toscrape playground at depth 0, 1, 2"

echo "Testing indexer on toscrape at depth 0 file"
./indexer ../tse-output/toscrape-depth-0/ ../tse-output-new/toscrape-index-0
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on toscrape at depth 0 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/toscrape-index-0) <(sort ../tse-output-new/toscrape-index-0)


echo "Testing indexer on toscrape at depth 1 file"
./indexer ../tse-output/toscrape-depth-1/ ../tse-output-new/toscrape-index-1
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on toscrape at depth 1 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/toscrape-index-1) <(sort ../tse-output-new/toscrape-index-1)

echo "Testing indexer on toscrape at depth 2 file"
./indexer ../tse-output/toscrape-depth-2/ ../tse-output-new/toscrape-index-2
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on toscrape at depth 2 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/toscrape-index-2) <(sort ../tse-output-new/toscrape-index-2)

echo "=========================================================="
echo "=========================================================="
echo "Testing indextest (error-handling)"
echo "Note: only basic argument number tests were done as stated by the REQUIREMENTS"

echo "Error-handling: Insufficient arguments"
./indextest ../tse-output-new/letters-depth-1/
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Insufficient arguments provided"
fi

echo "Error-handling: Too many arguments"
./indextest ../tse-output-new/letters-depth-1/ ../garbage/ ../tse-output/
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Too many arguments provided"
fi

echo "=========================================================="
mkdir ../tse-output-newtest

echo "Testing indextest on letters at depth 0, 1, 2, 3, 4, 5"
echo "Note: These are simple closed sets of cross-linked web pages"

echo "Testing indextest on letters at depth 0 file"
./indextest ../tse-output-new/letters-index-0 ../tse-output-newtest/letters-index-0
if [ $? -ne 0 ]; then
    echo >&2 "Indextest on letters at depth 0 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/letters-index-0) <(sort ../tse-output-newtest/letters-index-0)

echo "Testing indextest on letters at depth 1 file"
./indextest ../tse-output-new/letters-index-1 ../tse-output-newtest/letters-index-1
if [ $? -ne 0 ]; then
    echo >&2 "Indextest on letters at depth 1 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/letters-index-1) <(sort ../tse-output-newtest/letters-index-1)

echo "Testing indextest on letters at depth 2 file"
./indextest ../tse-output-new/letters-index-2 ../tse-output-newtest/letters-index-2
if [ $? -ne 0 ]; then
    echo >&2 "Indextest on letters at depth 2 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/letters-index-2) <(sort ../tse-output-newtest/letters-index-2)

echo "Testing indextest on letters at depth 3 file"
./indextest ../tse-output-new/letters-index-3 ../tse-output-newtest/letters-index-3
if [ $? -ne 0 ]; then
    echo >&2 "Indextest on letters at depth 3 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/letters-index-3) <(sort ../tse-output-newtest/letters-index-3)

echo "Testing indextest on letters at depth 4 file"
./indextest ../tse-output-new/letters-index-4 ../tse-output-newtest/letters-index-4
if [ $? -ne 0 ]; then
    echo >&2 "Indextest on letters at depth 4 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/letters-index-4) <(sort ../tse-output-newtest/letters-index-4)

echo "Testing indextest on letters at depth 5 file"
./indextest ../tse-output-new/letters-index-5 ../tse-output-newtest/letters-index-5
if [ $? -ne 0 ]; then
    echo >&2 "Indextest on letters at depth 5 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output/letters-index-5) <(sort ../tse-output-newtest/letters-index-5)

echo "Manual verification of files -- no words are missing or duplicated"

echo "=========================================================="
echo "Testing indextest on Wikipedia playground at depth 0, 1, 2"

echo "Testing indextest on Wikipedia at depth 0 file"
./indextest ../tse-output-new/wikipedia-index-0 ../tse-output-newtest/wikipedia-index-0
if [ $? -ne 0 ]; then
    echo >&2 "indextest on wikipedia at depth 0 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output-new/wikipedia-index-0) <(sort ../tse-output-newtest/wikipedia-index-0)

echo "Testing indextest on Wikipedia at depth 1 file"
./indextest ../tse-output-new/wikipedia-index-1 ../tse-output-newtest/wikipedia-index-1
if [ $? -ne 0 ]; then
    echo >&2 "indextest on wikipedia at depth 1 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output-new/wikipedia-index-1) <(sort ../tse-output-newtest/wikipedia-index-1)

echo "Testing indextest on Wikipedia at depth 2 file"
./indextest ../tse-output-new/wikipedia-index-2 ../tse-output-newtest/wikipedia-index-2
if [ $? -ne 0 ]; then
    echo >&2 "indextest on wikipedia at depth 2 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output-new/wikipedia-index-2) <(sort ../tse-output-newtest/wikipedia-index-2)

echo "=========================================================="
echo "Testing indextest on toscrape playground at depth 0, 1, 2"

echo "Testing indextest on toscrape at depth 0 file"
./indextest ../tse-output-new/toscrape-index-0 ../tse-output-newtest/toscrape-index-0
if [ $? -ne 0 ]; then
    echo >&2 "indextest on toscrape at depth 0 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output-new/toscrape-index-0) <(sort ../tse-output-newtest/toscrape-index-0)


echo "Testing indextest on toscrape at depth 1 file"
./indextest ../tse-output-new/toscrape-index-1 ../tse-output-newtest/toscrape-index-1
if [ $? -ne 0 ]; then
    echo >&2 "indextest on toscrape at depth 1 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output-new/toscrape-index-1) <(sort ../tse-output-newtest/toscrape-index-1)

echo "Testing indextest on toscrape at depth 2 file"
./indextest ../tse-output-new/toscrape-index-2 ../tse-output-newtest/toscrape-index-2
if [ $? -ne 0 ]; then
    echo >&2 "indextest on toscrape at depth 2 failed"
    exit 1
fi

echo "Comparing differences with tse-output"
diff -qy <(sort ../tse-output-new/toscrape-index-2) <(sort ../tse-output-newtest/toscrape-index-2)

echo "================== End of testing.sh =================="

exit 0