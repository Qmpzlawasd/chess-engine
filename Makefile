CC = g++
CFLAGS = -I $(SRC_DIR) -Wall -std=c++23 -pthread

SRC_DIR = src
BUILD_DIR = build

TARGET = main

ENGINE_DEPS = $(wildcard $(SRC_DIR)/*.h)
ENGINE_SRCS = $(wildcard $(SRC_DIR)/*.cpp)

.PHONY: pre-build all engine

all: pre-build engine run

pre-build:
	mkdir -p $(BUILD_DIR)

engine: $(ENGINE_SRCS) $(ENGINE_DEPS) pre-build
	$(CC) -o $(BUILD_DIR)/$(TARGET) $(ENGINE_SRCS) $(CFLAGS)

run:
	./$(BUILD_DIR)/$(TARGET)
