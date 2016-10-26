//
// Created by Andrew Russell on 10/25/16.
//

#include "include/entry.h"

entry::entry() {
    this->title = NULL;
    this->username = NULL;
    this->password = NULL;
}

entry::entry(string title, string username, string password) {
    this->title = new SafeString(title);
    this->username = new SafeString(username);
    this->password = new SafeString(password);
}

string entry::serialize() {
    string result;  // Result buffer
    unsigned char unsigned_buf[sizeof(unsigned)];  // Buffer for unsigned

    // Stash the ID
    {
        memcpy(unsigned_buf, &this->id, sizeof(unsigned));
        result.append((char *) unsigned_buf, sizeof(unsigned_buf));
    }

    // Stash the title
    {
        // Suck out some information
        string s = this->title->get_data();
        unsigned len = (unsigned) s.length();

        // Reformat and save the length
        memcpy(unsigned_buf, &len, sizeof(unsigned));
        result.append((char *) unsigned_buf, sizeof(unsigned_buf));

        // Save the string itself
        result.append(s);
    }

    // Stash the username
    {
        // Suck out some information
        string s = this->username->get_data();
        unsigned len = (unsigned) s.length();

        // Reformat and save the length
        memcpy(unsigned_buf, &len, sizeof(unsigned));
        result.append((char *) unsigned_buf, sizeof(unsigned_buf));

        // Save the string itself
        result.append(s);
    }

    // Stash the password
    {
        // Suck out some information
        string s = this->password->get_data();
        unsigned len = (unsigned) s.length();

        // Reformat and save the length
        memcpy(unsigned_buf, &len, sizeof(unsigned));
        result.append((char *) unsigned_buf, sizeof(unsigned_buf));

        // Save the string itself
        result.append(s);
    }

    return result;
}

void entry::deserialize(string s) {
    // Basically just an alias to the data in the string
    const char *buf = s.data();
    unsigned i = 0;

    // Fetch the entry ID
    {
        memcpy(&this->id, &buf[i], sizeof(this->id));
        i += sizeof(this->id);
    }

    // Fetch the title
    {
        unsigned *len;

        // Check if we have enough data for a length
        if (s.length() < sizeof(unsigned) + i) {
            throw new runtime_error("Invalid format");
        }

        // Copy in the length
        len = (unsigned *) &buf[i];
        i += sizeof(unsigned);

        // Check if we have enough data for a value
        if (s.length() < *len + i) {
            throw new runtime_error("Invalid format");
        }

        // Copy in the value
        unsigned char *ca_value = (unsigned char *) malloc(*len);
        memcpy(ca_value, &buf[i], *len);
        i += *len;

        // Save off the value and release the memory
        this->title = new SafeString(ca_value, *len);
        free(ca_value);
    }

    // Fetch the username
    {
        unsigned *len;

        // Check if we have enough data for a length
        if (s.length() < sizeof(unsigned) + i) {
            throw new runtime_error("Invalid format");
        }

        // Copy in the length
        len = (unsigned *) &buf[i];
        i += sizeof(unsigned);

        // Check if we have enough data for a value
        if (s.length() < *len + i) {
            throw new runtime_error("Invalid format");
        }

        // Copy in the value
        unsigned char *ca_value = (unsigned char *) malloc(*len);
        memcpy(ca_value, &buf[i], *len);
        i += *len;

        // Save off the value and release the memory
        this->username = new SafeString(ca_value, *len);
        free(ca_value);
    }

    // Fetch the password
    {
        unsigned *len;

        // Check if we have enough data for a length
        if (s.length() < sizeof(unsigned) + i) {
            throw new runtime_error("Invalid format");
        }

        // Copy in the length
        len = (unsigned *) &buf[i];
        i += sizeof(unsigned);

        // Check if we have enough data for a value
        if (s.length() < *len + i) {
            throw new runtime_error("Invalid format");
        }

        // Copy in the value
        unsigned char *ca_value = (unsigned char *) malloc(*len);
        memcpy(ca_value, &buf[i], *len);
        i += *len;

        // Save off the value and release the memory
        this->password = new SafeString(ca_value, *len);
        free(ca_value);
    }

    // If there was a length mismatch, we might have overwrite something
    if (i != s.length()) {
        throw new runtime_error("Incorrect length");
    }
}
