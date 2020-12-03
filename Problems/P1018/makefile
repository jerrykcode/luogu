OJ = P1018

ifeq ($(OS), Windows_NT)
	TARGET = $(OJ).exe
	RUN = $(TARGET)
	LEAN_CMD = del
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = $(OJ)
		RUN = ./$(TARGET)
		CLEAN_CMD = rm
	endif
endif

DIFF = diff -b

run : $(TARGET) in out
	$(RUN) < in > tmp_out
	-$(DIFF) tmp_out out
	$(CLEAN_CMD) tmp_out

clean : 
	$(CLEAN_CMD) $(TARGET)
