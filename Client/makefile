CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system
CFLAGS += -pthread
LDFLAGS += -pthread


all:StompBookClubClient
	g++ -o bin/StompBookClubClient bin/Book.o bin/connectionHandler.o bin/clientHolder.o bin/keyboardClient.o bin/readingClient.o bin/stompProtocol.o bin/writingClient.o bin/StompBookClubClient.o $(LDFLAGS)

StompBookClubClient: bin/Book.o bin/connectionHandler.o bin/clientHolder.o bin/keyboardClient.o bin/readingClient.o bin/stompProtocol.o bin/writingClient.o bin/StompBookClubClient.o

bin/Book.o: src/Book.cpp include/Book.h
	g++ $(CFLAGS) -o bin/Book.o src/Book.cpp
	
bin/connectionHandler.o: src/connectionHandler.cpp include/connectionHandler.h
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/clientHolder.o:  src/clientHolder.cpp include/clientHolder.h
	g++ $(CFLAGS) -o bin/clientHolder.o src/clientHolder.cpp

bin/keyboardClient.o: src/keyboardClient.cpp include/keyboardClient.h
	g++ $(CFLAGS) -o bin/keyboardClient.o src/keyboardClient.cpp

bin/readingClient.o : src/readingClient.cpp include/readingClient.h
	g++ $(CFLAGS) -o bin/readingClient.o src/readingClient.cpp

bin/stompProtocol.o : src/stompProtocol.cpp include/stompProtocol.h
	g++ $(CFLAGS) -o bin/stompProtocol.o src/stompProtocol.cpp

bin/writingClient.o : src/writingClient.cpp include/writingClient.h
	g++ $(CFLAGS) -o bin/writingClient.o src/writingClient.cpp

bin/StompBookClubClient.o:  src/StompBookClubClient.cpp
	g++ $(CFLAGS) -o bin/StompBookClubClient.o src/StompBookClubClient.cpp

.PHONY: clean
clean:
	rm -f bin/*
