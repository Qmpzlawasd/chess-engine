CC = g++
CFLAGS = -I $(SRC_DIR) -Wall -std=c++23 -pthread -Weffc++ -march=native -Ofast
CWARNING = -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused

SRC_DIR = src
BUILD_DIR = build

TARGET = main

ENGINE_DEPS = $(wildcard $(SRC_DIR)/*.h)
ENGINE_SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/Piece/*.cpp)  $(wildcard $(SRC_DIR)/Move/*.cpp) $(wildcard $(SRC_DIR)/Board/*.cpp) $(wildcard $(SRC_DIR)/PositionHash/*.cpp) $(wildcard $(SRC_DIR)/Game/*.cpp)  $(wildcard $(SRC_DIR)/Perft/*.cpp)  $(wildcard $(SRC_DIR)/MagicBitboards/*.cpp) $(wildcard $(SRC_DIR)/FEN/*.cpp) $(wildcard $(SRC_DIR)/NNUE/*.cpp)

.PHONY: pre-build all engine engine-debug run test

warning: pre-build engine-debug run

all: pre-build engine run

pre-build:
	mkdir -p $(BUILD_DIR)

engine: $(ENGINE_SRCS) $(ENGINE_DEPS) pre-build
	$(CC) -o $(BUILD_DIR)/$(TARGET) $(ENGINE_SRCS) $(CFLAGS)

engine-debug: $(ENGINE_SRCS) $(ENGINE_DEPS) pre-build
	$(CC) -o $(BUILD_DIR)/$(TARGET) $(ENGINE_SRCS) $(CFLAGS) $(CWARNING)

test:
	chmod +x $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET) "test"

run:
	echo ./$(BUILD_DIR)/$(TARGET)

