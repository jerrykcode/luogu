OJ = P1973

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

run : $(TARGET) input output
	$(RUN) < input > tmp_output
	-$(DIFF) tmp_output output
	$(CLEAN_CMD) tmp_output
    
clean : $(TARGET)
	$(CLEAN_CMD) $<
