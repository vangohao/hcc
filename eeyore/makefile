CXX = g++-5
eeyore : lex.yy.o y.tab.o main.o gotolist.o symbol.o node.o pre.o fault.o
	$(CXX) -o eeyore lex.yy.o y.tab.o main.o gotolist.o symbol.o node.o pre.o fault.o
fault.o : fault.cpp node.h symbol.h gotolist.h fault.h pre.h
	$(CXX) -c fault.cpp --std=c++11
pre.o : pre.cpp node.h symbol.h gotolist.h fault.h pre.h
	$(CXX) -c pre.cpp --std=c++11
gotolist.o : gotolist.cpp node.h symbol.h gotolist.h fault.h pre.h
	$(CXX) -c gotolist.cpp --std=c++11
symbol.o : symbol.cpp node.h symbol.h gotolist.h fault.h pre.h
	$(CXX) -c symbol.cpp --std=c++11
node.o : node.cpp node.h node.h symbol.h gotolist.h fault.h pre.h
	$(CXX) -c node.cpp --std=c++11
lex.yy.o : lex.yy.c y.tab.h node.h symbol.h gotolist.h fault.h pre.h
	$(CXX) -c lex.yy.c --std=c++11
y.tab.o : y.tab.c y.tab.h node.h symbol.h gotolist.h fault.h pre.h
	$(CXX) -c y.tab.c --std=c++11
main.o : main.cpp node.h symbol.h gotolist.h fault.h pre.h
	$(CXX) -c main.cpp --std=c++11
lex.yy.c : lexer.l
	flex -l lexer.l
y.tab.c y.tab.h : parser.y node.h symbol.h gotolist.h fault.h pre.h
	bison -y -d parser.y
clean:
	-rm eeyore lex.yy.o y.tab.o lex.yy.c y.tab.c y.tab.h gotolist.o symbol.o node.o main.o pre.o fault.o 1700010686.tar a.out
tarfile:
	tar -cv parser.y lexer.l *.h *.cpp makefile -f 1700010686.tar tmp.oi
