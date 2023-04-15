all: GraphColoring.o
	g++ -o GraphColoring GraphColoring.o -L../ipasir/sat/lingelingbcj/ -lipasirlingelingbcj

clean:
	rm *.o
	
GraphColoring.o:	
	g++ -c GraphColoring.cpp
