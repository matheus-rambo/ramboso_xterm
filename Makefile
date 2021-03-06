# The C compiler
CC=gcc

# Compiler flags
C_FLAGS= -Wall

# Name of the shell
SHELL_NAME=ramboso_xterm

HEADERS_DIR := ./include
SRC_DIR     := ./src
BUILD_DIR   := ./build
BIN_DIR     := $(BUILD_DIR)/bin

# Object files
OBJS=createdir.o   \
	 currentdir.o  \
	 showdir.o     \
	 system-info.o \
	 readline.o    \
	 shell.o

BINS=createdir   \
	 currentdir  \
     showdir     \
     system-info

$(SHELL_NAME): shell.o
	$(CC) -o  $(BUILD_DIR)/$(SHELL_NAME) $(BUILD_DIR)/shell.o

shell.o: $(HEADERS_DIR)/shell.h shell.c 
	$(CC) -I $(HEADERS_DIR) -c -o $(BUILD_DIR)/shell.o shell.c

createdir.o: $(HEADERS_DIR)/command.h $(HEADERS_DIR)/createdir.h $(SRC_DIR)/createdir.c
	$(CC) -I $(HEADERS_DIR) -c -o $(BUILD_DIR)/createdir.o $(SRC_DIR)/createdir.c

currentdir.o: $(HEADERS_DIR)/command.h $(SRC_DIR)/currentdir.c
	$(CC) -I $(HEADERS_DIR) -c -o $(BUILD_DIR)/currentdir.o $(SRC_DIR)/currentdir.c

showdir.o: $(HEADERS_DIR)/command.h $(SRC_DIR)/showdir.c
	$(CC) -I $(HEADERS_DIR) -c -o $(BUILD_DIR)/showdir.o $(SRC_DIR)/showdir.c

system-info.o: $(HEADERS_DIR)/command.h $(HEADERS_DIR)/system-info.h $(SRC_DIR)/system-info.c
	$(CC) -I $(HEADERS_DIR) -c -o $(BUILD_DIR)/system-info.o $(SRC_DIR)/system-info.c

readline.o: $(HEADERS_DIR)/readline.h $(SRC_DIR)/readline.c
	$(CC) -I $(HEADERS_DIR) -c -o $(BUILD_DIR)/readline.o $(SRC_DIR)/readline.c

install: $(OBJS) $(SHELL_NAME)
	$(foreach bin, $(BINS), $(CC) -o $(BIN_DIR)/$(bin) $(BUILD_DIR)/$(bin).o;)

build: clean
	mkdir -p $(BIN_DIR)

.PHONY: clean run

clean:
	rm -Rf $(BUILD_DIR) 2>>/dev/null

run:
	$(BUILD_DIR)/./$(SHELL_NAME) $(BIN_DIR)/


	
