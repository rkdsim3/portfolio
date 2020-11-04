all: program1 program2 program3

program1: program1.cpp
	g++ -o program1 program1.cpp -std=c++11

program2: program2.cpp
	g++ -o program2 program2.cpp -std=c++11

program3: program3.cpp
	g++ -o program3 program3.cpp -pthread -std=c++11

clean:
	rm -f program1 program2 program3 program1.o program2.o program3.o


#all: program1 program2 program3
#
#program1: program1.o
#	gcc -o program1 program1.o
#
#program2: program2.o
#	gcc -o program2 program2.o
#
#program3: program3.o
#	gcc -o program3 program3.o
#	
#program1.o: program1.c
#	gcc -c program1.c
#
#program2.o: program2.c
#	gcc -c program2.c
#
#program3.o: program3.c
#	gcc -c program3.c
#
#clean:
#	rm -f program1 program2 program3 program1.o program2.o program3.o
