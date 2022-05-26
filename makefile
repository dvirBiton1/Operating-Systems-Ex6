.PHONY: all clean
CXX=gcc
CPP=g++

all: main1 #pollServer clientReactor #main1 #guard singleton pollServer clientReactor #server client
	./main1
main1: main1.o
	$(CXX) main1.o -o main1 -lpthread
main1.o: main1.c
	$(CXX) -c main1.c
guard: guard.o
	$(CPP) guard.o -o guard -lpthread
guard.o: guard.cpp
	$(CPP) -c guard.cpp
singleton: singleton.o
	$(CPP) singleton.o -o singleton
singleton.o: singleton.cpp
	$(CPP) -c singleton.cpp
pollServer: pollServer.o 
	$(CPP) pollServer.cpp reactor.cpp -o pollServer -pthread -lpthread 
pollServer.o: pollServer.cpp
	$(CPP) -c pollServer.cpp
clientReactor: clientReactor.o
	$(CPP) clientReactor.cpp -o clientReactor -lpthread
clientReactor.o: clientReactor.cpp
	$(CPP) -c clientReactor.cpp

clean :
	rm -f *.o main1 guard singleton pollServer clientReactor