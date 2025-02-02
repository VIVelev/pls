CC = clang

CFLAGS = -Wall -Wextra -Werror -Wno-gnu -pedantic-errors -std=c2x -O2

SRC_DIR = src

C_SOURCES = $(shell find $(SRC_DIR) ! -name "main.c" -name "*.c")
HEADERS = $(shell find $(SRC_DIR) -name "*.h")
OBJ = $(C_SOURCES:.c=.o)

pls: $(SRC_DIR)/main.c $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Generic rule for wildcards
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	find . -name "*.o" -type f -delete
	find . -name "*.out" -type f -delete
	rm pls
