CC = gcc
CFLAGS = -I/usr/local/include/SDL2 -Ithird-party/tomlc99 -Ithird-party -I. -g -Wall -Wextra -O2
LDFLAGS = -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lopenal -lcurl -lm

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

TOMLC99_DIR = third-party/tomlc99
TOMLC99_SRCS = $(TOMLC99_DIR)/toml.c
TOMLC99_OBJS = $(patsubst $(TOMLC99_DIR)/%.c, $(OBJ_DIR)/%.o, $(TOMLC99_SRCS))

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS)) $(TOMLC99_OBJS)
TARGET = $(BIN_DIR)/interactive_fiction_engine

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< to $@"

$(OBJ_DIR)/%.o: $(TOMLC99_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< to $@"

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleaned up build files"

.PHONY: all clean
