//
// Created by Ethan Beaver on 10/25/16.
//

#ifndef CPP_PASS_DISK_STORAGE_H
#define CPP_PASS_DISK_STORAGE_H

#endif //CPP_PASS_DISK_STORAGE_H

class diskStorage {
public:

    
    userInterface();
    /**
     * Display the passwords! If you can...
     */
    displayPasswords(string path);

    /**
     * Input Passwords
     */
    inputPasswords();

    /**
     * Write the password safe to a file
     *
     */
    writeToFile(string path);

    /**
     * Read a password safe from a file
     *
     */
    readFromFile(string path);

    /**
     * Write the key to a file
     */

    writeKeyFile(string path);

    /**
     * Load a key file to decrypt the password file
     *
     */
    loadKeyFile(string path);

};