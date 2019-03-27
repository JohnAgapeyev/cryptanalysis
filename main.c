#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "util.h"
#include "feal.h"

int main(int argc, char **argv) {
    uint8_t x = 0xab;

    uint8_t y = ROTL(x, 3);
    uint8_t z = ROTR(y, 3);

    printf("Input %d, Output %d, Again %d\n", x, y, z);

    uint16_t a = 0xabcd;

    uint16_t b = ROTL(a, 3);
    uint16_t c = ROTR(b, 3);

    printf("Input %d, Output %d, Again %d\n", a, b, c);
    uint32_t d = 0x00010203;

    uint32_t e = ROTL(d, 3);
    uint32_t f = ROTR(e, 3);

    printf("Input %d, Output %d, Again %d\n", d, e, f);

    uint64_t g = 0x0001020304050607;

    uint64_t h = ROTL(g, 3);
    uint64_t i = ROTR(h, 3);

    printf("Input %lu, Output %lu, Again %lu\n", g, h, i);

    test_feal();

    return EXIT_SUCCESS;
}
