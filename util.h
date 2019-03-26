#ifndef UTIL_H
#define UTIL_H

#include <limits.h>

#define ROTL(a,b) (((a) << (b)) | ((a) >> (sizeof(a) * CHAR_BIT - (b))))
#define ROTR(a,b) (((a) >> (b)) | ((a) << (sizeof(a) * CHAR_BIT - (b))))

void foo(void);

#endif
