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
BINS=$(patsubst %.o,%,$(OBJS))


shell.o: $(HEADERS_DIR)/shell.h shell.c 
	$(CC) -I $(HEADERS_DIR) -c shell.c

$(SHELL_NAME): shell.o
	$(CC) -o $(SHELL_NAME) shell.o

dircreate.o: $(HEADERS_DIR)/command.h $(SRC_DIR)/dircreate.c
	$(CC) -I $(HEADERS_DIR) -c $(SRC_DIR)/dircreate.c

build: $(OBJS) $(SHELL_NAME)
	mkdir $(BIN_DIR)
	$(CC) -o $(BINS) $(BINS).o
	mv $(BINS) $(BIN_DIR)

.PHONY: clean

clean:
	rm -Rf $(BIN_DIR) *.o $(SHELL_NAME) *.out 


	
