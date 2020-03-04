//
// Created by Adi Solo on 11/01/2020.
//

#include <writingClient.h>
#include <iostream>

writingClient::writingClient(std::mutex &queueMutex, std::mutex &ConnectionHandlerMutex,
                             std::condition_variable &assignmentQueue_cv, queue<string> &assignmentQueue,
                             ClientHolder &clientHolder, std::condition_variable &connectionHandler_cv)
        : queueMutex(queueMutex), ConnectionHandlerMutex(ConnectionHandlerMutex), assignmentQueue_cv(assignmentQueue_cv), assignmentQueue(assignmentQueue),
        clientHolder(clientHolder), connectionHandler_cv(connectionHandler_cv), protocol(stompProtocol()) {
    //stompProtocol protocol; //initialize a stomp protocol
   // connectionHandlerPtr = clientHolder.getConnectionHandlerPtr();
}

void writingClient::run() {
    string Frame;
    string msg;
    while (!clientHolder.isBye()) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            assignmentQueue_cv.wait(lock, [=] { return !this->assignmentQueue.empty(); });
            msg = assignmentQueue.front();
            assignmentQueue.pop();
        }
        //wait on the queue till there are messages
        Frame = protocol.process(msg);
        //User Entered a login command.
        if (Frame.substr(0,Frame.find("\n")) == "CONNECT") {//post to Find
            string host = msg.substr(msg.find_last_of("\n") + 1, msg.find_last_of(":")-msg.find_last_of("\n") -1);
            string string_port = msg.substr(msg.find_last_of(":") + 1, msg.npos);
            short port = (short) (std::stol(string_port));
            msg = msg.substr(msg.find_last_of("\n") - 1);
            Frame = Frame.substr(0,Frame.size()-host.size()-string_port.size()-1);
            {
                std::lock_guard<std::mutex> lock(ConnectionHandlerMutex);
                //will sync ConnectionHandlerMutex only if The
                // connection handler is changed
               // connectionHandlerPtr = ;
                clientHolder.setConnectionHandlerPtr(new ConnectionHandler(host, port));
                //destractor for the previous.
                clientHolder.setDisconnectReceipt(false, "");
                clientHolder.getConnectionHandlerPtr()->connect();
            }
            connectionHandler_cv.notify_all();
        }
        else if(Frame.find("DISCONNECT") == 0) {
            string ReceiptId = Frame.substr(msg.find(":") + 1);
            ReceiptId = ReceiptId.substr(0, ReceiptId.find("\n"));
            clientHolder.setDisconnectReceipt(true, ReceiptId);
        }
        else if((Frame.find("GOTDISCONNECT") == 0)|(Frame.find("ERROR")==0)) {
            clientHolder.getConnectionHandlerPtr()->close();
            Frame = "";
        }
        else if(Frame == "bye1"){
            Frame = "";
            clientHolder.setBye(true);
            //bye = true;
            std::cout << "bye bye kid"<< std::endl;
            connectionHandler_cv.notify_all();
            assignmentQueue_cv.notify_all();
        } else if (Frame == "bye2") {
            assignmentQueue_cv.notify_all();
            Frame = "";
        }
        if (Frame != "") {
            Frame.append("\n");
            if (!clientHolder.getConnectionHandlerPtr()->sendFrameAscii(Frame, '\0')) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
                //Need to exit??
            }
        }
    }
    std::cout << "bye writing..." << std::endl;
}