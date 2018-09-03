Completed Spring 2017
M. Sheinbaum + 3 other team members

Project Description: Created a Super Simple File System using inode pointer structure.

Skills Used: Queues, Producer/Consumer Problem

Comments: Worked on this project with 3 other team members. It was alright, but frustrating at times.
For this program, dbps (all lowercase) stands for "direct block pointers" and DBPS (all uppercase) stands for "double indirect block pointers."
Probably could streamline the program more by having a method for whenever you wanted to allocate space for a file.

Use:
Make the disk with:
<filepath>ssfs_mkdsk <number of blocks> <block size> <diskname>

Block size must be a number between 128 to 512
Number of blocks must be a number between 1024 and 128000
Both numbers must be a power of 2.
diskname is an optional field - if not included, the new disk is named DISK

Perform prepared tests on the disk with:
<filepath>ssfs <diskname> testCases/<testcase name>

Available commands (case sensitive)

CREATE <filename>: Reserves the name of a new file
IMPORT <filename> <unix filename>: Imports the contents of a unix file to a file for the program.
CAT <filename>: Prints all of the contents of the file to the terminal
DELETE <filename>: Deletes a file and frees all its associated space.
WRITE <filename> <char> <startByte> <numBytes>: Writes <numBytes> number of <char> starting at position <startByte>.
READ <filename> <startByte> <numBytes>: Reads <numBytes> number of bytes starting at position <startByte>.
LIST: Lists the files on the disk, prints number of files to terminal
SHUTDOWN: Writes the data in the program to the disk.
