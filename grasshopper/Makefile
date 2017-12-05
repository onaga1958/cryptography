all: main test

main: grasshopper utils
	g++ main.cpp grasshopper.o utils.o -o main -Wno-write-strings -Ofast

test: grasshopper utils
	g++ test.cpp grasshopper.o utils.o -o test -Wno-write-strings -Ofast

grasshopper:
	g++ -c grasshopper.cpp -o grasshopper.o -Ofast

utils:
	g++ -c utils.cpp -o utils.o

add:
	rm -rf *.o main test
	git add .

clean:
	rm -rf *.o main test
