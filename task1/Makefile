CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -O2
TARGET  := bin/task1
SRC_DIR := src
INC_DIR := include
OBJ_DIR := build

SRCS := $(SRC_DIR)/main.c $(SRC_DIR)/sync.c
OBJS := $(OBJ_DIR)/main.o $(OBJ_DIR)/sync.o

.PHONY: all run clean distclean dirs

all: dirs $(TARGET)

dirs:
	@mkdir -p $(OBJ_DIR) bin

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/sync.h
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

run: all
	@echo "Running with N=3"
	@./$(TARGET) 3

clean:
	@rm -rf $(OBJ_DIR)/*

distclean: clean
	@rm -rf bin
