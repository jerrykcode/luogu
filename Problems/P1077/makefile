OJ = P1077

ifeq ($(OS), Windows_NT)
	TARGET = $(OJ).exe
	RUN = $(TARGET)
	CLEAN_CMD = del
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = $(OJ)
		RUN = ./$(TARGET)
		CLEAN_CMD = rm
	endif
endif

DIFF = diff -b

run : in out $(TARGET)
	$(RUN) < in > tmp_out
	-$(DIFF) tmp_out out
	$(CLEAN_CMD) tmp_out

clean : $(TARGET)
	$(CLEAN_CMD) $<
