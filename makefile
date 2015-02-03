CC = gcc
CXX = g++

all:

OBJS=flakor.o 


%.o : % .cpp

clean:
    rm -rf *.o
