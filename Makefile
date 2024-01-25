all: build run clean

build:
	g++ src/main.cpp -std=c++2a -o main

run:
	/src/main

clean:
	rm src/main