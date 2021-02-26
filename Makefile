# The C compiler
CC=gcc

# Compiler flags
C_FLAGS= -Wall

# Name of the shell
SHELL_NAME=ramboso_xterm

HEADERS_DIR := ./include
SRC_DIR     := ./src
BIN_DIR     := ./bin

OBJS=dircreate.o

shell.o: $(HEADERS_DIR)/shell.h shell.c 
	$(CC) -I $(HEADERS_DIR) -c shell.c

$(SHELL_NAME): shell.o
	$(CC) -o $(SHELL_NAME) shell.o

dircreate.o: $(HEADERS_DIR)/command.h $(SRC_DIR)/dircreate.c
	$(CC) -I $(HEADERS_DIR) -c $(SRC_DIR)/dircreate.c


.PHONY: clean

clean:
	rm -f *.o $(SHELL_NAME) *.out

build:
	mkdir $(BIN_DIR)
	
