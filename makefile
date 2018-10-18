hcc : lex.yy.o y.tab.o main.o gotolist.o symbol.o node.o pre.o
	g++ -o hcc lex.yy.o y.tab.o main.o gotolist.o symbol.o node.o pre.o
pre.o : pre.cpp pre.h
	g++ -c pre.cpp --std=c++11
gotolist.o : gotolist.cpp gotolist.h
	g++ -c gotolist.cpp --std=c++11
symbol.o : symbol.cpp symbol.h
	g++ -c symbol.cpp --std=c++11
node.o : node.cpp node.h
	g++ -c node.cpp --std=c++11
lex.yy.o : lex.yy.c y.tab.h
	g++ -c lex.yy.c --std=c++11
y.tab.o : y.tab.c y.tab.h
	g++ -c y.tab.c --std=c++11
main.o : main.cpp
	g++ -c main.cpp --std=c++11
lex.yy.c : lexer.l
	flex -l lexer.l
y.tab.c y.tab.h : parser.y node.h symbol.h gotolist.h gotolist.cpp symbol.cpp node.cpp
	bison -y -d parser.y
clean:
	rm hcc lex.yy.o y.tab.o lex.yy.c y.tab.c y.tab.h gotolist.o symbol.o node.o main.o pre.o