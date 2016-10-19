//
// Created by Andrew Russell on 10/19/16.
//

#ifndef CPP_PASS_SAFE_STRING_H_H
#define CPP_PASS_SAFE_STRING_H_H

#include <string>

using namespace std;

class SafeString {
public:
    SafeString();

    SafeString(string s);

    ~SafeString();

private:
    char *data;
    unsigned int length;
};

#endif //CPP_PASS_SAFE_STRING_H_H
