CCARGS = -O1 -g -std=c++14
tigger: lex.yy.o analyz.o y.tab.o tigger.o main.o
	g++ $(CCARGS) -o tigger lex.yy.o analyz.o y.tab.o tigger.o main.o
lex.yy.o: lex.yy.c y.tab.h tigger.h analyz.h
	g++ $(CCARGS) -c lex.yy.c
y.tab.o: y.tab.c y.tab.h tigger.h analyz.h
	g++ $(CCARGS) -c y.tab.c
lex.yy.c: lexer.l
	flex -l lexer.l
y.tab.c y.tab.h: tigger.y tigger.h analyz.h
	bison -y -d tigger.y
analyz.o: analyz.cpp analyz.h tigger.h
	g++ $(CCARGS) -c analyz.cpp
tigger.o: tigger.cpp tigger.h
	g++ $(CCARGS) -c tigger.cpp
main.o: main.cpp tigger.h
	g++ $(CCARGS) -c main.cpp
clean:
	rm tigger lex.yy.o y.tab.o lex.yy.c y.tab.c y.tab.h analyz.o tigger.o main.o
tarfile: 
	tar -cv *.cpp *.h *.l *.y eeyore tigger makefile riscv64C -f 1700010686.tar 
