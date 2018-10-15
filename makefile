hcc : lex.yy.o y.tab.o main.o
	g++ -o hcc lex.yy.o y.tab.o main.o gotolist.o symbol.o node.o
gotolist.o : gotolist.cpp gotolist.h
	g++ -c gotolist.cpp
symbol.o : symbol.cpp symbol.h
	g++ -c symbol.cpp
node.o : node.cpp node.h
	g++ -c node.cpp
lex.yy.o : lex.yy.c y.tab.h
	g++ -c lex.yy.c
y.tab.o : y.tab.c y.tab.h
	g++ -c y.tab.c
main.o : main.cpp
	g++ -c main.cpp
lex.yy.c : lexer.l
	flex -l lexer.l
y.tab.c y.tab.h : parser.y node.h symbol.h gotolist.h gotolist.cpp symbol.cpp node.cpp
	bison -y -d parser.y
clean:
	rm hcc lex.yy.o y.tab.o lex.yy.c y.tab.c y.tab.h