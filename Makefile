main: complex.o main.cpp sort.o
	g++ main.cpp complex.o  sort.o -o main -fopenmp -mavx -O3


complex.o : complex.cpp
	g++ -c complex.cpp -mavx2 -fopenmp

sort.o : sort.cpp 
	g++ -c sort.cpp -fopenmp 


clear:
	erase *.o