# Makefile for the crawler project

CC = gcc
CFLAGS = -Wall -pedantic -std=c11
LIBS = -lcurl

# List of source files
SRCS = url.c pagedir.c curl.c crawler.c set.c hashtable.c

# Output binary
TARGET = crawler

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
