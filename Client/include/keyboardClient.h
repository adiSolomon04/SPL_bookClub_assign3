//
// Created by Adi Solo on 13/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_KEYBOARDCLIENT_H
#define BOOST_ECHO_CLIENT_KEYBOARDCLIENT_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <clientHolder.h>

using namespace std;

class keyboardClient {
private:
    std::mutex &queueMutex;
    queue<string> &assignmentQueue;
    std::condition_variable &cv;
    ClientHolder &clientHolder;

    string userName;
    vector<string> splitByspace(string line);
public:
    keyboardClient(std::mutex &queueMutex, queue<string> &assignmentQueue, std::condition_variable &cv,
                   ClientHolder &clientHolder);
    void run();

    void addToQueue(string line);

    string keyboardTranslate(string keyboardLine);
};


#endif //BOOST_ECHO_CLIENT_KEYBOARDCLIENT_H
