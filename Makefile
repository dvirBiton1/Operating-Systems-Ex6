.PHONY: all clean
CXX=gcc

all: main1
main1: main1.o 
	$(CXX) main1.o -o main1
main1.o: main1.c
	$(CXX) -c main1.c
clean :
	rm -f *.o main1