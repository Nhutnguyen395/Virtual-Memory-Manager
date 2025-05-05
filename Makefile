CC:=gcc
CFLAGS:=-g
SRC_DIR:=src
BIN_DIR:=bin
INCLUDE_DIR:=include

all: setup manager

manager: src/helpers.c src/manager.c
	$(CC) -I $(INCLUDE_DIR) $(CFLAGS) $(SRC_DIR)/helpers.c $(SRC_DIR)/manager.c -o $(BIN_DIR)/manager.exe

setup:
	-mkdir $(BIN_DIR)

clean:
	-rmdir /s /q $(BIN_DIR)

.PHONY: clean all setup