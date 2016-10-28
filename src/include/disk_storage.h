//
// Created by Ethan Beaver on 10/25/16.
//

#ifndef CPP_PASS_DISK_STORAGE_H
#define CPP_PASS_DISK_STORAGE_H

#endif //CPP_PASS_DISK_STORAGE_H
#include "safe_string.h"
#include "storage.h"

class diskStorage {
public:

    /**
     * Retrieve Passwords
     */
    string retrievePasswords(string keyPath, string passPath);

    /**
     * Input Passwords
     */
    void inputPasswords(SafeString *pass);

    /**
     * Write the password safe to a file
     *
     */
    bool writeToFile(Storage *storage, string passPath);

    /**
     * Read a password safe from a file
     *
     */
    bool readFromFile(string keyPath, string passPath);

private:
    Storage *storage;
};