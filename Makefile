CXX=g++
CXXFLAGS=-std=c++14 -ggdb -Wall -Wextra -pedantic -Werror -Wnon-virtual-dtor -Iinclude `xmlrpc-c-config c++2 --cflags`

CLIENT_LIBS=`xmlrpc-c-config c++2 client --libs`
SERVER_LIBS=`xmlrpc-c-config c++2 abyss-server server-util --libs`

default: client server

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

server: server.o
	$(CXX) $(CXXFLAGS) -o server server.o $(SERVER_LIBS)

client: client.o
	$(CXX) $(CXXFLAGS) -o client client.o $(CLIENT_LIBS)

clean:
	rm -f client server *.o
