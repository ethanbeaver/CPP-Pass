//
// Created by Andrew Russell on 10/19/16.
//

#include <iostream>
#include "include/storage.h"

Storage::Storage(SafeString *key) : Storage(key, NULL) {}

Storage::Storage(SafeString *key, SafeString *safe) {
    if (key != NULL) {
        // Extract key into array
        char *cp_key = (char *) malloc(key->get_max_length());
        memset(cp_key, 0, key->get_max_length());
        unsigned key_len;
        key->get_data((unsigned char *) cp_key, &key_len);

        // Key size checking
        if (key_len < sizeof(this->key))
            throw new runtime_error("Provided key was too short");

        // Copy the first 16 bytes into the key
        memcpy(this->key, cp_key, sizeof(this->key));
    } else {
        throw new runtime_error("Key must be provided");
    }
// TODO: Move this stuff into its own function and call it from `load` and `save`.
//        // Decrypt the password safe ///////////////////////////////////////////
//
//        char *cp_safe = (char *) malloc(safe->get_max_length());
//        memset(cp_safe, 0, safe->get_max_length());
//        unsigned safe_len;
//        safe->get_data((unsigned char *) cp_safe, &safe_len);
//
//        // Assemble a decryption object
//        EVP_CIPHER_CTX *ctx;
//        if (!(ctx = EVP_CIPHER_CTX_new()))
//            throw new runtime_error(
//                    "Abstract cipher object failed to initialize");
//        if (EVP_DecryptInit(ctx, EVP_aes_128_cbc(), this->key, this->key) != 1)
//            throw new runtime_error(
//                    "AES decryption object failed to initialize");
//
//        // Build a temporary container for the plain text
//        unsigned char *plain = (unsigned char *) malloc(safe_len);
//        if (plain == NULL)
//            throw new runtime_error(
//                    "Unable to allocate extra memory for plain text");
//        memset(plain, 0, safe_len);
//
//        // Decrypt the cipher text
//        int plain_len_update, plain_len_final = 0;
//        if (EVP_DecryptUpdate(ctx, plain, &plain_len_update,
//                              (const unsigned char *) cp_safe,
//                              (int) safe_len) != 1)
//            throw new runtime_error("Encryption update failed");
//        if (EVP_DecryptFinal_ex(ctx, plain + plain_len_update,
//                                &plain_len_final) !=
//            1)
//            throw new runtime_error("Encryption finalization failed");
//        int plain_len = plain_len_update + plain_len_final;
//
//        // Build decrypted string objects
//        string s_plain((char *) plain, (unsigned) plain_len);
//
//        // Clean up decryption mechanics
//        memset(plain, 0, safe_len);
//        free((void *) plain);
//        EVP_CIPHER_CTX_free(ctx);
}

Storage::~Storage() {

}

void Storage::load(SafeString *ss) {
    string s = ss->get_data();

    // TODO: put decryption here

    // Ensure the length isn't entirely invalid
    if (s.length() < SHA512_DIGEST_LENGTH)
        throw new runtime_error("Safe file is too short");

    // Extract relevant sections
    string safe(s.substr(0, s.length() - SHA512_DIGEST_LENGTH));
    string digest(s.substr(s.length() - SHA512_DIGEST_LENGTH));

    // Fetch and compute digests
    unsigned char given_digest[SHA512_DIGEST_LENGTH],
            computed_digest[SHA512_DIGEST_LENGTH];
    memcpy(&given_digest,
           digest.data(),
           digest.length());
    SHA512((const unsigned char *) safe.data(),
           safe.length(),
           computed_digest);

    // Check if computed and given digests match
    if (memcmp(given_digest, computed_digest, SHA512_DIGEST_LENGTH) != 0) {
        throw new runtime_error("File verification failed");
    }

    while (safe.length()) {
        // Get serialized entry length
        unsigned len;
        memcpy(&len, safe.data(), sizeof(len));

        // Build substring containing just this entry's data
        string buf((char *) safe.data() + sizeof(len), len);

        // Perform the deserialization
        entry e;
        e.deserialize(buf);

        // Store the results
        this->entries.push_back(e);

        // Step to the next entry
        s.erase(0, sizeof(len) + len);
    }
}

SafeString *Storage::save() {
    ostringstream oss(ios::app);

    // Serialize all entries and save to the output stream
    for (unsigned i = 0; i < this->entries.size(); i++) {
        // Perform the actual serialization
        string s = this->entries[i].serialize();

        // Buffer the length
        unsigned len = (unsigned) s.length();
        char ca_len[4];
        memcpy(ca_len, &len, sizeof(len));

        // Dump to output stream
        oss << ca_len;
        oss << s;
    }

    // Compute and append SHA512 digest
    string s = oss.str();
    unsigned char digest[SHA512_DIGEST_LENGTH];
    SHA512((const unsigned char *) s.data(), s.length(), digest);

    oss << string((char *) digest, SHA512_DIGEST_LENGTH);
    s = oss.str();

    // TODO: put encryption here

    // Bundle and return the entries
    return new SafeString(s);
}

vector<entry> Storage::list() {
    return vector<entry>(this->entries);
}

bool Storage::create(entry *e) {
    // Try to assign a random, unique ID
    unsigned tries = 1000;  // (9x10^-300)% chance of not finding a unique value in a set of 2^31 entries, which is prohibitively large
    while (true) {
        // Assign a totally random ID
        e->id = (unsigned) rand();

        // Check for collisions
        bool found_match = false;
        for (unsigned i = 0; i < this->entries.size(); i++) {
            if (this->entries[i].id == e->id) {
                found_match = true;
                break;
            }
        }

        // If a match wasn't found, the ID is unique
        if (!found_match) {
            break;
        }

        // Note how many times we tried and failed
        if (--tries == 0) {
            // Failed too many times
            return false;
        }
    }

    // Store the entry in our table
    this->entries.push_back(*e);

    // Creation succeeded!
    return true;
}

bool Storage::get(entry *e) {
    for (unsigned i = 0; i < this->entries.size(); i++) {
        if (e->id == this->entries[i].id) {
            memcpy(e, &this->entries[i], sizeof(entry));
            return true;
        }
    }

    return false;
}

bool Storage::set(entry *e) {
    for (unsigned i = 0; i < this->entries.size(); i++) {
        if (e->id == this->entries[i].id) {
            memcpy(&this->entries[i], e, sizeof(entry));
            return true;
        }
    }

    return false;
}

bool Storage::remove(entry *e) {
    for (unsigned i = 0; i < this->entries.size(); i++) {
        if (e->id == this->entries[i].id) {
            entry e_i = this->entries[i];
            delete e_i.title;
            delete e_i.username;
            delete e_i.password;

            this->entries.erase(this->entries.begin() + i);
            return true;
        }
    }

    return false;
}

