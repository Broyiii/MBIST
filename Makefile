all:
	g++ MBISTPart.cpp parser.cpp writeInfo.cpp parseFile.cpp BK.cpp genetic.cpp -pthread -static -O3 -o MBISTPart