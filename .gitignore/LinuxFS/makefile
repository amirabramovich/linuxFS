CC= g++
CFLAGS=-g -Wall -Weffc++ -std=c++11
LFLAGS=-L/usr/lib

all: First fs

fs: bin/FileSystem.o bin/Commands.o bin/Main.o bin/Files.o bin/Environment.o 

	$(CC) -o bin/fs bin/FileSystem.o bin/Commands.o bin/Main.o bin/Files.o bin/Environment.o $(LFLAGS)
	@echo -e '\e[105m\033[1;96mFinished building target: fs\033[0m'

bin/Files.o: src/Files.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/Files.o src/Files.cpp

bin/Main.o: src/Main.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/Main.o src/Main.cpp

bin/Environment.o: src/Environment.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/Environment.o src/Environment.cpp

bin/FileSystem.o: src/FileSystem.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/FileSystem.o src/FileSystem.cpp

bin/Commands.o: src/Commands.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/Commands.o src/Commands.cpp
	
First:
	@echo -e '\e[105m\033[1;96mBuilding target: fs\033[0m'

clean:
	rm -f bin/*