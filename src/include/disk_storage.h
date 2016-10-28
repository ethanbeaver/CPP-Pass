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
     * Retrieve Passwords and display them
     */
    void retrievePasswords();

    /**
     * Input Passwords and store them in a safe
     */
    void inputPasswords();

    /**
     * Write the password safe to a file
     *
     */
    bool writeToFile(string passPath);

    /**
     * Read a password safe from a file
     *
     */
    bool readFromFile(string keyPath, string passPath);

    /**
     * Generate a key file for the user to use!
     *
     */
    void genKey(string keyPath);

    void Menu();
private:
    Storage *storage;
};