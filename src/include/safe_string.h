//
// Created by Andrew Russell on 10/19/16.
//

#ifndef CPP_PASS_SAFE_STRING_H_H
#define CPP_PASS_SAFE_STRING_H_H

#include <cstdlib>
#include <functional>
#include <cstring>
#include <vector>

#include "openssl/rand.h"
#include "openssl/evp.h"
#include "openssl/aes.h"

using namespace std;

class SafeString {
public:
    SafeString(unsigned char *s, unsigned len);

    SafeString(SafeString *s);

    ~SafeString();

    void get_data(unsigned char *s);

    unsigned get_length();

private:
    unsigned char *p_data;
    unsigned long data_len;
    unsigned char key[16];
};

#endif //CPP_PASS_SAFE_STRING_H_H
