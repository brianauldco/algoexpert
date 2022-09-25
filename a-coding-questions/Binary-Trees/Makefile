default: all
all: construct validateBST

construct: construct.o
	g++ construct.o -o construct

validateBST: validateBST.o
	g++ validateBST.o -o validateBST

construct.o: construct.cpp
	g++ -c construct.cpp

validateBST.o: validateBST.cpp
	g++ -c validateBST.cpp

clean:
	rm -f *.o rm -f *~
