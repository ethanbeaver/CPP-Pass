//
// Created by Andrew Russell on 10/19/16.
//

#include "include/safe_string.h"

SafeString::SafeString(unsigned char *s, unsigned len) {
    // TODO: Make this operation atomic.

    // Generate a random key
    RAND_bytes(this->key, sizeof(this->key));

    // Assemble an encryption object
    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new()))
        throw new runtime_error("Abstract cipher object failed to initialize");
    if (EVP_EncryptInit(ctx, EVP_aes_128_cbc(), this->key, this->key) != 1)
        throw new runtime_error("AES encryption object failed to initialize");

    // Build a temporary container for the ciphered text
    int block_size = EVP_CIPHER_CTX_block_size(ctx);
    unsigned char *cipher = (unsigned char *) malloc(len + block_size);
    if (cipher == NULL)
        throw new runtime_error(
                "Unable to allocate extra memory for cipher text");

    // Encrypt the plaintext
    int cipher_len_update, cipher_len_final;
    if (EVP_EncryptUpdate(ctx, cipher, &cipher_len_update, s, len) != 1)
        throw new runtime_error("Encryption update failed");
    if (EVP_EncryptFinal(ctx, cipher + cipher_len_update, &cipher_len_final) !=
        1)
        throw new runtime_error("Encryption finalization failed");

    // Store data in the object
    this->data_len = (size_t) (cipher_len_update + cipher_len_final);
    this->p_data = malloc(this->data_len);
    if (this->p_data == NULL)
        throw new runtime_error(
                "Unable to allocate exact memory for cipher text");
    memcpy(this->p_data, cipher, this->data_len);

    // Release the memories
    memset(cipher, 0, len + block_size);
    free((void *) cipher);
    EVP_CIPHER_CTX_free(ctx);
}

SafeString::SafeString(SafeString *ss) {
    // TODO: Implement if I have time
}

SafeString::~SafeString() {
    // Erase and release the cipher text
    memset(&this->p_data, 0, this->data_len);
    free(this->p_data);
    // Erase the AES key
    memset(&this->key, 0, sizeof(this->key));
}

void SafeString::get_data(unsigned char *s, unsigned *len) {
    // TODO: Make this operation atomic.

    // Assemble a decryption object
    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new()))
        throw new runtime_error("Abstract cipher object failed to initialize");
    if (EVP_DecryptInit(ctx, EVP_aes_128_cbc(), this->key, this->key) != 1)
        throw new runtime_error("AES decryption object failed to initialize");

    // Build a temporary container for the plain text
    unsigned char *plain = (unsigned char *) malloc(this->data_len);
    if (plain == NULL)
        throw new runtime_error(
                "Unable to allocate extra memory for plain text");
    memset(plain, 0, this->data_len);

    // Decrypt the cipher text
    int plain_len_update, plain_len_final = 0;
    if (EVP_DecryptUpdate(ctx, plain, &plain_len_update,
                          (const unsigned char *) this->p_data,
                          (int) this->data_len) != 1)
        throw new runtime_error("Encryption update failed");
    if (EVP_DecryptFinal_ex(ctx, plain + plain_len_update, &plain_len_final) !=
        1)
        throw new runtime_error("Encryption finalization failed");
    int plain_len = plain_len_update + plain_len_final;

    // Give the result to the caller
    memcpy(s, plain, (size_t) plain_len);
    *len = (unsigned) plain_len;

    // Clean and release the memories
    memset(plain, 0, this->data_len);
    free((void *) plain);
    EVP_CIPHER_CTX_free(ctx);
}

unsigned SafeString::get_max_length() {
    return (unsigned) this->data_len;
}
