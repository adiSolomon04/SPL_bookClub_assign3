//
// Created by Adi Solo on 11/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_READINGCLIENT_H
#define BOOST_ECHO_CLIENT_READINGCLIENT_H

#include <queue>
#include <mutex>
#include <connectionHandler.h>

#include <condition_variable>
#include <clientHolder.h>

using namespace std;
class mutex;
class ClientHolder;

class readingClient {
private:
    std::mutex &queueMutex;
    std::mutex &ConnectionHandlerMutex;
    queue<string> &assignmentQueue;
    ClientHolder &clientHolder;
    std::condition_variable &connectionHandler_cv; //initialize
    std::condition_variable &assignmentQueue_cv;

public:
    readingClient(std::mutex &queueMutex, std::mutex &ConnectionHandlerMutex, queue<string> &assignmentQueue,
                  ClientHolder &clientHolder, condition_variable &connectionHandler_cv,
                  std::condition_variable &assignmentQueue_cv);

    //readingClient(mutex &queueMutex, mutex &connectionHandlerMutex, const condition_variable &connectionHandlerCv,
                  //const queue<string> &assignmentQueue, ClientHolder &clientHolder);

    void run();

    void addToQueue(string line);
};


#endif //BOOST_ECHO_CLIENT_READINGCLIENT_H
