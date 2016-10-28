//
// Created by Andrew Russell on 10/19/16.
//

#include <iostream>
#include "include/storage.h"

Storage::Storage(SafeString *key) {
    // Extract key into array
    char *cp_key = (char *) malloc(key->get_max_length());
    memset(cp_key, 0, key->get_max_length());
    unsigned key_len;
    key->get_data((unsigned char *) cp_key, &key_len);

    // Key size checking
    if (key_len < sizeof(this->key))
        throw new runtime_error("Provided key was too short");

    // Copy the first 32 bytes into the key (for 256-bit AES)
    memcpy(this->key, cp_key, sizeof(this->key));
}

Storage::~Storage() {
    memset(this->key, 0, sizeof(this->key));
}

void Storage::load(SafeString *ss) {
    string s = decrypt(this->key, ss)->get_data();

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

    // If it decrypted and the digest still matched, we can be pretty sure it's
    // an okay file. Treat it like it's properly formatted. Worst case is it
    // segfaults.
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
        safe.erase(0, sizeof(len) + len);
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
        string s_len(ca_len, sizeof(ca_len));

        // Dump to output stream
        oss << s_len;
        oss << s;
    }

    // Compute and append SHA512 digest
    string s = oss.str();
    unsigned char digest[SHA512_DIGEST_LENGTH];
    SHA512((const unsigned char *) s.data(), s.length(), digest);

    oss << string((char *) digest, SHA512_DIGEST_LENGTH);
    s = oss.str();

    // Bundle and return the entries
    return encrypt(this->key, new SafeString(s));
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

SafeString *Storage::encrypt(unsigned char *key, SafeString *plain) {
    char iv[AES_BLOCK_SIZE];
    memset(iv, 0, AES_BLOCK_SIZE);

    string plain_text = plain->get_data();
    unsigned plain_text_len = (unsigned) plain_text.length();
    char *cipher_text = (char *) malloc(
            plain->get_max_length() + AES_BLOCK_SIZE);

    EVP_CIPHER_CTX *ctx;

    int len, cipher_text_len;

    if (!(ctx = EVP_CIPHER_CTX_new()))
        throw new runtime_error("Unable to initialize context");

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key,
                           (unsigned char *) iv) != 1)
        throw new runtime_error("Unable to initialize encryption");

    if (EVP_EncryptUpdate(ctx, (unsigned char *) cipher_text, &len,
                          (unsigned char *) plain_text.data(),
                          plain_text_len) != 1)
        throw new runtime_error("Encryption update failed");
    cipher_text_len = len;

    if (EVP_EncryptFinal_ex(ctx, (unsigned char *) cipher_text + len, &len)
        != 1)
        throw new runtime_error("Encryption finalization failed");
    cipher_text_len += len;

    EVP_CIPHER_CTX_free(ctx);

    SafeString *ss = new SafeString((unsigned char *) cipher_text,
                                    (unsigned) cipher_text_len);

    free(cipher_text);

    return ss;
}

SafeString *Storage::decrypt(unsigned char *key, SafeString *cipher) {
    char iv[AES_BLOCK_SIZE];
    memset(iv, 0, AES_BLOCK_SIZE);

    string cipher_text = cipher->get_data();
    unsigned cipher_text_len = (unsigned) cipher_text.length();
    char *plain_text = (char *) malloc(
            cipher->get_max_length() + AES_BLOCK_SIZE);

    EVP_CIPHER_CTX *ctx;

    int len, plain_text_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        throw new runtime_error("Unable to initialize context");

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key,
                           (unsigned char *) iv) != 1)
        throw new runtime_error("Unable to initialize decryption");

    if (EVP_DecryptUpdate(ctx, (unsigned char *) plain_text, &len,
                          (unsigned char *) cipher_text.data(),
                          cipher_text_len) != 1)
        throw new runtime_error("Decryption update failed");
    plain_text_len = len;

    if (EVP_DecryptFinal_ex(ctx, (unsigned char *) plain_text + len, &len) != 1)
        throw new runtime_error("Decryption finalization failed");
    plain_text_len += len;

    SafeString *ss = new SafeString((unsigned char *) plain_text,
                                    (unsigned) plain_text_len);

    EVP_CIPHER_CTX_free(ctx);
    free(plain_text);

    return ss;
}

SafeString *Storage::generate_key() {
    // Setup
    unsigned key_len = 32;
    unsigned char *local_key = (unsigned char *) malloc(key_len);

    // Get cryptographically-random bytes
    RAND_bytes(local_key, key_len);

    // Convert to SafeString and deallocate memory
    SafeString *ss = new SafeString(local_key, key_len);
    free(local_key);

    return ss;
}

bool Storage::search(entry *e, SafeString *title) {
    for (unsigned i = 0; i < this->entries.size(); i++) {
        // See if there's a match in the title
        unsigned long title_match = this->entries[i].title->get_data()
                .find(title->get_data());

        // Copy the data into the entry if there's a match
        if (title_match != string::npos) {
            memcpy(e, &this->entries[i], sizeof(entry));
            return true;
        }
    }

    return false;
}
