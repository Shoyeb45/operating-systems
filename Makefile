CC = gcc
CFLAGS = -Wall -Wextra -pthread -I./utils
CCPP = g++
CPP_VERSION = --std=c++17
BUILD_DIR = build

DATA_STRUCTURE_SRC = I-concurrency/iii-lock-based-concurrent-data-structures
QUEUE_BUILD = $(BUILD_DIR)/queue

UTILS_SRC = utils/utils.c
UTILS_OBJ = $(BUILD_DIR)/utils.o

# Create build directory
dirs:
	mkdir -p $(BUILD_DIR)

# Compile utils once
$(UTILS_OBJ): $(UTILS_SRC) | dirs
	$(CC) $(CFLAGS) -c $(UTILS_SRC) -o $(UTILS_OBJ)

# Build queue
queue: $(UTILS_OBJ)
	$(CC) $(CFLAGS) \
	$(DATA_STRUCTURE_SRC)/queue/concurrent-queue.c \
	$(DATA_STRUCTURE_SRC)/queue/performance.c \
	-o $(QUEUE_BUILD).out

hash-map: $(UTILS_OBJ)
	$(CCPP) $(CPP_VERSION) \
	$(DATA_STRUCTURE_SRC)/hash-map/performance.cpp  \
	$(UTILS_OBJ) \
	-o $(BUILD_DIR)/hash-map.out


# Clean
clean:
	rm -rf build *.out