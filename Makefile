# Makefile template for project 1

CC = gcc
CFLAGS = -g -Wall
# main executable file first
TARGET = shell
# object files next
OBJS = helpers.o main.o
# header files next
DEPS = helpers.h
.PHONY : clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

.c.o: $(DEPS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET) *~

