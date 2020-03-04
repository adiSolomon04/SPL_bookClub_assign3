//
// Created by Adi Solo on 15/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_CLIENTHOLDER_H
#define BOOST_ECHO_CLIENT_CLIENTHOLDER_H

#include <connectionHandler.h>
#include <readingClient.h>
#include <writingClient.h>
#include <keyboardClient.h>
#include <mutex>
//#include <condition_variable> - might need to add in all of the files

class ClientHolder {
private:
    ConnectionHandler *connectionHandlerPtr;
    bool bye = false;
    bool doneDisconnect = false;
    std::string  disconnectReceipt="";


public:
    ClientHolder(ConnectionHandler *connectionHandlerPtr);
    virtual ~ClientHolder();
    ClientHolder (const ClientHolder &clientHolder);
    ClientHolder& operator= (const ClientHolder &clientHolder);

    bool isBye() const;

    void setBye(bool bye);

    ConnectionHandler *getConnectionHandlerPtr();
    void setConnectionHandlerPtr(ConnectionHandler *connectionHandlerPtr);

    void setDisconnectReceipt(bool doneDisconnect, const string &disconnectReceipt);
    bool isDisconnectReceiptId(std::string Frame);

};


#endif //BOOST_ECHO_CLIENT_CLIENTHOLDER_H
