#!/bin/bash
# testing.sh -- testing file for querier
# Usage: make test
# Input: none
# Output: Testing results, see testing.out for more specific results
#
# Irene Lam, August 15, 2020
# Dartmouth CS50, Summer 2020

make
mkdir -p ../querygarbage

echo "Integration testing for querier module."
echo "PLEASE ensure you have tse-output downloading before running the below script."
echo "Note that errors were printed to stderr, while normal output was printed to stdout."
echo "Certain messages may not align for this reason."
echo "For details on how to download tse-output, please see the general README."
echo "Please use valgrind to check for memory leaks"

echo "=========================================================="

echo "Testing: Parameter Checks"

echo "Error-handling: Insufficient arguments"
./querier ../tse-output/letters-depth-6 < BASIC_TEST
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Insufficient arguments provided"
fi

echo "Error-handling: Too many arguments"
./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6 ../tse-output/ < BASIC_TEST
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Too many arguments provided"
fi

echo "Error-handling: Testing querier on invalid pagedirectory (not crawler-produced)"
./querier ../querygarbage ../tse-output/letters-index-6 < BASIC_TEST
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Indexer requires a crawler-produced directory"
fi

echo "Error-handling: Testing querier on invalid pagedirectory (non-existent)"
./querier ../tse-outputt/ ../tse-output/letters-index-6 < BASIC_TEST
if [ $? -eq 0 ]; then
    echo >&2 "Error caught: querier ignores a non-existent pagedirectory"
fi

echo "Error-handling: Testing querier on crawler directory with a backslash"
./querier ../tse-output/letters-depth-6/ ../tse-output/letters-index-6 < BASIC_TEST 
if [ $? -eq 0 ]; then
    echo >&2 "Querier continues if there already is a backslash"
fi

echo "Error-handling: Testing querier on crawler directory without a backslash"
./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6 < BASIC_TEST 
if [ $? -eq 0 ]; then
    echo >&2 "Querier adds a backslash if doesn't already exist"
fi

mkdir ../queryWithoutRead
chmod -r ../queryWithoutRead

echo "Error-handling: Testing querier on invalid directory (no read permission)"
./querier ../tse-output/letters-depth-6 ../queryWithoutRead
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Directory must be readable"
fi

chmod +r ../queryWithoutRead

echo "=========================================================="
echo "Testing querier on edge cases for letters at depth 6" 

echo "Testing querier on empty query"
echo "" | ./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does nothing and continues on empty query"
fi

echo "Testing querier on whitespace"
echo "   " | ./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does nothing and continues on query of only whitespace"
fi

echo "Testing querier with a starting operator"
echo "AND playground" | ./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow AND to be first in the query"
fi

echo "OR playground" | ./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow OR to be first in the query"
fi

echo "Testing querier with an ending operator"
echo "playground AND" | ./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow AND to be last in the query"
fi

echo "playground OR" | ./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow OR to be last in the query"
fi

echo "Testing querier with consecutive operators"
echo "playground AND OR playground" | ./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow AND and OR to be consecutive"
fi

echo "playground AND AND playground" | ./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow AND and AND to be consecutive"
fi

echo "playground OR OR playground" | ./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow OR and OR to be consecutive"
fi

echo "playground OR AND playground" | ./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow OR and AND to be consecutive"
fi

echo "Testing querier on words not in file"
./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6 < MISSING_WORDS
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier returns 'No documents match.' for words it cannot find."
fi

echo "Testing querier on non-letter characters"
./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6 < NON_LETTERS
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow non-letter characters."
fi

echo "=========================================================="
echo "Basic querier tests on letters at depth 6"

echo "Testing querier on one word" 
./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6 < ONE_WORD_TEST
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed for one word test."
    exit 1
fi

echo "Testing querier on basic conjunctions" 
./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6 < BASIC_CONJUNCTION
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed for basic conjunction test."
    exit 1
fi

echo "Testing conjunction precedence" 
./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6 < PRECEDENCE_TEST
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed for conjunction precedence test."
    exit 1
fi

echo "Testing querier with fuzzquery" 
./fuzzquery ../tse-output/letters-index-6 10 2 | ./querier ../tse-output/letters-depth-6 ../tse-output/letters-index-6 
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed for conjunction precedence test."
    exit 1
fi

echo "Manually checked that all of the above results produce the expected results"

echo "=========================================================="
echo "Querier tests on toscrape at depth 0, 1, 2"

echo "Testing querier on toscrape at depth 0" 
./querier ../tse-output/toscrape-depth-0 ../tse-output/toscrape-index-0 < TOSCRAPE_TEST
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on Toscrape at depth 0."
    exit 1
fi

echo "Testing querier on toscrape at depth 1" 
./querier ../tse-output/toscrape-depth-1 ../tse-output/toscrape-index-1 < TOSCRAPE_TEST
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on Toscrape at depth 1."
    exit 1
fi

echo "Testing querier on toscrape at depth 2" 
./querier ../tse-output/toscrape-depth-2 ../tse-output/toscrape-index-2 < TOSCRAPE_TEST
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on Toscrape at depth 2."
    exit 1
fi

echo "Manually checked that all of the above results produce the expected results"

echo "=========================================================="
echo "Querier tests on wikipedia at depth 0, 1, 2"

echo "Testing querier on wikipedia at depth 0" 
./querier ../tse-output/wikipedia-depth-0 ../tse-output/wikipedia-index-0 < WIKIPEDIA_TEST
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on wikipedia at depth 0."
    exit 1
fi

echo "Testing querier on wikipedia at depth 1" 
./querier ../tse-output/wikipedia-depth-1 ../tse-output/wikipedia-index-1 < WIKIPEDIA_TEST
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on wikipedia at depth 1."
    exit 1
fi

echo "Testing querier on wikipedia at depth 2" 
./querier ../tse-output/wikipedia-depth-2 ../tse-output/wikipedia-index-2 < WIKIPEDIA_TEST
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on wikipedia at depth 2."
    exit 1
fi

echo "Manually checked that all of the above results produce the expected results"

echo "=========================================================="
echo "Fuzz tests on miscellaneous files"

echo "Fuzz tests on letters at depth 2" 
./fuzzquery ../tse-output/letters-index-2 5 3 | ./querier ../tse-output/letters-depth-2 ../tse-output/letters-index-2 
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on fuzz tests for letters at depth 2."
    exit 1
fi

echo "Fuzz tests on toscrape at depth 2" 
./fuzzquery ../tse-output/toscrape-index-2 5 3 | ./querier ../tse-output/toscrape-depth-2 ../tse-output/toscrape-index-2 
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on fuzz tests for toscrape at depth 2."
    exit 1
fi

echo "Fuzz tests on wikipedia at depth 0" 
./fuzzquery ../tse-output/wikipedia-index-0 5 3 | ./querier ../tse-output/wikipedia-depth-0 ../tse-output/wikipedia-index-0 
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on fuzz tests for wikipedia at depth 0."
    exit 1
fi

echo "================== End of testing.sh =================="

exit 0