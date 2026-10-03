CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11 -g
SAN_FLAGS = -O1 -fno-omit-frame-pointer -fsanitize=address,undefined -fno-sanitize-recover=all
SRC = src/main.c
BIN = bin/laboratorio01
DEBUG_BIN = bin/laboratorio01-debug

.PHONY: all run debug clean

all: $(BIN)

$(BIN): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

run: all
	@./$(BIN)

# Build instrumentado: AddressSanitizer (overflow, use-after-free, leaks)
# + UndefinedBehaviorSanitizer (overflow de inteiro, ponteiro desalinhado, etc.)
$(DEBUG_BIN): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SAN_FLAGS) $(SRC) -o $(DEBUG_BIN)

debug: $(DEBUG_BIN)
	@./$(DEBUG_BIN)

clean:
	rm -rf bin
