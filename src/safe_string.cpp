//
// Created by Andrew Russell on 10/19/16.
//

#include "include/safe_string.h"

SafeString::SafeString(string *s) {
    RAND_bytes(this->key, sizeof(this->key));

    EVP_CIPHER_CTX ctx;
    EVP_EncryptInit(&ctx, EVP_aes_128_cbc(), this->key, this->key);
    EVP_CIPHER_CTX_set_key_length(&ctx, sizeof(this->key));

    int block_size = EVP_CIPHER_CTX_block_size(&ctx);
    size_t plain_len = s->length();
    size_t cipher_len = plain_len + ((plain_len - 1) % block_size + 1);

    unsigned char *c_in = (unsigned char *) s->data();
    this->p_data = (unsigned char *) malloc(cipher_len);

    int cipher_len_actual;
    EVP_EncryptUpdate(&ctx, this->p_data, &cipher_len_actual, c_in, (int) s->length());
    EVP_EncryptFinal(&ctx, this->p_data, &cipher_len_actual);
    this->data_len = (unsigned long) cipher_len_actual;
}

SafeString::SafeString(SafeString *s) {

}

SafeString::~SafeString() {
    memset(&this->p_data, 0, this->data_len);
    memset(&this->key, 0, sizeof(this->key));
    free(this->p_data);
}

void SafeString::get(string *s) {
    // TODO: not yet implemented
}