# Project 2: Elastic Array and Disk Usage Analyzer
## Building
I made a class called elist.c that mimics an arraylist that you can use in other programming languages such as java. In the da.c class, i made a directory traversal that adds all of the files of a directory to an arraylist. The program is meant to print out the files within a directory plus additional information about their memory usage and last date accessed. Finally, the program takes command line options that allow a user to specify whether to sort by size, time, or even add a limit to the number of files displayed.
## Running + Example Usage
To run this program, you can type: './da directory_name' to the command line. To sort by time you can add the flag -a, to sort by size you can add the flag -s, and to add a limit you can add -l plus an integer to specify the limit. 
