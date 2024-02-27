# Makefile for building driver.exe from factorial.o and driver.o

# Compiler
CC = g++

# Compiler flags
CFLAGS = -std=c++20

# Target executable
TARGET = driver.exe

# Source files
SRCS = Tile.cpp driver.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to link the object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

# Run the target file
run: $(TARGET)
	./$(TARGET) $(options)

# Clean target
clean:
	rm -f $(OBJS) $(TARGET)
