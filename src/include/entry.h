//
// Created by Andrew Russell on 10/25/16.
//

#ifndef CPP_PASS_ENTRY_H
#define CPP_PASS_ENTRY_H

#include "safe_string.h"

struct entry {
    /**
     * Unique entry identifier. Do not modify unless you know what you're doing.
     */
    unsigned int id;

    /**
     * Pointer to a safeified title.
     */
    SafeString *title;

    /**
     * Pointer to a safeified username.
     */
    SafeString *username;

    /**
     * Pointer to a safeified password.
     */
    SafeString *password;

    entry();

    entry(string title, string username, string password);

    /**
     * Turn an entry into a string!
     *
     * @return Stringified entry
     */
    string serialize();

    /**
     * Turn a string into an entry! Go ahead... try to break it.
     *
     * @param s String representation of entry
     */
    void deserialize(string s);
};

#endif //CPP_PASS_ENTRY_H
