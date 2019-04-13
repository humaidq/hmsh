CC = gcc
CFLAGS = -Wall -Wextra -s -Os

TARGET = hmsh

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
