//
// Created by Andrew Russell on 10/19/16.
//

#ifndef CPP_PASS_STORAGE_H
#define CPP_PASS_STORAGE_H

//#include <string>
#include "safe_string.h"

//using namespace std;

struct entry {
    unsigned int id;
    SafeString title;
    SafeString username;
    SafeString password;
};

class Storage {
public:
    Storage();

    ~Storage();

    void apply_key(SafeString key);

    void apply_password(SafeString password);

    bool get(unsigned int id, entry *e);

    bool set(unsigned int id, entry *e);

private:

};

#endif //CPP_PASS_STORAGE_H
