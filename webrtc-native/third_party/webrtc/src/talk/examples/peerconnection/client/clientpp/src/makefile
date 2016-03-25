

CFLAGS=-I. -I../lib/rapidjson/include -I../../websocketpp/src -I../../boost/boost_1_53_0
LINKERFLAGS=-static -L ../../websocketpp -lwebsocketpp -L ../../boost/boost_1_53_0/stage/lib -lboost_regex -lboost_system -lboost_random -lboost_thread -lboost_date_time -pthread -lrt
CC=g++
OBJ = socket_io_client.o main.o 

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

socketio: $(OBJ)
	g++ -o $@ $^ $(LINKERFLAGS)
	
clean: 
	rm -rf socketio
	rm -rf *.o

all: socketio

