OJ = P1345
SRC = $(OJ).cc
OBJ = $(OJ).o
CC = g++
CXXFLAGS += -std=c++11

windows : $(OJ).exe in out
	$< < in > tmp_out
	-FC tmp_out out
	del tmp_out

linux : $(OJ) in out
	./$< < in > tmp_out
	-diff -b tmp_out  out
	rm tmp_out


$(OJ).exe : $(OBJ)
	$(CC) $< -o $@ $(CXXFLAGS)

$(OJ) : $(OBJ)
	$(CC) $< -o $@ $(CXXFLAGS)

$(OBJ) : $(SRC)
	$(CC) -c $< $(CXXFLAGS)

win_clean : 
	del $(OBJ)
	del $(OJ).exe

li_clean : 
	rm $(OBJ)
	rm $(OJ)
