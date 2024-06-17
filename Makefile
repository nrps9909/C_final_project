CC = gcc
CFLAGS = -I/usr/local/include/SDL2 -Ithird-party/tomlc99 -I/usr/local/include -I. -g -Wall -Wextra -O2
LDFLAGS = -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lcurl -lm -lopenal

SRCS = src/game_engine.c src/main.c src/script_parser.c src/sound_manager.c src/ui_gui.c third-party/tomlc99/toml.c
OBJS = $(SRCS:.c=.o)
BIN = bin/interactive_fiction_engine

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	@mkdir -p bin
	$(CC) $(OBJS) -o $(BIN) $(LDFLAGS)

%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BIN)
	rm -rf bin
	rm -rf src/*.o
	rm -rf third-party/tomlc99/*.o
