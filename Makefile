CC = gcc
CFLAGS = -Wall -Wextra -pthread -I./utils

BUILD_DIR = build

QUEUE_SRC = I-concurrency/iii-lock-based-concurrent-data-structures/queue
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
	$(QUEUE_SRC)/concurrent-queue.c \
	$(QUEUE_SRC)/performance.c \
	$(UTILS_OBJ) \
	-o $(QUEUE_BUILD).out

# Clean
clean:
	rm -rf build *.out