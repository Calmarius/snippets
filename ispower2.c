

/**
 * Checks if the number is a power of 2.
 */
int ispower2(unsigned x)
{
    /**
     * When you subtract 1 from a number it's most significant 1 bit usually remains set.
     * For example: 100011 --> 100010.
     * But when it's a power of 2 the most significant 1 bit will get zeroed: 100000 -> 011111.
     * Moreover in this case the new number and the old one won't have any 1 bits set in the same places.
     * This allows us check whether a number is a power of 2 just and &-ing together the number and the number-1.
     * If the result of the & is 0 the number is a power of 2.
     *
     * Since this test would say 0 is a power of 2, we need to care about that as well.
     */
    return x && !(x & (x-1));
}

#ifdef UNIT_TEST

/* Unit test. */
#include <assert.h>

int main()
{
    assert(!ispower2(0));
    assert(ispower2(1));
    assert(ispower2(2));
    assert(!ispower2(3));
    assert(ispower2(4));
    assert(!ispower2(5));
    assert(!ispower2(6));
    assert(!ispower2(7));
    assert(ispower2(8));
    assert(!ispower2(9));
    assert(!ispower2(10));
    assert(!ispower2(11));
    assert(!ispower2(12));
    assert(!ispower2(13));
    assert(!ispower2(14));
    assert(!ispower2(15));
    assert(ispower2(16));
    assert(ispower2(0x80000000));
    assert(!ispower2(0x80100000));
    assert(!ispower2(0x7FFFFFFF));
    assert(!ispower2(0xFFFFFFFF));
    return 0;
}

#endif