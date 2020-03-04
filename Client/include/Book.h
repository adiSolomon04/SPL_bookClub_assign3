//
// Created by adisolo@wincs.cs.bgu.ac.il on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_BOOK_H
#define BOOST_ECHO_CLIENT_BOOK_H
#include <string>
using namespace std;


class Book {
private:
    string Name;
    string lander;

public:

    Book(const string &name);

    const string &getName() const;

    const string &getLander() const;

    void setLander(const string &lander);
};


#endif //BOOST_ECHO_CLIENT_BOOK_H
