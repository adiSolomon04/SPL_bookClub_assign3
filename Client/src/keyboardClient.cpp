//
// Created by Adi Solo on 13/01/2020.
//

#include <mutex>
#include <iostream>
#include <keyboardClient.h>

keyboardClient::keyboardClient(std::mutex &queueMutex, queue<string> &assignmentQueue, std::condition_variable &cv,
                               ClientHolder &clientHolder)
        : queueMutex(queueMutex), assignmentQueue(assignmentQueue), cv(cv), clientHolder(clientHolder), userName(""){
}

void keyboardClient::run(){
    //TODO: need to change
    const short bufsize = 1024;
    char buf[bufsize];
    while(!clientHolder.isBye()){
        //blocking- read a line.
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        string protocolCmd = keyboardTranslate(line);
        addToQueue(protocolCmd);
        if(line=="bye"){
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock);
        }
    }
    std::cout << "bye keyboard..." << std::endl;
}

void keyboardClient::addToQueue(string line) {
    std::unique_lock<std::mutex> lock(queueMutex);
    assignmentQueue.push(line);
    //if(line.find("CONNECT")!=line.npos)
    cv.notify_all();
    //will notify even if not connected. problem?
}

string keyboardClient::keyboardTranslate(string keyboardLine) {
    //Split the string into vector by space
    vector<string> strs = splitByspace(keyboardLine);
    string protocolCmd = "";
    string str1;
    string str2;

    if(strs[0]=="login"){
        protocolCmd = strs[1];//.substr(':');
        str1 = strs[1];
        str2 = strs[2];
        protocolCmd = "CONNECT\naccept-version:1.2\nhost:"+str1.substr(0,':')+"\nlogin:"+str2
                +"\npasscode:"+strs[3]+"\n"+str1;
        userName = str2;
    }
    if(strs[0]=="join"){
        //add id of the subscription
        str1 = strs[1];
        protocolCmd = "SUBSCRIBE\ndestination:"+str1+"\nid:";
    }
    if(strs[0]=="exit"){
        str1 = strs[1];
        //add id of the subscription
        protocolCmd = "UNSUBSCRIBE\ndestination:"+str1+"\nid:";
    }
    if(strs[0]=="add"){
        str1 = strs[1];
        str2 = strs[2];
        protocolCmd = "SEND\ndestination:"+str1+"\n\n"+userName+" has added the book "+str2;
    }
    if(strs[0]=="borrow"){
        str1 = strs[1];
        str2 = strs[2];
        protocolCmd = "SEND\ndestination:"+str1+"\n\n"+userName+" wish to borrow "+str2;
    }
    if(strs[0]=="return"){
        str1 = strs[1];
        str2 = strs[2];
        //complete in protocol - lender
        protocolCmd = "SEND\ndestination:"+str1+"\n\nReturning "+str2;
    }
    if(strs[0]=="status"){
        str1 = strs[1];
        protocolCmd = "SEND\ndestination:"+str1+"\n\nbook status";
    }
    if(strs[0]=="logout"){
        //complete in protocol - receipt id
        protocolCmd = "DISCONNECT\nreceipt:";
    }
    if(strs[0]=="bye"){
        protocolCmd = "bye";
        userName = str2;
    }
    return protocolCmd;
}

vector<string> keyboardClient::splitByspace(string keyboardLine){
    vector<string> strs;
    size_t pos = keyboardLine.find( ' ' );
    size_t initialPos = 0;

    while( pos != std::string::npos ) {
        strs.push_back( keyboardLine.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;
        pos = keyboardLine.find( ' ', initialPos );
    }

    // Add the last one
    strs.push_back( keyboardLine.substr( initialPos, std::string::npos));
    //std::min( pos, keyboardLine.size() ) - initialPos + 1 ) );?
    return strs;
}