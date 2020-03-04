//
// Created by adisolo@wincs.cs.bgu.ac.il on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_STOMPPROTOCOL_H
#define BOOST_ECHO_CLIENT_STOMPPROTOCOL_H
#include <string>
#include <unordered_map>
#include <Book.h>

using namespace std;

class stompProtocol {
private:
   /* bool terminate;*/
    bool isLoggedin = false;
    int subId = 1;//?
    int receiptId = 1;
    string userName="";

    unordered_map<std::string, unordered_map<std::string, Book*>> topicBooksinPossesion;
    unordered_map<std::string, unordered_map<std::string, Book*>> topicBooksNotinPossesion;
    unordered_map<std::string, unordered_map<std::string, Book*>> topicBooksWaitingWishList;

    unordered_map<std::string, int> topicTosubId;
    unordered_map<int,std::string> receiptIdToMessage;

    void deleteUser();

public:
    stompProtocol();
    string process(string msg);
    //bool shouldTerminate();


};


#endif //BOOST_ECHO_CLIENT_STOMPPROTOCOL_H
