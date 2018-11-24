tigger: lex.yy.o analyz.o y.tab.o tigger.o main.o
	g++ -g -Wall -std=c++14 -o tigger lex.yy.o analyz.o y.tab.o tigger.o main.o
lex.yy.o: lex.yy.c y.tab.h tigger.h analyz.h
	g++ -g -Wall -std=c++14 -c lex.yy.c
y.tab.o: y.tab.c y.tab.h tigger.h analyz.h
	g++ -g -Wall -std=c++14 -c y.tab.c
lex.yy.c: lexer.l
	flex -l lexer.l
y.tab.c y.tab.h: tigger.y tigger.h analyz.h
	bison -y -d tigger.y
analyz.o: analyz.cpp analyz.h tigger.h
	g++ -g -Wall -std=c++14 -c analyz.cpp
tigger.o: tigger.cpp tigger.h
	g++ -g -Wall -std=c++14 -c tigger.cpp
main.o: main.cpp tigger.h
	g++ -g -Wall -std=c++14 -c main.cpp
clean:
	rm tigger lex.yy.o y.tab.o lex.yy.c y.tab.c y.tab.h analyz.o tigger.o main.o
