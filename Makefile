CC := g++
CFLAGS := -Wall -g

all: memtable

memtable: memtable.cpp
	$(CC) -o memtable memtable.cpp

clean:
	rm -rf *.o memtable
