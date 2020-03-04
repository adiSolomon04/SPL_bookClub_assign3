//
// Created by adisolo@wincs.cs.bgu.ac.il on 12/01/2020.
//

#include <iostream>
#include <unordered_set>
#include <stompProtocol.h>
stompProtocol::stompProtocol():topicBooksinPossesion(), topicBooksNotinPossesion(), topicBooksWaitingWishList(), topicTosubId(), receiptIdToMessage(){

}

/*bool stompProtocol::shouldTerminate(){
    return terminate;
}*/
string stompProtocol::process(string msg){
    std::string answer = "";
    //Connect - difine sockets, connected frame - boolean is connected  = true;
    if((msg.substr(0,msg.find("\n"))=="CONNECT")&!isLoggedin) {
        answer = msg;
        string s = "login:";
        userName = msg.substr(msg.find(s)+s.size());
        userName = userName.substr(0,userName.find("\n"));
    }
    if((msg=="bye")&!isLoggedin)
        answer = msg+"1";
    else if((msg=="bye")&isLoggedin)
        answer = msg+"2";
    if(msg.substr(0,msg.find("\n"))=="CONNECTED"){
        isLoggedin = true;
        std::cout<<"login successful" << std::endl;
    }

    string cmd = msg.substr(0,msg.find("\n"));
    if(cmd=="ERROR"){
        //to do
        string s = "message:";
        string errorMsg = msg.substr(msg.find(s) + s.size());
        errorMsg = errorMsg.substr(0, errorMsg.find("\n"));
        std::cout<<"Error:"+errorMsg << std::endl;
        answer = "ERROR";
        deleteUser();
    }
    if(isLoggedin){
        if(cmd=="MESSAGE") {
            string dest = msg.substr(msg.find("destination:"));
            dest = dest.substr(dest.find(":") + 1, dest.find("\n") - dest.find(":") - 1);
            cout << dest << ":" << msg.substr(msg.find("\n\n") + 2);
            if (msg.find("wish to borrow") != string::npos) {
                string s = "borrow ";
                string bookNameWished = msg.substr(msg.find(s) + s.size());
                bookNameWished = bookNameWished.substr(0, bookNameWished.find("\n"));
                if(topicBooksinPossesion.find(dest) != topicBooksinPossesion.end()) {
                    std::unordered_map<std::string, Book *>::iterator it = (topicBooksinPossesion.find(dest)->second).find(bookNameWished);
                    if (it != (topicBooksinPossesion.find(dest)->second).end()) {
                        answer = "SEND\ndestination:" + dest + "\n\n" + userName + " has " + bookNameWished;
                    }
                }
            } else if ((msg.find("has")!= string::npos) & (msg.find("has added")== string::npos)){
                string s = "has ";
                string bookNameWished = msg.substr(msg.find(s) + s.size());
                bookNameWished = bookNameWished.substr(0, bookNameWished.find("\n"));
                if(topicBooksWaitingWishList.find(dest) != topicBooksWaitingWishList.end()) {
                    unordered_map<std::string, Book *> wishOfGaner = topicBooksWaitingWishList.find(dest)->second;
                    std::unordered_map<std::string, Book *>::iterator it = wishOfGaner.find(bookNameWished);
                    if (it != wishOfGaner.end()) {
                        string msgBody = msg.substr(msg.find("\n\n") + 2);
                        string lander = msgBody.substr(0, msgBody.find(" "));
                        answer = "SEND\ndestination:" + dest + "\n\n" + "Taking " + bookNameWished + " from " + lander;
                        wishOfGaner.erase(it);
                        (topicBooksWaitingWishList.find(dest)->second).erase(bookNameWished);
                        (it->second)->setLander(lander);
                        if(topicBooksinPossesion.find(dest) == topicBooksinPossesion.end())
                            topicBooksinPossesion.insert({{dest, unordered_map<std::string, Book*>()}});
                        (topicBooksinPossesion.find(dest)->second).insert({it->first, it->second});
                    }
                }
            }else if (msg.find("Taking")!= string::npos){
                string s = "Taking ";
                string bookNameWished = msg.substr(msg.find(s) + s.size());
                bookNameWished = bookNameWished.substr(0, bookNameWished.find(" "));
                s = "from ";
                string nameOfLandered = msg.substr(msg.find(s) + s.size());
                nameOfLandered = nameOfLandered.substr(0, nameOfLandered.find("\n"));


                if(nameOfLandered==userName){
                    if(topicBooksinPossesion.find(dest)!= topicBooksinPossesion.end()) {
                        std::unordered_map<std::string, Book *>::iterator it = (topicBooksinPossesion.find(dest)->second).find(bookNameWished);
                        if(it!=(topicBooksinPossesion.find(dest)->second).end()) {
                            (topicBooksinPossesion.find(dest)->second).erase(bookNameWished);
                            if(topicBooksNotinPossesion.find(dest) == topicBooksNotinPossesion.end())
                                topicBooksNotinPossesion.insert({{dest, unordered_map<std::string, Book*>()}});
                            (topicBooksNotinPossesion.find(dest)->second).insert({it->first, it->second});
                        }
                    }
                }
            }else if (msg.find("Returning")!= string::npos){
                string s = "Returning ";
                string bookNameReturning = msg.substr(msg.find(s) + s.size());
                bookNameReturning = bookNameReturning.substr(0, bookNameReturning.find(" to"));
                s = "to ";
                string nameOfLander = msg.substr(msg.find(s) + s.size());
                nameOfLander = nameOfLander.substr(0, nameOfLander.find("\n"));
                if(nameOfLander==userName){
                    if(topicBooksNotinPossesion.find(dest)!= topicBooksNotinPossesion.end()) {
                        std::unordered_map<std::string, Book *>::iterator it = (topicBooksNotinPossesion.find(dest)->second).find(bookNameReturning);
                        if(it != (topicBooksNotinPossesion.find(dest)->second).end()) {
                            (topicBooksNotinPossesion.find(dest)->second).erase(bookNameReturning);
                            if(topicBooksinPossesion.find(dest) == topicBooksinPossesion.end())
                                topicBooksinPossesion.insert({{dest, unordered_map<std::string, Book*>()}});
                            (topicBooksinPossesion.find(dest)->second).insert({it->first, it->second});
                        }
                    }
                }
            }else if (msg.find("status")!= string::npos){
                answer = "SEND\ndestination:" + dest + "\n\n"+userName+":";
                if(topicBooksinPossesion.find(dest)!=topicBooksinPossesion.end()) {
                    unordered_map<std::string, Book *> topicBooksInPossesionOfGaner = topicBooksinPossesion.find(dest)->second;

                    for (auto it : topicBooksInPossesionOfGaner) {
                        answer += it.second->getName() + ",";
                    }
                    answer = answer.substr(0, answer.size() - 1);
                }
            }
       }
       if(cmd=="RECEIPT"){
           string gotReceiptId = msg.substr(msg.find(":")+1);
           gotReceiptId = gotReceiptId.substr(0,gotReceiptId.find("\n"));
           string Message = receiptIdToMessage.find(std::stoi(gotReceiptId))->second;
           if(Message.find("Joined ") == 0){
                std::cout<<Message << std::endl;
           }else if(Message.find("Exited ") == 0){
               string s = "Exited club ";
               topicTosubId.erase(Message.substr(s.size()));
               std::cout<<Message << std::endl;
           }else if(Message.find("DISCONNECT") == 0){
               answer = "GOTDISCONNECT";
               std::cout<<Message << std::endl;
               deleteUser();
           }
           receiptIdToMessage.erase(std::stoi(gotReceiptId));
       }
       if(cmd == "SUBSCRIBE"){
           answer = msg + std::to_string(subId)+"\nreceipt:"+std::to_string(receiptId)+"\n";
           string dest = msg.substr(msg.find("destination:"));
           dest = dest.substr(dest.find(":")+1,dest.find("\n")-dest.find(":")-1);
           topicTosubId.insert({dest,subId});//maybe better in RECEIPT
           receiptIdToMessage.insert({receiptId,"Joined club "+dest});
           subId++;
           receiptId++;

       }
       if(cmd == "UNSUBSCRIBE"){
           string dest = msg.substr(msg.find("destination:"));
           dest = dest.substr(dest.find(":")+1,dest.find("\n")-dest.find(":")-1);
           answer = msg + std::to_string(topicTosubId.find(dest)->second)+"\nreceipt:"+std::to_string(receiptId)+"\n";
           receiptIdToMessage.insert({receiptId,"Exited club "+dest}); //todo when get receipt dell from topicTosubId,should del the book of topics?
           receiptId++;
       }
       if(cmd == "DISCONNECT"){
           answer = msg +std::to_string(receiptId)+"\n";
           receiptIdToMessage.insert({receiptId,"DISCONNECT"});
           receiptId++;
           //todo delete all books?
       }
       if(cmd == "SEND"){
           //add
           string destination = msg.substr(msg.find("destination:"));
           destination = destination.substr(destination.find(":")+1,destination.find("\n")-destination.find(":")-1);
           if(msg.find("added the book")!=msg.npos) {
               string bookName = msg.substr(msg.find_last_of(" ")+1, msg.size() - 1);
               Book* newBook = new Book(bookName);
               if(topicBooksinPossesion.find(destination)==topicBooksinPossesion.end())
                   topicBooksinPossesion.insert({destination, unordered_map<std::string, Book*>()});
               (topicBooksinPossesion.find(destination)->second).insert({bookName,newBook});
               answer = msg;

           }
           if(msg.find("borrow")!=msg.npos) {
               string bookName = msg.substr(msg.find_last_of(" ")+1, msg.size() - 1);
               answer = msg;
               Book* newBook = new Book(bookName);
               if(topicBooksWaitingWishList.find(destination)==topicBooksWaitingWishList.end())
                   topicBooksWaitingWishList.insert({destination, unordered_map<std::string, Book*>()});
               (topicBooksWaitingWishList.find(destination)->second).insert({bookName,newBook});
           }
           if(msg.find("Returning")!=msg.npos) {
               string bookName = msg.substr(msg.find_last_of(" ")+1, msg.size() - 1);
               Book* returnBook = topicBooksinPossesion.find(destination)->second.find(bookName)->second;
               topicBooksinPossesion.find(destination)->second.erase(bookName);
               answer = msg.append(" to "+returnBook->getLander());
               delete returnBook;
           }
           if(msg.find("status")!=msg.npos) {
               answer = msg;
           }
       }
    }
    return answer;
}
void stompProtocol::deleteUser(){
    for (auto it : topicBooksinPossesion) {
        for (auto itGaner : it.second) {
            delete itGaner.second;
        }
        it.second.clear();
    }
    topicBooksinPossesion.clear();

    for (auto it : topicBooksNotinPossesion) {
        for (auto itGaner : it.second) {
            delete itGaner.second;
        }
        it.second.clear();
    }
    topicBooksNotinPossesion.clear();

    for (auto it : topicBooksWaitingWishList) {
        for (auto itGaner : it.second) {
            delete itGaner.second;
        }
        it.second.clear();
    }
    topicBooksWaitingWishList.clear();

    topicTosubId.clear();
    receiptIdToMessage.clear();
     subId = 1;
     receiptId = 1;
     userName="";
    isLoggedin = false;
}