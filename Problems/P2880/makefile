ifeq ($(OS), Windows_NT)
	TARGET = P2880.exe
	EXECUTE = $(TARGET)
	CLEAN = del $(TARGET)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = P2880
		EXECUTE = ./$(TARGET)
		CLEAN = rm $(TARGET)
	endif
endif

run : $(TARGET) input
	$(EXECUTE) < input

clean : 
	$(CLEAN)

CC = gcc
CFLAGS = -std=c99

$(TARGET) : P2880.c
	$(CC) -o $(TARGET) P2880.c $(CFLAGS)
