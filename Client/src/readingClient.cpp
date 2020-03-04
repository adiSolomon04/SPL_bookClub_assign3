//
// Created by Adi Solo on 11/01/2020.
//


#include <readingClient.h>
#include <condition_variable>

readingClient::readingClient(std::mutex &queueMutex, std::mutex &ConnectionHandlerMutex, queue<string> &assignmentQueue,
                             ClientHolder &clientHolder, condition_variable &connectionHandler_cv,
                             std::condition_variable &assignmentQueue_cv)
        : queueMutex(queueMutex), ConnectionHandlerMutex(ConnectionHandlerMutex), assignmentQueue(assignmentQueue),
          clientHolder(clientHolder), connectionHandler_cv(connectionHandler_cv), assignmentQueue_cv(assignmentQueue_cv){
    //Conn* connectionHandler = holder.connectionHandler();
    //holder will have field bye? no need because we have wait on connectionHandler.
}


void readingClient::run() {
    std::string Frame;
    while (!clientHolder.isBye()) {//?? no need - line 11
        {
            std::unique_lock<std::mutex> lock(ConnectionHandlerMutex);
            //Read whole Frame from server.
            connectionHandler_cv.wait(lock, [=] {return (clientHolder.getConnectionHandlerPtr()->isConnected()|clientHolder.isBye());});
           /* if (!clientHolder.getConnectionHandlerPtr()->isConnected()) {
                connectionHandler_cv.wait(lock);*/
           // std::cout << "connectionHandler_cv notified in readingClient.cpp"<< std::endl;

                //wait for wakeup by Writer - when uploads the Conn Handler
            //}
            if (!clientHolder.isBye()) {
                if (!clientHolder.getConnectionHandlerPtr()->getFrameAscii(Frame, '\0')) {
                    //connection was closed - take a new connection handler Next round.
                    std::cout << "Disconnected. Exiting... in readingClient.cpp\n" << std::endl;
                } else {
                    addToQueue(Frame);
                    if(clientHolder.isDisconnectReceiptId(Frame)|(Frame.find("ERROR")==0)){
                        clientHolder.getConnectionHandlerPtr()->setConnectedBoolean(false);
                    }
                    Frame.clear();
                }
            }
        }
    }
    std::cout << "bye reading..." << std::endl;
}

void readingClient::addToQueue(string line) {
    std::lock_guard<std::mutex> lock(queueMutex);
    assignmentQueue.push(line);
    assignmentQueue_cv.notify_all();
}


