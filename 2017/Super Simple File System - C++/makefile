all: ssfs ssfs_mkdsk

ssfs: ssfs.o timer.o File.o Disk.o IBP.o DIBP.o
	g++ -std=c++11 *.o -o ssfs -pthread

ssfs_mkdsk: ssfs.o timer.o File.o Disk.o IBP.o DIBP.o
	g++ -std=c++11 *.o -o ssfs_mkdsk -pthread

ssfs.o:  ssfs.cpp timer.h File.h Disk.h IBP.h DIBP.h
	g++ -std=c++11 -c -g  ssfs.cpp -o ssfs.o -pthread

timer.o: timer.cpp timer.h
	g++ -std=c++11 -c -g timer.cpp -o timer.o -pthread

Disk.o: Disk.h Disk.cpp File.h
	g++ -std=c++11 -c -g  Disk.cpp -o Disk.o -pthread

File.o: File.h File.cpp IBP.h DIBP.h
	g++ -std=c++11 -c -g  File.cpp -o File.o -pthread
	
IBP.o: IBP.h IBP.cpp
	g++ -std=c++11 -c -g  IBP.cpp -o IBP.o -pthread
	
DIBP.o: DIBP.h DIBP.cpp IBP.h
	g++ -std=c++11 -c -g  DIBP.cpp -o DIBP.o -pthread

gdb:
	gdb ssfs

run: all
	./ssfs

zip: clean
	zip -r Assignment_5 *

clean: 
	rm -f *o ssfs ssfs_mkdsk *.exe *.exe.stackdump

valgrind: 
	valgrind --leak-check=yes ./ssfs

disk:
	./ssfs_mkdsk 1024 128 disk1

test1:
	./ssfs_mkdsk 1024 128 disk1
	./ssfs disk1 testCases/test.txt
	
	
test2:
	./ssfs disk1 testCases/test2.txt
	
	
test3:
	./ssfs disk1 testCases/test3.txt
	
test4:
	./ssfs_mkdsk 1024 128 disk2
	./ssfs disk2 testCases/test4.txt
	
test5:
	./ssfs_mkdsk 1024 128 disk3
	./ssfs disk3 testCases/test5.txt
	
	
test6:
	./ssfs_mkdsk 1024 128 disk4
	./ssfs disk4 testCases/test6-1.txt testCases/test6-2.txt
	
test7:
	./ssfs_mkdsk 1024 128 disk4
	./ssfs disk4 testCases/test6-1.txt testCases/test6-2.txt testCases/test6-3.txt
	
test8:
	./ssfs_mkdsk 1024 128 disk4
	./ssfs disk4 testCases/test6-1.txt testCases/test6-2.txt testCases/test6-3.txt testCases/test6-4.txt