//
// Created by Andrew Russell on 10/19/16.
//

#include "include/safe_string.h"

SafeString::SafeString(unsigned char *s, unsigned len) {
    // Generate a random key
    RAND_bytes(this->key, sizeof(this->key));

    // Assemble an encryption object
    EVP_CIPHER_CTX ctx;
    EVP_EncryptInit(&ctx, EVP_aes_128_cbc(), this->key, this->key);
    EVP_CIPHER_CTX_set_key_length(&ctx, sizeof(this->key));

    // Build a temporary container for the ciphertext
    int block_size = EVP_CIPHER_CTX_block_size(&ctx);
    unsigned char *cipher = (unsigned char *) malloc(len + block_size);

    // Encrypt the plaintext
    int cipher_len_update, cipher_len_final;
    EVP_EncryptUpdate(&ctx, cipher, &cipher_len_update, s, len);
    EVP_EncryptFinal(&ctx, cipher + cipher_len_update, &cipher_len_final);

    // Store data in the object
    this->data_len = (unsigned long) (cipher_len_update + cipher_len_final);
    this->p_data = (unsigned char *) malloc(this->data_len);
    memcpy(this->p_data, cipher, this->data_len);

    // Release the memory used by the temporary container
    free((void*)cipher);
}

SafeString::SafeString(SafeString *s) {
    // Copy and rekey a SafeString object
}

SafeString::~SafeString() {
    memset(&this->p_data, 0, this->data_len);
    memset(&this->key, 0, sizeof(this->key));
    free(this->p_data);
}

void SafeString::get_data(unsigned char *s) {
    // TODO: not yet implemented
}

unsigned SafeString::get_length() {
    // TODO: not yet implemented
}
