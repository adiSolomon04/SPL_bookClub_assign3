#include <stdlib.h>

#include <connectionHandler.h>
#include <readingClient.h>
#include <writingClient.h>
#include <keyboardClient.h>

#include <mutex>
#include <thread>
#include <clientHolder.h>


/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main() {
    std::condition_variable assignmentQueue_cv;
    std::condition_variable connectionHandler_cv; //In order to wait in reading client for writing client to update ConnectionHandler.
    std::mutex queueMutex;
    std::mutex connectionHandlerMutex;
    queue<string> assignmentQueue;
    ConnectionHandler *connectionHandlerPtr = new ConnectionHandler("",0);

    ClientHolder clientHolder(connectionHandlerPtr);

    //not yet connected!!
    keyboardClient keyboardClient(queueMutex, assignmentQueue, assignmentQueue_cv, clientHolder);


    //send maybe port & host, make 2 connection handlers?
    writingClient writingClient(queueMutex, connectionHandlerMutex, assignmentQueue_cv, assignmentQueue, clientHolder,
                                connectionHandler_cv);
    readingClient readingClient(queueMutex, connectionHandlerMutex, assignmentQueue, clientHolder, connectionHandler_cv,
                                assignmentQueue_cv);

    std::thread th0(&keyboardClient::run, &keyboardClient);
    std::thread th1(&writingClient::run, &writingClient);
    std::thread th2(&readingClient::run, &readingClient);

    th0.join();
    th1.join();
    th2.join();

    //add destructor of connectionHandlerPtr in the clientHolder Destructor.
    //delete clientHolder.getConnectionHandlerPtr(); //
    //(*clientHolder.getConnectionHandlerPtr()).~ConnectionHandler();
    queueMutex.~mutex();
    connectionHandlerMutex.~mutex();
    //delete connectionHandlerPtr;
    return 0;
}