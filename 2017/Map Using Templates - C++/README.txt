Completed Fall 2017
M. Sheinbaum

Project Description: Implementing a map using templates.
The nodes of the map also contain data to be a doubly linked list to cut down on access time, as the specifications of the program also called for it to include an iterator.

Skills used: Templates, Initializer Lists, Maps, Linked Lists.

Comments: Only compiles with clang++-3.8, not g++. Some of the comments already existed. I'm not sure why it doesn't work with test2 when it works with very other test available.
I left test2 in because while the project is complete, it does have some bugs that are only apparent on that test and I don't know what is causing it.

For test2, the results are variable.
About 40% of the time it segfaults when it tries to get the size of the map.
About 40% of the time it double frees.
About 20% of the time it runs without errors.
As far as I'm aware and after running a lot of tests, it works fine on all of the other test cases.