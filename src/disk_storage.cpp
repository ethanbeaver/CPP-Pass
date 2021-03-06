//
// Created by ethan on 10/25/16.
//


#include "include/disk_storage.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <bitset>
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
    cout << "Here's a list of the titles of all the entries in the password manager:\n";
    for (unsigned i = 0; i < entries.size(); i++) {
        // See if there's a match in the title
        cout << i << ": " << entries[i].title->get_data() << endl;
    }
    cout << "\nYou're now viewing passwords. Choose an option\n";
    unsigned int select;
    int dnum;
    int choice;
    do {
        cout << "(1. View the titles of all entries, 2. View an entry, 3. Delete an entry, 0.Back): ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Here's a list of the titles:\n";
                for (unsigned i = 0; i < entries.size(); i++) {
                    // See if there's a match in the title
                    cout << i << ": " << entries[i].title->get_data() << endl;
                }
                break;
            case 2:
                cout << "Enter the number corresponding to the entry you'd like to see: ";
                cin >> select;
                if (select < entries.size()) {
                    cout << "Title: " << entries[select].title->get_data() << endl;
                    cout << "Username: " << entries[select].username->get_data() << endl;
                    cout << "Password: " << entries[select].password->get_data() << endl;
                } else {
                    cout << "Invalid index!\n";
                }
                break;
            case 3:
                cout << "Which entry would you like to delete?(Enter -1 to abort) \n";
                cin >> dnum;
                if(dnum >= 0 && dnum <entries.size()) {
                    storage->remove(&entries[dnum]);
                    return; //Don't remove this return. Bad things happen if you do.
                }
                break;
            default:
                cout << "Goodbye!\n";
                break;
        }
    } while (choice != 0);
    return;
}

void diskStorage::inputPasswords() {
    //Input passwords from the command line and pass them to the safe storage
    char *t = new char[100];
    char *user = new char[100];
    cout << "Title: ";
    cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    cin.getline(t, 100);
    cout << "Username: ";
    cin.getline(user, 100);
    char *pass = new char[100];
    pass = getpass("Password: ");
/*    SafeString pass((unsigned char *)password, (unsigned int)strlen(password));
    SafeString t((unsigned char *)title, (unsigned int)strlen(title));
    SafeString user((unsigned char *)username,(unsigned int)strlen(username));*/
    entry site(t, user, pass);
    /*site.title = new SafeString(t);
    site.username = new SafeString(user);
    site.password = new SafeString(pass);*/

    storage->create(&site);
    memset(t,'-',100);
    memset(user,'-',100);
    memset(pass,'-',100);
    return;
}

bool diskStorage::writeToFile(char *passPath) {
    //Write the password safe that's in memory to a file on the hard disk as well as the key
    SafeString *passData = storage->save();
    string data = passData->get_data();
    //const char *buffer = data.c_str();
    /*ofstream passOut;
    remove(passPath.c_str());
    passOut.open(passPath, ios::out | ios::binary);
    passOut.write( (char*)data.data(), data.length());
    passOut.close();*/

    FILE *pFile;
    pFile = fopen(passPath, "wb");
    fwrite(data.c_str(), sizeof(char), data.length(), pFile);
    fclose(pFile);
    return true;
}

bool diskStorage::readFromFile(char *keyPath, char *passPath) {
    //Read a password safe that's in a file into memory

    FILE *kFile;
    long lSize;
    char *buffer2;
    size_t result;

    kFile = fopen(keyPath, "rb");
    if (kFile == NULL) {
        cerr << "Invalid key file!" << endl;
        return false;
    }
    fseek(kFile, 0, SEEK_END);
    lSize = ftell(kFile);
    rewind(kFile);

    buffer2 = (char *) malloc(sizeof(char) * lSize);
    if (buffer2 == NULL) {
        fputs("Memory error", stderr);
        exit(2);
    }

    result = fread(buffer2, 1, lSize, kFile);
    if (result != lSize) {
        fputs("Reading error", stderr);
        exit(3);
    }


    SafeString *keyFile = new SafeString((unsigned char *) buffer2,
                                         (unsigned int) lSize);

    storage = new Storage(keyFile);

    FILE *pFile;
    long lSize1;
    char *buffer1;
    size_t result1;

    pFile = fopen(passPath, "rb");
    if (pFile == NULL) {
        cout << "Invalid file! Creating new password file...\n";
        return true;
    }
    fseek(pFile, 0, SEEK_END);
    lSize1 = ftell(pFile);
    rewind(pFile);

    buffer1 = (char *) malloc(sizeof(char) * lSize1);
    if (buffer1 == NULL) {
        fputs("Memory error", stderr);
        exit(2);
    }

    result1 = fread(buffer1, 1, (size_t) lSize1, pFile);
    if (result1 != lSize1) {
        fputs("Reading error", stderr);
        exit(3);
    }

    SafeString *passFile = new SafeString((unsigned char *) buffer1,
                                          (unsigned int) lSize1);
    storage->load(passFile);
    memset(buffer1, '-', lSize1);
    memset(buffer2,'-',lSize);
    return true;
}

void diskStorage::genKey(char *keyPath) {
    SafeString *key = Storage::generate_key();
    string keyData = key->get_data();

    FILE *pFile;
    pFile = fopen(keyPath, "wb");
    fwrite(keyData.c_str(), sizeof(char), keyData.length(), pFile);
    fclose(pFile);
    return;
}

void diskStorage::Menu() {
    int choice = 0;
    cout << "You're in! What would you like to do in your password safe?\n";
    do {
        cout << "(1. View/remove passwords, 2.Enter a password, 0.Exit): ";
        cin >> choice;
        switch (choice) {
            case 1:
                diskStorage::retrievePasswords();
                break;
            case 2:
                diskStorage::inputPasswords();
                break;
            default:
                cout << "Goodbye!\n";
                break;
        }
    } while (choice != 0);
    return;
}