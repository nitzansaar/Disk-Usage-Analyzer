# Project 2: Elastic Array and Disk Usage Analyzer
## Building
I made a class called elist.c that mimics an arraylist that you can use in other programming languages such as java. In the da.c class, i made a directory traversal that adds all of the files of a directory to an arraylist. The program is meant to print out the files within a directory plus additional information about their memory usage and last date accessed. Finally, the program takes command line options that allow a user to specify whether to sort by size, time, or even add a limit to the number of files displayed.
## Running + Example Usage
To run this program, you can type: './da directory_name' to the command line. To sort by time you can add the flag -a, to sort by size you can add the flag -s, and to add a limit you can add -l plus an integer to specify the limit. 
## What I learned
- File and Directory Operations: Learned to navigate and access files using dirent.h, interpret file attributes with stat, and handle file types, sizes, and timestamps.
- Command-Line Argument Parsing: Gained experience with getopt for parsing user input, implementing flags to modify program behavior, and validating and converting string arguments.
- Dynamic Memory Allocation: Acquired skills in memory management using malloc, realloc, and free, while preventing memory leaks and handling allocation errors.
- Data Structures and Algorithms: Implemented a dynamic array for flexible data storage, utilized sorting algorithms (qsort) for effective data organization, and designed efficient resizable arrays.
- String Manipulation and Formatting: Practiced string manipulation with functions like strcpy, formatted strings for human-readable output, and created custom functions for converting bytes and timestamps to readable formats.