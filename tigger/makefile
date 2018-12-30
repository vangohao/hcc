CC = gcc-5
CXX = g++-5
CCARGS = -O1 -g -std=c++14 -Wfatal-errors
all: tigger eeyore
eeyore: 
	cd eeyore_src && make CXX=$(CXX) && cp eeyore ../eeyore && cd ..
tigger: lex.yy.o analyz.o y.tab.o tigger.o main.o
	$(CXX) $(CCARGS) -o tigger lex.yy.o analyz.o y.tab.o tigger.o main.o
lex.yy.o: lex.yy.c y.tab.h tigger.h analyz.h
	$(CXX) $(CCARGS) -c lex.yy.c
y.tab.o: y.tab.c y.tab.h tigger.h analyz.h
	$(CXX) $(CCARGS) -c y.tab.c
lex.yy.c: lexer.l
	flex -l lexer.l
y.tab.c y.tab.h: tigger.y tigger.h analyz.h
	bison -y -d tigger.y
analyz.o: analyz.cpp analyz.h tigger.h
	$(CXX) $(CCARGS) -c analyz.cpp
tigger.o: tigger.cpp tigger.h
	$(CXX) $(CCARGS) -c tigger.cpp
main.o: main.cpp tigger.h
	$(CXX) $(CCARGS) -c main.cpp
clean:
	cd eeyore_src && make clean && cd ..
	-rm -f tigger lex.yy.o y.tab.o lex.yy.c y.tab.c y.tab.h analyz.o tigger.o main.o 1700010686.tar eeyore
tarfile: eeyore tigger
	tar -cv *.cpp *.h *.l *.y eeyore tigger makefile riscv64C riscv32C tiggerC -f 1700010686.tar 
