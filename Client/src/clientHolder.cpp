//
// Created by Adi Solo on 15/01/2020.
//

#include <clientHolder.h>


ClientHolder::ClientHolder(ConnectionHandler *connectionHandlerPtr) :
        connectionHandlerPtr(connectionHandlerPtr) {}

ConnectionHandler *ClientHolder::getConnectionHandlerPtr(){
    return connectionHandlerPtr;
}

bool ClientHolder::isBye() const {
    return bye;
}

void ClientHolder::setBye(bool bye) {
    ClientHolder::bye = bye;
}

void ClientHolder::setConnectionHandlerPtr(ConnectionHandler *connectionHandlerPtr) {
    //(*this->connectionHandlerPtr).~ConnectionHandler();
    delete this->connectionHandlerPtr;
    ClientHolder::connectionHandlerPtr = connectionHandlerPtr;
}

ClientHolder::~ClientHolder() {
    //(*this->connectionHandlerPtr).~ConnectionHandler();
    delete this->connectionHandlerPtr;
}
ClientHolder::ClientHolder (const ClientHolder &clientHolder):connectionHandlerPtr(nullptr){
    bye = clientHolder.bye;
    doneDisconnect = clientHolder.doneDisconnect;
    disconnectReceipt="";
}


void ClientHolder::setDisconnectReceipt(bool doneDisconnect, const string &disconnectReceipt) {
    ClientHolder::doneDisconnect = doneDisconnect;
    ClientHolder::disconnectReceipt = disconnectReceipt;
}
bool ClientHolder::isDisconnectReceiptId(std::string Frame){
    return (doneDisconnect && Frame.find(disconnectReceipt)!=Frame.npos);
}

ClientHolder& ClientHolder::operator=(const ClientHolder &clientHolder) {
    delete this->connectionHandlerPtr;
    connectionHandlerPtr= nullptr;
    bye = clientHolder.bye;
    doneDisconnect = clientHolder.doneDisconnect;
    disconnectReceipt="";
    return *this;
}

