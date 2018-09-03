all: prog1generator prog1sorter

prog1generator: main.o Generate.o
	g++ -std=c++11 main.o Generate.o -o prog1generator

main.o: main.cpp Timer.h
	g++ -std=c++11 -c -g main.cpp -o main.o

Timer.o: Timer.cpp Timer.h
	g++ -std=c++11 -c -g Timer.cpp -o Timer.o

Generate.o: Generate.cpp Generate.h
	g++ -std=c++11 -c -g Generate.cpp -o Generate.o
	
prog1sorter: Timer.o sorterMain.o Sorter.o
	g++ -std=c++11 Timer.o sorterMain.o Sorter.o -o prog1sorter

sorterMain.o: sorterMain.cpp Sorter.h
	g++ -std=c++11 -c -g sorterMain.cpp -o sorterMain.o

Sorter.o: Sorter.cpp Sorter.h
	g++ -std=c++11 -c -g Sorter.cpp -o Sorter.o

clean:
	rm -f *.o prog1generator.* prog1sorter.* *.txt

zip: clean
	zip -r generator *
