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
} DATA;

#endif
