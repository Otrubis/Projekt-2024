
TARGET = photoshop


CC = gcc
CFLAGS = -Wall -Wextra -std=c11


SOURCES = main.c photoshop.c
HEADERS = structure.h
OBJECTS = $(SOURCES:.c=.o)


$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)


%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)


.PHONY: clean
