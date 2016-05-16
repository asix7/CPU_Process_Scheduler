# 
# Makefile for project 1 (COMP30017)
#

## CC  = Compiler.
## CFLAGS = Compiler flags.
CC  = gcc
CFLAGS  = -g -Wall


## OBJ = Object files.
## SRC = Source files.
## EXE = Executable name.

SRC =		process_scheduler.c algorithms.c linked_list.c memory.c
OBJ =		process_scheduler.o algorithms.o linked_list.o memory.o
EXE =		simulation

## Top level target is executable.
$(EXE): $(OBJ)
		$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -lm


## Clean: Remove object files and core dump files.
clean:
		/bin/rm $(OBJ) 

## Clobber: Performs Clean and removes executable file.

clobber: clean
		/bin/rm $(EXE) 

## Dependencies

scheduler.o:	algorithms.h linked_list.h
parser.o:		algorithms.h linked_list.h memory.h
linked_list.o:	linked_list.h
memory.o:		memory.h
