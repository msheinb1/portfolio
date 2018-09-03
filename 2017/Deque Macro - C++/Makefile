all: test

test: test.o
	g++ -std=c++11 -g test.o -o test -ldl

test.o: test.1.cpp Deque.hpp
	g++ -std=c++11 -Wall -Wextra -pedantic -c -g new_test.cpp -o test.o -ldl

clean:
	rm -f *o test *.exe *.exe.stackdump
	
gdb: all
	gdb test
	
run: all
	./test
	
valgrind: all
	valgrind -v test