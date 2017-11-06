#include <stdint.h>
#include <stddef.h>

/**
 * The FNV hash is a quick hash function that should be used to implement hash tables.
 * This hash is NOT a cryptographic hash and shouldn't be used for such purposes.
 */

/**
 * buf: the data to hash.
 * n: the number of bytes to hash.
 */
uint32_t fnv32(const uint8_t *buf, size_t n)
{
    uint32_t hash = 0x811c9dc5;
    size_t i;

    for (i = 0; i < n; i++)
    {
        hash ^= buf[i];
        hash *= 16777619;
    }

    return hash;
}


uint64_t fnv64(const uint8_t *buf, size_t n)
{
    uint64_t hash = 0xcbf29ce484222325;
    size_t i;

    for (i = 0; i < n; i++)
    {
        hash ^= buf[i];
        hash *= 1099511628211;
    }

    return hash;
}

/* TEST */

#ifdef UNIT_TEST

#include <assert.h>
#include <string.h>

int main()
{
    const char *testStr = "Hello world!";
    size_t n = strlen(testStr);
    
    assert(fnv32((const uint8_t*)testStr, n) == 0x7a78f512);
    assert(fnv64((const uint8_t*)testStr, n) == 0x9e527e14572072d2);
}

#endif