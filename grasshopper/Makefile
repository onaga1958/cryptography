all: main test

main: grasshopper
	g++ main.cpp grasshopper.o -o main

test: grasshopper
	g++ test.cpp grasshopper.o -o test -Wno-write-strings

grasshopper:
	g++ -c grasshopper.cpp -o grasshopper.o

clean:
	rm -rf *.o main test
