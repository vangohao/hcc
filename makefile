hcc : lex.yy.o y.tab.o main.o
	g++ -o hcc lex.yy.o y.tab.o main.o
lex.yy.o : lex.yy.c y.tab.h
	g++ -c lex.yy.c
y.tab.o : y.tab.c y.tab.h
	g++ -c y.tab.c
main.o : main.cpp
	g++ -c main.cpp
lex.yy.c : lexer.l
	flex -l lexer.l
y.tab.c y.tab.h : parser.y node.h symbol.h
	bison -y -d parser.y
clean:
	rm hcc lex.yy.o y.tab.o lex.yy.c y.tab.c y.tab.h