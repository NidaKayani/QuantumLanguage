#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

int popcount(unsigned int x) {
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
}

bool isPowerOfTwo(unsigned int x) { return x && !(x & (x - 1)); }

int main(void) {
    printf("%d %d %d\n", popcount(255), popcount(1024), popcount(0));
    printf("%d %d %d\n", isPowerOfTwo(64), isPowerOfTwo(65), isPowerOfTwo(1));
    uint8_t small = 250;
    small += 10;
    printf("%u\n", small);
    int32_t big = INT32_MAX;
    printf("%d\n", big);
    int flags = 0;
    flags |= 1 << 3;
    flags |= 1 << 0;
    flags &= ~(1 << 0);
    printf("%d %d\n", flags, (flags >> 3) & 1);
    bool done = false;
    printf("%d %d\n", done, !done);
    printf("%d %d\n", CHAR_BIT, SCHAR_MAX);
    unsigned x = 0xF0F0;
    printf("%x %x\n", x ^ 0xFFFF, x << 4);
    return 0;
}
