# Makefile
# By: Luke Cotton
# Makefile for FileCopyServer.

# C++ Compiler
CXX=g++

# All target.
all: FileCopyServer

# Link program.
FileCopyServer: main.o Server.o threadmain.o Connection.o
	$(CXX) -o FileCopyServer main.o threadmain.o Server.o Connection.o -lpthread

# Compiles main.o.
main.o: main.cpp
	$(CXX) -c -o main.o main.cpp

# Compiles threadmain.o.
threadmain.o: threadmain.cpp
	$(CXX) -c -o threadmain.o threadmain.cpp

# Compiles Server.o.
Server.o: Server.cpp
	$(CXX) -c -o Server.o Server.cpp

# Compiles Connection.o.
Connection.o: Connection.cpp
	$(CXX) -c -o Connection.o Connection.cpp

# Cleans FileCopyServer.
clean:
	rm -f *.o FileCopyServer file1.txt

# Runs FileCopyServer.
run: FileCopyServer
	./FileCopyServer ./
