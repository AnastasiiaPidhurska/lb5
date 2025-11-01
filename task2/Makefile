CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -O2
TARGET  := bin/task2
SRC_DIR := src
INC_DIR := include
OBJ_DIR := build

SRCS := $(SRC_DIR)/main.c $(SRC_DIR)/duplex.c
OBJS := $(OBJ_DIR)/main.o $(OBJ_DIR)/duplex.o

.PHONY: all run clean distclean dirs

all: dirs $(TARGET)

dirs:
	@mkdir -p $(OBJ_DIR) bin

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/duplex.h
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

run: all
	@./$(TARGET)

clean:
	@rm -rf $(OBJ_DIR)/*

distclean: clean
	@rm -rf bin
