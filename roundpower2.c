#include <stdint.h>

/**
 * Rounds the number up to the next power of 2.
 */

uint64_t roundUpP2(uint64_t x)
{
    /** First we subtract one to avoid rounding up number that are already a power of 2.
     * The 'or' trick fills the number with ones starting from the most signficant 1 bit.
     * Then adding 1 to reach the next power of 2.
     *
     * Example: 10000001 ->subtract-> 10000000 -> 11000000 -> 11110000 -> 11111111 ->adding 1-> 100000000
     */
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