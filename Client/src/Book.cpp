//
// Created by adisolo@wincs.cs.bgu.ac.il on 12/01/2020.
//

#include <Book.h>


Book::Book(const string &name) : Name(name), lander("") {
}

const string &Book::getLander() const {
    return lander;
}

void Book::setLander(const string &lander) {
    Book::lander = lander;
}

const string &Book::getName() const {
    return Name;
}
