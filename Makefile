CC = gcc
CFLAGS = -O3 -Wall -Wextra -pedantic -g
LFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt
TARGET = main

$(TARGET): main.c
	$(CC) main.c $(CFLAGS) -o $(TARGET) $(LFLAGS)

run: $(TARGET)
	./$(TARGET)
