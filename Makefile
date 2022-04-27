.PHONY: all clean
CXX=gcc

all: server client
server: Ex5.h server.o 
	$(CXX) server.o -o server -lpthread -ltbb
client: client.o Ex5.h
	$(CXX) client.o -o client -lpthread -ltbb
# test: test.o Ex4.hpp
# 	$(CXX) test.o -o test -lpthread -ltbb
server.o: server.c Ex5.h
	$(CXX) -c server.c
client.o: client.c Ex5.h
	$(CXX) -c client.c
# test.o: test.cpp Ex5.hpp
# 	$(CXX) -c test.cpp

clean :
	rm -f *.o client server 