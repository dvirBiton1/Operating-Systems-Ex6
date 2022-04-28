.PHONY: all clean
CXX=gcc

all: guard #main1
main1: main1.o 
	$(CXX) main1.o -o main1 -lpthread
main1.o: main1.c
	$(CXX) -c main1.c
guard: guard.o 
	g++ guard.o -o guard -lpthread
guard.o: guard.cpp
	g++ -c guard.cpp
clean :
	rm -f *.o main1 guard