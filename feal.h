#ifndef FEAL_H
#define FEAL_H

#include "util.h"

uint64_t feal_8_encrypt(DATA_64 plaintext, DATA_32 (*subkeys)[8]);
uint64_t feal_8_decrypt(DATA_64 ciphertext, DATA_32 (*subkeys)[8]);

void test_feal(void);

#endif
