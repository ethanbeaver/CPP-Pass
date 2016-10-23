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
    /**
     * Safely store an arbitrary string in memory. This is not the
     * end-all-be-all of in-memory secure storage, but it should really slow
     * down the script kiddies.
     *
     * This is a ONE WAY PROCESS. In order to change the value, you must
     * instantiate a new object and destruct the current one.
     *
     * @param s Pointer to data to store
     * @param len Number of characters to store
     */
    SafeString(unsigned char *s, unsigned len);

    /**
     * <b>Not yet implemented!</b>
     *
     * Will eventually provide a nice way to duplicate a secure string securely.
     *
     * @param ss SecureString object to duplicate
     */
    SafeString(SafeString *ss);

    /**
     * What goes up must come down. Correctly and securely disassemble a
     * SafeString object.
     */
    ~SafeString();

    /**
     * Pull the unencrypted data out of secure memory.
     *
     * @param s Memory location to place decrypted data.
     * @param len Actual length of data decrypted. This is incontrast to
     * `get_max_length()` which always overestimates the length.
     */
    void get_data(unsigned char *s, unsigned *len);

    /**
     * Estimates the plain text's actual length. This function will always
     * overestimate.
     *
     * @return An estimated plain text length
     */
    unsigned get_max_length();

private:

    /**
     * Keeper of the data, defender of the insecure, pointer of the way.
     */
    void *p_data;

    /**
     * p_data's mighty sidekick. Always keeps p_data within bounds.
     */
    size_t data_len;

    /**
     * The key used to obfuscate the data in memory. This is technically a weak
     * point, but only an experienced hacker would be able to use it.
     */
    unsigned char key[16];
};

#endif //CPP_PASS_SAFE_STRING_H_H
