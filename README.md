# Search tree
This is a C++ implementation of constructing a search tree from a table. It is probably very inefficient due to its heavy use of vectors and it will crash when a leaf doesn't have a Gini coefficient of 0. Still, if you can use this code and make something out of it, go your gang. 

## Running the code
Currently, the code does not output anything of use. To get output, a breakpoint should be placed on `return 0` in `main()`, so the program output can be read in the debugger (the variable "splits"). If the code crashes because it cannot find the file, change `../illness.txt` in `main()` to the path of your file.

## File format
The program reads a file specified in `main.cpp` and constructs a search tree based on the data in the file.
The file format is as follows:
the first row consists of table headers, which are used to identify the attributes below.
Each header name is delimited by a tab.
Following the headers, the attributes should be listed, also tab-delimited. 
The last column consists of outcomes. See `weekend.txt` and `illness.txt` as examples.