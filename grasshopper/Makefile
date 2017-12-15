FLAGS=-Wno-write-strings -Ofast

all: main test

main: grasshopper utils
	g++ $(FLAGS) main.cpp grasshopper.o utils.o -o main

test: grasshopper utils
	g++ $(FLAGS) test.cpp grasshopper.o utils.o -o test

grasshopper:
	g++ $(FLAGS) -c grasshopper.cpp -o grasshopper.o

utils:
	g++ -c utils.cpp -o utils.o

clean:
	rm -rf *.o main test
