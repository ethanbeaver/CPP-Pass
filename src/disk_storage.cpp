//
// Created by ethan on 10/25/16.
//


#include "include/disk_storage.h"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <tclap/CmdLine.h>

#include "include/storage.h"
#include "storage.cpp"
#include "include/safe_string.h"


string diskStorage::retrievePasswords(string keyPath, string passPath) {
    //Read out passwords from the password safe onto the command line
    char *password = getpass("Password: ");
    SafeString pass((unsigned char *)password, (unsigned int)strlen(password));

}

void diskStorage::inputPasswords(SafeString *pass) {
    //Input passwords from the command line and pass them to the safe storage

}

bool diskStorage::writeToFile(Storage *storage, string passPath) {
    //Write the password safe that's in memory to a file on the hard disk as well as the key

    SafeString *passData = storage->save();
    string data = passData->get_data();
    ofstream passOut;
    OutFile.open(passPath, ios::out | ios::binary);
    OutFile.write( (char*)data.data(), data.length());
    OutFile.close();
}

bool diskStorage::readFromFile(string keyPath, string passPath){
    //Write a password safe that's in a file into memory
    ifstream safe (keyPath,fstream::binary);
    safe.seekg (0, safe.end);
    int length1 = safe.tellg();
    safe.seekg (0, safe.beg);
    char * buffer1 = new char [length1];
    safe.read(buffer1, length1);

    ifstream key (keyPath,fstream::binary);
    key.seekg (0, key.end);
    int length2 = key.tellg();
    key.seekg (0, key.beg);
    char * buffer2 = new char [length2];
    key.read(buffer2, length2);

    SafeString passFile((unsigned char *)buffer1, (unsigned int)strlen(buffer1));
    SafeString keyFile((unsigned char *)buffer2, (unsigned int)strlen(buffer2));

    storage = new Storage(&keyFile);
    storage->load(&passFile);

}