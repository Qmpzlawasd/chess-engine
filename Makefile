CC = g++
CFLAGS = -I $(SRC_DIR) -Wall -std=c++23 -pthread -Weffc++ -march=native -g
CWARNING = -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused


SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests

TARGET = main

ENGINE_DEPS = $(wildcard $(SRC_DIR)/*.h)
ENGINE_SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/Piece/*.cpp)  $(wildcard $(SRC_DIR)/Move/*.cpp) $(wildcard $(SRC_DIR)/Board/*.cpp)

.PHONY: pre-build all engine engine-debug run

warning: pre-build engine-debug run

all: pre-build engine run

pre-build:
	mkdir -p $(BUILD_DIR)

engine: $(ENGINE_SRCS) $(ENGINE_DEPS) pre-build
	$(CC) -o $(BUILD_DIR)/$(TARGET) $(ENGINE_SRCS) $(CFLAGS)

engine-debug: $(ENGINE_SRCS) $(ENGINE_DEPS) pre-build
	$(CC) -o $(BUILD_DIR)/$(TARGET) $(ENGINE_SRCS) $(CFLAGS) $(CWARNING)

run:
	./$(BUILD_DIR)/$(TARGET) "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 10 10" 2

