#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feal.h"
#include "util.h"

/*
 * Going with 8 rounds, since the cipher is plenty eak on 8 rounds, and because I don't want to stray far from documentation due to testing difficulties
 */

/*
 * Handwritten from documentation in Applied Cryptography
 */
uint8_t s0(const uint8_t a, const uint8_t b) {
    return ROTL((a + b) % 256, 2);
}

/*
 * Handwritten from documentation in Applied Cryptography
 */
uint8_t s1(const uint8_t a, const uint8_t b) {
    return ROTL((a + b + 1) % 256, 2);
}

/*
 * Handwritten from documentation in Applied Cryptography
 */
uint32_t f(DATA_32 a, DATA_16 b) {
    a.byte[1] ^= b.byte[0];
    a.byte[1] ^= a.byte[0];

    a.byte[2] ^= b.byte[1];
    a.byte[2] ^= a.byte[3];

    a.byte[1] = s1(a.byte[1], a.byte[2]);
    a.byte[0] = s0(a.byte[0], a.byte[1]);

    a.byte[2] = s0(a.byte[1], a.byte[2]);

    a.byte[3] = s1(a.byte[2], a.byte[3]);

    return a.dword;
}

/*
 * Handwritten from documentation in Applied Cryptography
 */
uint32_t fk(DATA_32 a, DATA_32 b) {
    a.byte[1] ^= a.byte[0];
    a.byte[2] ^= a.byte[3];

    a.byte[1] = s1(a.byte[1], a.byte[2] ^ b.byte[0]);
    a.byte[2] = s0(a.byte[2], a.byte[1] ^ b.byte[1]);

    a.byte[0] = s0(a.byte[0], a.byte[1] ^ b.byte[2]);
    a.byte[3] = s1(a.byte[3], a.byte[2] ^ b.byte[3]);

    return a.dword;
}

/*
 * Handwritten from documentation in Applied Cryptography
 * This I'm shaky on, it may not be 100% perfect
 */
void subkey_generation(DATA_64 key, DATA_32 (*out_keys)[8]) {
    uint32_t old_a = key.dword[0];

    key.dword[0] = fk((DATA_32){key.dword[0]}, (DATA_32){key.dword[1]});
    (*out_keys)[0].dword = key.dword[0];

    for (int i = 0; i < 7; ++i) {
        //Swap halves
        uint32_t tmp = key.dword[0];
        key.dword[0] = key.dword[1];
        key.dword[1] = tmp;

        uint32_t tmp2 = fk((DATA_32){key.dword[0]}, (DATA_32){key.dword[1] ^ old_a});
        (*out_keys)[i + 1].dword = tmp2;
        old_a = key.dword[0];
        key.dword[0] = tmp2;
    }
}

uint64_t feal_8_encrypt(DATA_64 plaintext, DATA_32 (*subkeys)[8]) {
    //Initial key xor
    plaintext.qword ^= (((uint64_t)(*subkeys)[4].dword) << 32) | (*subkeys)[5].dword;

    plaintext.dword[1] ^= plaintext.dword[0];
    plaintext.dword[0] ^= f((DATA_32){plaintext.dword[1]}, (DATA_16){(*subkeys)[0].word[0]});

    //Swap halves
    uint32_t tmp = plaintext.dword[0];
    plaintext.dword[0] = plaintext.dword[1];
    plaintext.dword[1] = tmp;

    for (int i = 1; i < 8; ++i) {
        plaintext.dword[0]
                ^= f((DATA_32){plaintext.dword[1]}, (DATA_16){(*subkeys)[i / 2].word[i % 2]});

        //Ignore swap on last round
        if (i != 7) {
            //Swap halves
            uint32_t tmp = plaintext.dword[0];
            plaintext.dword[0] = plaintext.dword[1];
            plaintext.dword[1] = tmp;
        }
    }

    //Final round xor
    plaintext.dword[1] ^= plaintext.dword[0];

    //Final key xor
    plaintext.qword ^= (((uint64_t)(*subkeys)[6].dword) << 32) | (*subkeys)[7].dword;

    return plaintext.qword;
}

uint64_t feal_8_decrypt(DATA_64 plaintext, DATA_32 (*subkeys)[8]) {
    //Final key xor
    plaintext.qword ^= (((uint64_t)(*subkeys)[6].dword) << 32) | (*subkeys)[7].dword;

    plaintext.dword[1] ^= plaintext.dword[0];
    plaintext.dword[0] ^= f((DATA_32){plaintext.dword[1]}, (DATA_16){(*subkeys)[3].word[1]});

    //Swap halves
    uint32_t tmp = plaintext.dword[0];
    plaintext.dword[0] = plaintext.dword[1];
    plaintext.dword[1] = tmp;

    for (int i = 6; i >= 0; --i) {
        plaintext.dword[0]
                ^= f((DATA_32){plaintext.dword[1]}, (DATA_16){(*subkeys)[i / 2].word[i % 2]});

        //Ignore swap on last round
        if (i != 0) {
            //Swap halves
            uint32_t tmp = plaintext.dword[0];
            plaintext.dword[0] = plaintext.dword[1];
            plaintext.dword[1] = tmp;
        }
    }

    //Final round xor
    plaintext.dword[1] ^= plaintext.dword[0];

    //Initial key xor
    plaintext.qword ^= (((uint64_t)(*subkeys)[4].dword) << 32) | (*subkeys)[5].dword;

    return plaintext.qword;
}

void test_feal(void) {
    DATA_64 pt;
    pt.qword = 0x0001020304050607;

    DATA_64 key;
    key.qword = 0x08090a0b0c0d0e0f;

    DATA_32 subkeys[8];
    subkey_generation(key, &subkeys);

    uint64_t ciphertext = feal_8_encrypt(pt, &subkeys);
    uint64_t plaintext = feal_8_decrypt((DATA_64){ciphertext}, &subkeys);

    if (plaintext == pt.qword) {
        printf("FEAL Decryption WORKS\n");
    } else {
        printf("FEAL Decryption FAILED %lu %lu\n", plaintext, pt.qword);
    }
}
