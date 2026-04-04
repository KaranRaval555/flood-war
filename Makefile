CC = gcc
CFLAGS = -O3 -Wall -Wextra -pedantic
LFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt
TARGET = main

$(TARGET): main.c base.h
	$(CC) main.c $(CFLAGS) -o $(TARGET) $(LFLAGS)

run: $(TARGET)
	./$(TARGET)
