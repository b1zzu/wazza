
# Define the compiler as args so that it can be overwritten if needed.
CC = gcc

# -Wall		prints all warning messages
# -g		generates debug symbols for gdb
CFLAGS = -Wall -g

# The list of source files to compile into object files.
SRCS = \
	src/server.c

# The list of object files to compile into the final executable.
OBJS = $(SRCS:.c=.o) # susbitute .c with .o

# The list of source test files to compile into object files.
TEST_SRCS = \
	test/server_test.c

# The list of object files to compile into the test executable with the source objects.
TEST_OBJS = $(TEST_SRCS:.c=.o)

# The final destination for the executable.
TARGET = wazza

all: $(TARGET)

# Compile the final executable using all previously build object files.
# $@ = wazza
# $^ = $(OBJS) src/main.c
$(TARGET): $(OBJS) src/main.c
	$(CC) $(CFLAGS) -o $@ $^
	
$(TARGET)_test: $(OBJS) $(TEST_OBJS) test/main.c
	$(CC) $(CFLAGS) -lcheck -o $@ $^

check: $(TARGET)_test
	./$(TARGET)_test

clean:
	rm -f $(OBJS) $(TARGET)
