COMPILER := gcc
FLAGS := -W -Wall -Wextra -Werror -pedantic -std=c23 -pthread
DEBUG_FLAGS := -g -O0
RELEASE_FLAGS := -O3 -DNDEBUG -Wno-unused-but-set-variable -Wno-unused-variable

SRC_DIR := ./src
TARGET_DIR := ../target/simulator
OBJ_DIR := $(TARGET_DIR)/obj
BINARY := $(TARGET_DIR)/main

SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

.PHONY: all
all: debug

.PHONY: debug
debug: FLAGS += $(DEBUG_FLAGS)
debug: $(BINARY)

.PHONY: release
release: FLAGS += $(RELEASE_FLAGS)
release: $(BINARY)

$(BINARY): $(OBJECTS) | $(TARGET_DIR)
	$(COMPILER) $(FLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(COMPILER) $(FLAGS) -c $c $< -o $@

$(TARGET_DIR) $(OBJ_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(TARGET_DIR)
