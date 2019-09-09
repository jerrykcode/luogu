TARGET = P2068

ifeq ($(OS), Windows_NT)	#Windows
	Execute = $(TARGET).exe
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux) #Linux
		Execute = ./$(TARGET)
	endif
endif

run : $(TARGET) input
	$(Execute) < input

CC = gcc
$(TARGET) : P2068.c
	$(CC) -o $@ $<
