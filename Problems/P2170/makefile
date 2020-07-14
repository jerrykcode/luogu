OJ = P2170

ifeq ($(OS), Windows_NT)
	TARGET = $(OJ).exe
	RUN = $(TARGET)
	CLEAN = del
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = $(OJ)
		RUN = ./$(TARGET)
		CLEAN = rm
	endif
endif

DIFF = diff -b

run : $(TARGET) input output
	$(RUN) < input > tmp_output
	$(DIFF) tmp_output output
	$(CLEAN) tmp_output

clean : $(TARGET)
	$(CLEAN) $<

clean_tmp_output : tmp_output
	$(CLEAN) $<
