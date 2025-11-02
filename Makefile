# Makefile - Modular build for Satori

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -Isrc
LDFLAGS = -lm

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/satori

# Source files by module
CORE_SRCS = $(SRC_DIR)/core/value.c $(SRC_DIR)/core/object.c $(SRC_DIR)/core/memory.c
FRONTEND_SRCS = $(SRC_DIR)/frontend/lexer.c $(SRC_DIR)/frontend/parser.c $(SRC_DIR)/frontend/ast.c $(SRC_DIR)/frontend/typechecker.c
BACKEND_SRCS = $(SRC_DIR)/backend/codegen.c
RUNTIME_SRCS = $(SRC_DIR)/runtime/vm.c
ERROR_SRCS = $(SRC_DIR)/error/error.c
MAIN_SRC = $(SRC_DIR)/main.c

SRCS = $(CORE_SRCS) $(FRONTEND_SRCS) $(BACKEND_SRCS) $(RUNTIME_SRCS) $(ERROR_SRCS) $(MAIN_SRC)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG
CFLAGS += $(RELEASE_FLAGS)

all: $(TARGET)

debug: CFLAGS = -Wall -Wextra -std=c99 -pedantic -Isrc $(DEBUG_FLAGS)
debug: clean $(TARGET)

release: CFLAGS = -Wall -Wextra -std=c99 -pedantic -Isrc $(RELEASE_FLAGS)
release: clean $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/core $(BUILD_DIR)/frontend $(BUILD_DIR)/backend $(BUILD_DIR)/runtime $(BUILD_DIR)/error

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	@echo "Built: $(TARGET)"

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/satori

uninstall:
	rm -f /usr/local/bin/satori

test-lexer: $(TARGET)
	./$(TARGET) -t examples/hello.sat

run-hello: $(TARGET)
	./$(TARGET) examples/hello.sat

.PHONY: all debug release clean install uninstall test-lexer run-hello
