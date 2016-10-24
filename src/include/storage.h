//
// Created by Andrew Russell on 10/19/16.
//

#ifndef CPP_PASS_STORAGE_H
#define CPP_PASS_STORAGE_H

#include "safe_string.h"
#include <sstream>

#include "openssl/rand.h"
#include "openssl/evp.h"
#include "openssl/aes.h"
#include "openssl/sha.h"

/**
 * Always initialize like `entry e = {};`. Failure to do so could result in a
 * segfault.
 */
struct entry {
    unsigned int id;
    SafeString *title;
    SafeString *username;
    SafeString *password;

    /**
     * Turn an entry struct into a string.
     *
     * @return Binary string representation of the entry object.
     */
    string serialize() {
        string title = this->title->get_data(),
                username = this->username->get_data(),
                password = this->password->get_data();

        unsigned title_len = (unsigned) title.length(),
                username_len = (unsigned) username.length(),
                password_len = (unsigned) password.length();

        char buf[4 + 4 * 3 + title_len + username_len + password_len];
        unsigned i = 0;

        memcpy(&buf[i], &this->id, sizeof(this->id));
        i += sizeof(this->id);

        memcpy(&buf[i], &title_len, sizeof(title_len));
        i += sizeof(title_len);
        memcpy(&buf[i], title.data(), title_len);
        i += title_len;

        memcpy(&buf[i], &username_len, sizeof(username_len));
        i += sizeof(username_len);
        memcpy(&buf[i], username.data(), username_len);
        i += username_len;

        memcpy(&buf[i], &password_len, sizeof(password_len));
        i += sizeof(password_len);
        memcpy(&buf[i], password.data(), password_len);
        i += password_len;

        return string(buf, i);
    }

    /**
     * Turn a string into an entry struct.
     *
     * Play nice with this guy. If you do something wrong, you'll either get a
     * segfault or runtime error.
     *
     * @param s String to deserialize.
     */
    void deserialize(string s) {
        char buf[s.length()];
        memcpy(buf, s.data(), s.length());

        unsigned title_len, username_len, password_len;

        unsigned i = 0;
        memcpy(&this->id, &buf[i], sizeof(id));
        i += sizeof(this->id);

        memcpy(&title_len, &buf[i], sizeof(title_len));
        i += sizeof(title_len);
        char ca_title[title_len];
        memcpy(ca_title, &buf[i], title_len);
        i += title_len;
        string s_title(ca_title, title_len);

        memcpy(&username_len, &buf[i], sizeof(username_len));
        i += sizeof(username_len);
        char ca_username[username_len];
        memcpy(ca_username, &buf[i], username_len);
        i += username_len;
        string s_username(ca_username, username_len);

        memcpy(&password_len, &buf[i], sizeof(password_len));
        i += sizeof(password_len);
        char ca_password[password_len];
        memcpy(ca_password, &buf[i], password_len);
        i += password_len;
        string s_password(ca_password, password_len);

        if (i != s.length())
            throw new runtime_error("Incorrect length");

        this->title = new SafeString(s_title);
        this->username = new SafeString(s_username);
        this->password = new SafeString(s_password);
    }
};

/**
 * Store, retrieve, and manage secure entries. All communication with this
 * module is performed using secure strings.
 */
class Storage {
public:

    /**
     * Create a new password safe.
     *
     * @param key Binary key data.
     */
    Storage(SafeString *key);

    /**
     * Safely clean up the password safe.
     */
    ~Storage();

    /**
     * Spit out a minimum set of data to let the frontend choose what to view.
     *
     * @return Array of IDs and titles.
     */
    vector<entry> list();

    /**
     * Put a new entry into the password safe. This will overwrite the provided
     * entry's ID field with a unique one.
     *
     * @param e
     * @return True on success, false on failure.
     */
    bool create(entry *e);

    /**
     * Fetch a particular entry which contains a username, password, and title.
     *
     * @param e
     * @return True on success, false on failure.
     */
    bool get(entry *e);

    /**
     * The ID field in the provided entry object is ignored.
     *
     * @param e
     * @return True on success, false on failure.
     */
    bool set(entry *e);

    /**
     * Remove an object from the password safe.
     *
     * @return True on success, false on failure.
     */
    bool remove(entry *e);

    /**
     * Load in a password safe file.
     *
     * @param s
     */
    void load(SafeString *s);

    /**
     * Save the current state of the password safe.
     *
     * @return
     */
    SafeString *save();

protected:

    static SafeString *encrypt(unsigned char *key, SafeString *plain);

    static SafeString *decrypt(unsigned char *key, SafeString *cipher);

private:

    vector<entry> entries;

    // TODO: Keep this is a SafeString
    unsigned char key[32];
};

#endif //CPP_PASS_STORAGE_H
