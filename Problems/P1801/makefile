QUESTION = P1801
ifeq ($(OS), Windows_NT)
	TARGET = $(QUESTION).exe
	EXECUTE = $(TARGET)
	CLEAN = del $(TARGET)
else
	UNAME_S = $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = $(QUESTION)
		EXECUTE = ./$(TARGET)
		CLEAN = rm $(TARGET)
	endif
endif

run : $(TARGET) input
	$(EXECUTE) < input

clean : $(TARGET)
	$(CLEAN)

CC = g++
SRC = $(QUESTION).cc

$(TARGET) : $(SRC)
	$(CC) -o $@ $<
