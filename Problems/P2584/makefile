OJ = P2584
ifeq ($(OS), Windows_NT)
	TARGET = $(OJ).exe
	EXECUTE_CMD = #empty
	CLEAN_CMD = del
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = $(OJ)
		EXECUTE_CMD = ./
		CLEAN_CMD = rm
	endif
endif

run : $(TARGET) input output
	$(EXECUTE_CMD)$(TARGET) < input > tmp_output
	diff tmp_output output
	$(CLEAN_CMD) tmp_output

clean : $(TARGET)
	$(CLEAN_CMD) $(TARGET)

CC = g++
SRC = $(OJ).cc
CXXFLAGS = -std=c++11

$(TARGET) : $(SRC)
	$(CC) -o $@ $< $(CXXFLAGS)
