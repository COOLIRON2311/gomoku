OFLAGS=-O3 -march=native
# OFLAGS=-O0 -g
all: build run

build:
	g++ $(OFLAGS) main.cpp

run:
	./a.exe 0

clean:
	rm a.exe


