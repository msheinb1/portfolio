all: clean test

test: test.o memory_manager.a
	g++ -std=c++11 -g test.o memory_manager.a -o test

test.o: memory_manager.h interposition.h
	g++ -std=c++11 -c -g test_main.c -o test.o
	
bitmap.o: common.h
	g++ -std=c++11 -c -g bitmap.c -o bitmap.o
	
memory_manager.o: memory_manager.h common.h
	g++ -std=c++11 -c -g memory_manager.c -o memory_manager.o
	
memory_manager.a: memory_manager.o bitmap.o
	ar rcs memory_manager.a memory_manager.o bitmap.o
	ranlib memory_manager.a

clean:
	rm -f *o test *.exe *.exe.stackdump *.a
	
gdb: all
	gdb test
	
run: all
	./test
	
valgrind: all
	valgrind -v test