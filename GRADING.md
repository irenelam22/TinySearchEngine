### GRADING.md for CS 50 Lab 6
### Author: Namya Malik

#### Makefiles 11.5/12 pts
* -0.5 pts: Makefile error when user does `make clean` in the `querier` subdirectory or in the top-level directory right after doing `make` (without running test script)
* `querier` and `fuzzquery` compile from top-level. No warnings. Good.

#### Git repo requirements 3/3 pts
* Proper file structure. Good.
* Use of `.gitignore`. Good.
* Git repo does not track any data files, core dumps or compiled objects. Good.

#### Documentation 15/15 pts
* Great comprehensive documenation!
* Great comprehensive testing script!

#### Coding Style 20/20 pts
* Good comments
* Good modular functions
* Good use of assertp()
* You don't really need a header file for `querier` since the functions in `querier.c` were local to that file. If those functions were going to be used in other source files (for example the functions in your `common` directory, then you would need a header file).

#### Functionality 33/40 pts
* Good validation of arguments
* Ranking of documents by score is good
* There is an issue with your "and" sequence. It does not perform the intersection. Incorrect output for 7 out of the 9 test cases with the following arguments: `./querier ~cs50/data/tse-output/toscrape-depth-1/ ~cs50/data/tse-output/toscrape-index-1`. The following queries produced incorrect outputs:
	* -1 pt: `book and history`. Expected output = 7 documents
	* -1 pt: `art price`. Expected output = 2 documents
	* -1 pt: `music band or painting`. Expected output = 5 documents
	* -1 pt: `modern music or classical poem`. Expected output = 1 document
	* -1 pt: `political nonfiction or modern art or humor story`. Expected output = 6 documents
	* -1 pt: `eat fat get thin or the time keeper or the black maria`. Expected output = 12 documents
	* -1 pt: `paris kitchen recipes baking or white cat bear whale or best reviews`. Expected output = 6 documents

#### Memory leaks 9/10 pts
* -1 pt: `./querier /net/ifs-users/cs50/data/tse-output/letters-depth-6/ ../index` produces a memory leak. You forgot to free `dircopy` if inputCheck() is false.

#### Other Comments
* None

Total score for lab 6: 91.5/100 pts
