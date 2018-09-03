all: clean test
#CC=g++
CC=clang++-3.8

test: test.o
	$(CC) -std=c++11 -g test.o -o test -ldl

test.o: test1.1.cpp Map.hpp
	$(CC) -std=c++11 -c -g exhaustive_test.cpp -o test.o -ldl
	
run: all
	./test
	
clean:
	rm -f *o test *.exe *.exe.stackdump
	
gdb: all
	gdb test
	
valgrind: all
	valgrind ./test
	
leak: all
	valgrind --leak-check=full ./test

leak_status: all
	valgrind --leak-check=full --show-leak-kinds=all ./test
