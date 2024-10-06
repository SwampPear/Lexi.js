CC = g++

TARGET = lexi
SRC_PATH = src
SUB_DIR = src

CFLAGS = -std=c++17 -Wall -Wextra -I $(SRC_PATH)

all: $(TARGET)

$(TARGET): src/main.cpp src/lexi.hpp
	$(CC) $(CFLAGS) -o $(TARGET) src/main.cpp

clean:
	$(RM) $(TARGET)