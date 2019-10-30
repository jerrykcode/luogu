ifeq ($(OS), Windows_NT) 
	TARGET = P2023.exe
	EXECUTE = $(TARGET)
	CLEAN = del $(TARGET)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = P2023
		EXECUTE = ./$(TARGET)
		CLEAN = rm $(TARGET)
	endif
endif

run : $(TARGET) input
	$(EXECUTE) < input

clean : $(TARGET)
	$(CLEAN)

CC = gcc
CFLAGS = -std=c99

$(TARGET) : P2023.c
	$(CC) -o $@ $< $(CFLAGS)
