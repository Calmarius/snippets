#include <stdint.h>

uint64_t roundUpP2(uint64_t x)
{
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    x++;

    return x;
}

#include <assert.h>

int main()
{
    assert(roundUpP2(0) == 0);
    assert(roundUpP2(1) == 1);
    assert(roundUpP2(2) == 2);
    assert(roundUpP2(3) == 4);
    assert(roundUpP2(69) == 128);
    assert(roundUpP2(50) == 64);
    assert(roundUpP2(0x7FFFFFFF) == 0x80000000);
    assert(roundUpP2(0x40000000) == 0x40000000);

    return 0;
}