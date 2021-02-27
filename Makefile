# The C compiler
CC=gcc

# Compiler flags
C_FLAGS= -Wall

# Name of the shell
SHELL_NAME=ramboso_xterm

HEADERS_DIR := ./include
SRC_DIR     := ./src
BIN_DIR     := ./bin

OBJS=createdir.o currentdir.o
BINS=$(patsubst %.o,%,$(OBJS))


shell.o: $(HEADERS_DIR)/shell.h shell.c 
	$(CC) -I $(HEADERS_DIR) -c shell.c

$(SHELL_NAME): shell.o
	$(CC) -o $(SHELL_NAME) shell.o

createdir.o: $(HEADERS_DIR)/command.h $(SRC_DIR)/createdir.c
	$(CC) -I $(HEADERS_DIR) -c $(SRC_DIR)/createdir.c

currentdir.o: $(HEADERS_DIR)/command.h $(SRC_DIR)/currentdir.c
	$(CC) -I $(HEADERS_DIR) -c $(SRC_DIR)/currentdir.c


build: clean $(OBJS) $(SHELL_NAME)
	mkdir $(BIN_DIR)
	$(foreach bin, $(BINS), $(CC) -o $(bin) $(bin).o;)
	mv $(BINS) $(BIN_DIR)

.PHONY: clean run

clean:
	rm -Rf $(BIN_DIR) *.o $(SHELL_NAME) *.out 

run: 
	./$(SHELL_NAME) ./bin/

	
