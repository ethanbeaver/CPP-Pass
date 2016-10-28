//
// Created by ethan on 10/25/16.
//


#include "include/disk_storage.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <tclap/CmdLine.h>
//#include <search.h>

#include "include/storage.h"
#include "storage.cpp"
#include "include/safe_string.h"
#include "include/entry.h"


void diskStorage::retrievePasswords() {
    //Read out passwords from the password safe onto the command line
    //Storage *storage;
    vector<entry> entries;
    entries = storage->list();
    cout << "Now it's time to view your passwords.\n";
    cout << "Here's a list of the titles:\n";
    for (unsigned i = 0; i < entries.size(); i++) {
        // See if there's a match in the title
        cout << i << ": " << entries[i].title->get_data() << endl;
    }
    cout << "Enter the number corresponding of the entry you'd like to see: ";
    unsigned int select;
    cin >> select;
    if(select < entries.size()) {
        cout << "Title: " << entries[select].title->get_data() << endl;
        cout << "Username: " << entries[select].username->get_data() << endl;
        cout << "Password: " << entries[select].password->get_data() << endl;

    }
    else{
        cout << "Invalid index!\n";
    }
    return;
}

void diskStorage::inputPasswords() {
    //Input passwords from the command line and pass them to the safe storage
    char *t = new char[100];
    char *user = new char[100];
    cout << "Title: ";
    cin >> t;
    cout << "Username: ";
    cin >> user;
    char *pass = new char[100];
    pass = getpass("Password: ");
/*    SafeString pass((unsigned char *)password, (unsigned int)strlen(password));
    SafeString t((unsigned char *)title, (unsigned int)strlen(title));
    SafeString user((unsigned char *)username,(unsigned int)strlen(username));*/
    entry site(t, user, pass);
    /*site.title = new SafeString(t);
    site.username = new SafeString(user);
    site.password = new SafeString(pass);*/

    //Storage *storage;
    storage->create(&site);
    return;
}

bool diskStorage::writeToFile(string passPath) {
    //Write the password safe that's in memory to a file on the hard disk as well as the key
    //Storage *storage;
    SafeString *passData = storage->save();
    string data = passData->get_data();
    ofstream passOut;
    remove(passPath.c_str());
    passOut.open(passPath, ios::out | ios::binary);
    passOut.write( (char*)data.data(), data.length());
    passOut.close();
    return true;
}

bool diskStorage::readFromFile(string keyPath, string passPath){
    //Read a password safe that's in a file into memory


    ifstream key (keyPath,fstream::binary);
    if(!key.good()) {
        cerr << "Invalid key file!" << endl;
        return false;
    }
    key.seekg(0, key.end);
    int length2 = key.tellg();
    key.seekg(0, key.beg);
    char *buffer2 = new char[length2];
    key.read(buffer2, length2);
    SafeString *keyFile = new SafeString((unsigned char *) buffer2, (unsigned int) strlen(buffer2));

    storage = new Storage(keyFile);
    ifstream safe (passPath,fstream::binary);
    if(!safe.good()) {
        cerr << "Invalid file! Creating new password file...\n";
        return true;
    }
    safe.seekg(0, safe.end);
    int length1 = safe.tellg();
    safe.seekg(0, safe.beg);
    char *buffer1 = new char[length1];
    safe.read(buffer1, length1);
    SafeString *passFile = new SafeString((unsigned char *)buffer1, (unsigned int)strlen(buffer1));
    storage->load(passFile);
    return true;
}

void diskStorage::genKey(string keyPath){
    SafeString *key = Storage::generate_key();
    string keyData = key->get_data();
    ofstream passKey;
    passKey.open(keyPath, ios::out | ios::binary);
    passKey.write( (char*)keyData.data(), keyData.length());
    passKey.close();
    return;
}

void diskStorage::Menu(){
    int choice = 0;
    cout << "You're in! What would you like to do in your password safe?\n";
    do {
        cout << "(1. View a password, 2.Enter a password, 0.Exit): ";
        cin >> choice;
        switch (choice) {
            case 1:
                diskStorage::retrievePasswords();
                break;
            case 2:
                diskStorage::inputPasswords();
                break;
            default:
                "Goodbye!\n";
                break;
        }
    }while(choice!=0);
    return;
}