all: prog4pageReplacement

prog4pageReplacement: main.o Generate.o LinkedList.o
	g++ -std=c++11 main.o Generate.o LinkedList.o -o prog4pageReplacement

main.o: main.cpp LinkedList.h Generate.h
	g++ -std=c++11 -c -g main.cpp -o main.o

Generate.o: Generate.cpp Generate.h
	g++ -std=c++11 -c -g Generate.cpp -o Generate.o

LinkedList.o: LinkedList.cpp LinkedList.h
	g++ -std=c++11 -c -g LinkedList.cpp -o LinkedList.o

clean:
	rm -f *.o prog4pageReplacement

test: all
	./prog4pageReplacement

gdb: all
	gdb prog4pageReplacement
