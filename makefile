CXX=g++-5
all: tigger
	cd tigger && make && cd ..
clean: 
	cd tigger && make clean && cd ..