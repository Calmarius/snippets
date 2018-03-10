#ifdef UNIT_TEST

#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    uint32_t *words; /* Allocated so we can test for leaks. */
    size_t n;
} BigInt;

/* With assert for testing.*/
uint32_t getWord(const BigInt *bi, size_t i)
{
    assert(i < bi->n);
    return bi->words[i];
}

void setWord(const BigInt *bi, size_t i, uint32_t w)
{
    assert(i < bi->n);
    bi->words[i] = w;
}

int copyBigint(BigInt *dst, const BigInt *src)
{
    size_t sz = src->n * sizeof(*src->words);

    free(dst->words);
    dst->words = malloc(sz);
    if (!dst->words) return -1;
    memcpy(dst->words, src->words, sz);
    dst->n = src->n;

    return 0;
}

int allocBigint(BigInt *dst, size_t n)
{
    free(dst->words);
    dst->words = malloc(n * sizeof(*dst->words));
    if (!dst->words) return -1;
    dst->n = n;

    return 0;
}

BigInt genSimpleBigint(uint32_t x)
{
    BigInt b;

    b.words = malloc(sizeof(*b.words));
    b.n = 1;

    b.words[0] = x; /* Not checked for NULL, it should fail loud. */

    return b;
}

#define WORD_TYPE uint32_t
#define WORD_BITS 32
#define BIGINT_TYPE BigInt
#define GETWORD(bi, i) getWord((bi), (i))
#define SETWORD(bi, i, w) setWord(bi, i ,w)
#define GETNWORDS(bi) ((bi)->n)
#define ZERO_BIGINT(bi) memset(((bi)->words), 0, (bi)->n * sizeof(*(bi)->words))
#define SETNWORDS(bi, nWords) ((bi)->n = (nWords))
#define NUM_THEORY
#define COPY_BIGINT(dst, src) {if (copyBigint(dst, src)) goto cleanup;}
#define ALLOC_BIGINT(bi, nWords) {if (allocBigint((bi), nWords)) goto cleanup; }
#define INIT_EMPTY(bi) {(bi)->words = NULL; (bi)->n = 0;}
#define DEINIT_BIGINT(bi) (free((bi)->words))
#define DEFINE_STUFF
#include "arbitrary_precision/bigint.h"

uint32_t one = 1;
const BigInt g_one = {&one, 1};

#define BIGNUM BigInt
#define BIGNUM_RELEASE(bi) (free((bi)->words))
#define MUL(a, b, out) mulEx(a, b, out)
#define SUB(a, b, out) sub(a, b, out)
#define XGCD(a, b, x, y, gcd) gcdExtendedEuclidean(a, b, x, y, gcd)
#define LCM(a, b, out) lcm(a, b, out)
#define EQUAL(a, b) equal(a, b)
#define ONE (&g_one)
#define DECLARE_STUFF
#define DEFINE_STUFF
#include "crypto/pubkey/rsa.h"

int main()
{
    {
        BigInt p = genSimpleBigint(61);
        BigInt q = genSimpleBigint(53);
        BigInt e = genSimpleBigint(17);
        BigInt modulus;
        BigInt privkey;
        int res;

        res = rsaMakeKeys(&p, &q, &e, &modulus, &privkey);
        assert(res == 0);
        assert(modulus.n == 2);
        assert(modulus.words[0] == 780);
        assert(modulus.words[1] == 0);
        assert(privkey.n == 2);
        assert(privkey.words[0] == 413);
        assert(privkey.words[1] == 0);




        free(p.words);
        free(q.words);
        free(e.words);
    }

    printf("RSA OK. %s %s\n", __DATE__, __TIME__);

    return 0;
}

#endif