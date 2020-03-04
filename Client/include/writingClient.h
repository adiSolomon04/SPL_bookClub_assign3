//
// Created by Adi Solo on 11/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_WRITINGCLIENT_H
#define BOOST_ECHO_CLIENT_WRITINGCLIENT_H

#include <queue>
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <Book.h>
#include <connectionHandler.h>
#include <stompProtocol.h>
#include <clientHolder.h>

class ClientHolder;
using std::string;
using namespace std;


class writingClient {
private:
    std::mutex &queueMutex;
    std::mutex &ConnectionHandlerMutex;
    std::condition_variable &assignmentQueue_cv;
    queue<string> &assignmentQueue;
    ClientHolder &clientHolder;
    std::condition_variable &connectionHandler_cv;

   // ConnectionHandler* connectionHandlerPtr = nullptr;
    stompProtocol protocol;
    bool bye = false;





public:
    writingClient(std::mutex &queueMutex, std::mutex &ConnectionHandlerMutex,
                  std::condition_variable &assignmentQueue_cv, queue<string> &assignmentQueue,
                  ClientHolder &clientHolder, std::condition_variable &connectionHandler_cv);
    void run();

};


#endif //BOOST_ECHO_CLIENT_WRITINGCLIENT_H
