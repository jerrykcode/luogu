OJ = P1533
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
	$(CLEAN_CMD) $<

CC = g++
CXXFLAGS = -std=c++11
SRC = $(OJ).cc

$(TARGET) : $(SRC)
	$(CC) -o $@ $< $(CXXFLAGS)
