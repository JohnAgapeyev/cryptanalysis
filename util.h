#ifndef UTIL_H
#define UTIL_H

#include <limits.h>
#include <stdint.h>

#define ROTL(a, b) (((a) << (b)) | ((a) >> (sizeof(a) * CHAR_BIT - (b))))
#define ROTR(a, b) (((a) >> (b)) | ((a) << (sizeof(a) * CHAR_BIT - (b))))

typedef union {
    uint64_t qword;
    uint32_t dword[2];
    uint16_t word[4];
    uint8_t byte[8];
} DATA_64;

typedef union {
    uint64_t qword[2];
    uint32_t dword[4];
    uint16_t word[8];
    uint8_t byte[16];
} DATA_128;

typedef union {
    uint64_t qword[4];
    uint32_t dword[8];
    uint16_t word[16];
    uint8_t byte[32];
} DATA_256;

#endif
