//
// Created by Andrew Russell on 10/19/16.
//

#ifndef CPP_PASS_STORAGE_H
#define CPP_PASS_STORAGE_H

#include "safe_string.h"
#include "entry.h"
#include <sstream>

#include "openssl/rand.h"
#include "openssl/evp.h"
#include "openssl/aes.h"
#include "openssl/sha.h"


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
     * Fetch a particular entry which contains a username, password, and title
     * given an ID.
     *
     * @param e
     * @return True on success, false on failure.
     */
    bool get(entry *e);

    /**
     * Update data in a particular entry.
     *
     * It's important to not touch the ID. It'll overwrite any IDs that do match
     * but will fail on IDs that don't exist.
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

    /**
     * Search through the titles and username. Returns the first match.
     *
     * @param e
     * @return True on entry found, false on entry not found.
     */
    bool search(entry *e, SafeString *title);

    /**
     * Generate a new random key. Output should be cryptographically random.
     *
     * @return
     */
    static SafeString *generate_key();

    /**
     * Used only as a debug tool. Never to be used in production.
     *
     * @param s
     */
    static void dump_hex(string *s);

protected:

    /**
     * Encrypt arbitrary strings without an IV.
     *
     * @param key 32 byte key for AES-256
     * @param plain Secured string of arbitrary data
     * @return String of encrypted data (cipher text)
     */
    static SafeString *encrypt(unsigned char *key, SafeString *plain);

    /**
     * Decrypt arbitrary ciphers without an IV.
     *
     * @param key 32 byte key for AES-256
     * @param cipher Secured string of ciphered data
     * @return String of decrypted data (plain text)
     */
    static SafeString *decrypt(unsigned char *key, SafeString *cipher);

private:

    /**
     * A whole big pile of entries, hot off the press.
     */
    vector<entry> entries;

    /**
     * Plaintext key. This does not need to be encrypted since the file it came
     * from is not encrypted.
     */
    unsigned char key[32];
};

#endif //CPP_PASS_STORAGE_H
