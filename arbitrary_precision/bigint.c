#ifdef UNIT_TEST

#include <inttypes.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define WORD_COUNT 8

typedef struct
{
    uint32_t words[WORD_COUNT];
    size_t n;
    void *dummy; /* Something to alert about memory leaks in valgrind. */
} BigInt;

void dump(const BigInt *bi, const char *header)
{
    size_t n = bi->n;

    printf("%s", header);
    while (n --> 0)
    {
        printf("%08x ", bi->words[n]);
    }
    printf("\n");
}

#define BIGINT_TYPE BigInt
#define GETNWORDS(bi) ((bi)->n)
#define SETNWORDS(bi, nWords) ((bi)->n = (nWords))

#define BOUNDS_CHECK

#ifdef BOUNDS_CHECK
    #define ASSERT(x) assert(x)

    uint32_t getWord(const BigInt *bi, size_t i)
    {
        assert(i < WORD_COUNT);
        assert(i < bi->n);
        return bi->words[i];
    }

    #define GETWORD(bi, i) (getWord((bi), (i)))

    void setWord(BigInt *bi, size_t i, uint32_t word)
    {
        assert(i < WORD_COUNT);
        assert(i < bi->n);
        bi->words[i] = word;
    }

    #define SETWORD(bi, i, word) (setWord((bi), (i), (word)))

#else
    #define ASSERT
    #define GETWORD(bi, i) ((bi)->words[i])
    #define SETWORD(bi, i, word) ((bi)->words[i] = (word))
#endif

#define WORD_TYPE uint32_t
#define WORD_BITS 32
#define NUM_THEORY
#define COPY_BIGINT(dst, src) {free((dst)->dummy); *(dst) = *(src); (dst)->dummy = malloc(10); }
#define ALLOC_BIGINT(bi, nWords) {free((bi)->dummy); (bi)->n = (nWords); assert((bi)->n <= WORD_COUNT); (bi)->dummy = malloc(10); if (!(bi)->dummy) goto cleanup;}
#define INIT_EMPTY(bi) {(bi)->n = 0; (bi)->dummy = NULL; }
#define DEINIT_BIGINT(bi)  {free((bi)->dummy);}
#define ZERO_BIGINT(bi) (memset(bi, 0, (bi)->n * sizeof((bi)->words[0])))
#define DUMP_BIGINT(x, y) dump(x, y)
#define DECLARE_STUFF
#define DEFINE_STUFF
#include "bigint.h"

typedef struct
{
    uint32_t words[8];
} Stuff256;

#define BIGINT_TYPE Stuff256
#define WORD_TYPE uint32_t
#define WORD_BITS 32
#define GETWORD(bi, i) ((bi)->words[i])
#define SETWORD(bi, i, word) ((bi)->words[i] = (word))
#define GETNWORDS(bi) 8
#define SETNWORDS(bi, words) (void)(words)
#define ZERO_BIGINT(bi) (memset(bi, 0, sizeof((bi)->words)))
#define PREFIX i256_
#define DECLARE_STUFF
#define DEFINE_STUFF
#include "bigint.h"

#include <stdio.h>
#include <signal.h>

int main()
{
    uint32_t high, low;
    uint32_t result;
    int carry;

    carry = addDigit(0xFFFFFFFF, 0xFFFFFFFF, &result);
    assert(carry == 1);
    assert(result == 0xFFFFFFFE);

    mulDigit(0xFFFFFFFF, 0xFFFFFFFF, &high, &low);
    assert(high == 0xFFFFFFFE);
    assert(low == 1);

    {
        BigInt zero = {{0, 0, 0, 0}, 4, NULL};
        BigInt nonzero = {{0, 1, 0, 0}, 4, NULL};

        assert(isZero(&zero));
        assert(!isZero(&nonzero));
        free(zero.dummy);
        free(nonzero.dummy);
    }
    {
        BigInt A = {{0x87654321, 0x2468ACE0, 0x369CF258, 0x48C048C0}, 4, NULL};
        BigInt B = {{0x88888888, 0xF2222222, 0x33333333, 0xC4444444}, 4, NULL};
        BigInt C = {0};

        /* Normal addition */
        carry = add(&A, &B, &C);
        assert(C.words[0] == 0x0FEDCBA9);
        assert(C.words[1] == 0x168ACF03);
        assert(C.words[2] == 0x69D0258C);
        assert(C.words[3] == 0x0D048D04);
        assert(C.n == 4);
        assert(carry == 1);

        B.n = 2;

        /* Test when one of the numbers is shorter. */
        carry = add(&A, &B, &C);
        assert(C.words[0] == 0x0FEDCBA9);
        assert(C.words[1] == 0x168ACF03);
        assert(C.words[2] == 0x369CF259);
        assert(C.words[3] == 0x48C048C0);
        assert(C.n == 4);
        assert(carry == 0);
    }
    {
        BigInt A = {{0xFFFFFFFF, 0xEEEEEEEE, 0xDDDDDDDD, 0xCCCCCCCC}, 4, NULL};
        BigInt B = {{0xFFFFFFFF, 0xEEEEEEEE, 0xDDDDDDDD, 0xCCCCCCCC}, 4, NULL};
        BigInt C = {0};

        carry = add(&A, &B, &C);
        assert(C.words[0] == 0xFFFFFFFE);
        assert(C.words[1] == 0xDDDDDDDD);
        assert(C.words[2] == 0xBBBBBBBB);
        assert(C.words[3] == 0x99999999);
        assert(C.n == 4);
        assert(carry == 1);

        /* Check if it works, when output matches input, (it should).*/
        carry = add(&A, &B, &A);
        assert(A.words[0] == 0xFFFFFFFE);
        assert(A.words[1] == 0xDDDDDDDD);
        assert(A.words[2] == 0xBBBBBBBB);
        assert(A.words[3] == 0x99999999);
        assert(C.n == 4);
        assert(carry == 1);
    }
    {
        BigInt A = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 4, NULL};
        BigInt B = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 4, NULL};
        BigInt C = {0};
        int res;

        /* Exercise all sorts of carry and overflow. */
        C.n = 8;
        res = mul(&A, &B, &C);
        assert(res == 0);
        assert(C.words[0] == 0x00000001);
        assert(C.words[1] == 0x00000000);
        assert(C.words[2] == 0x00000000);
        assert(C.words[3] == 0x00000000);
        assert(C.words[4] == 0xFFFFFFFE);
        assert(C.words[5] == 0xFFFFFFFF);
        assert(C.words[6] == 0xFFFFFFFF);
        assert(C.words[7] == 0xFFFFFFFF);
        assert(C.n == 8);
    }

    {
        BigInt A = {{0x12345678, 0x12345678, 0x12345678, 0x12345678}, 4, NULL};
        BigInt B = {{0x87654321, 0x87654321, 0x87654321, 0x87654321}, 4, NULL};
        BigInt C = {0};
        int res;

        /* Randomized test. */
        C.n = 8;
        res = mul(&A, &B, &C);
        assert(res == 0);
        assert(C.words[0] == 0x70b88d78);
        assert(C.words[1] == 0xeb11e7f5);
        assert(C.words[2] == 0x656b4272);
        assert(C.words[3] == 0xdfc49cf0);
        assert(C.words[4] == 0x78acdc7d);
        assert(C.words[5] == 0xfe538200);
        assert(C.words[6] == 0x83fa2782);
        assert(C.words[7] == 0x09a0cd05);
        assert(C.n == 8);

        /* Test if truncation works in general. */
        C.n = 4;
        res = mul(&A, &B, &C);
        assert(res == 1);
        assert(C.words[0] == 0x70b88d78);
        assert(C.words[1] == 0xeb11e7f5);
        assert(C.words[2] == 0x656b4272);
        assert(C.words[3] == 0xdfc49cf0);
        assert(C.n == 4);
    }
    {
        BigInt A = {{1, 1, 0, 0}, 4, NULL};
        BigInt B = {{1, 1, 1, 0}, 4, NULL};
        BigInt C;
        int res;

        /* Nontruncation case. The result fits. */
        C.n = 4;
        res = mul(&A, &B, &C);
        assert(res == 0);
        assert(C.n == 4);
        assert(C.words[0] == 0x00000001);
        assert(C.words[1] == 0x00000002);
        assert(C.words[2] == 0x00000002);
        assert(C.words[3] == 0x00000001);
    }
    {
        BigInt A = {{1, 1, 1, 1}, 4, NULL};
        BigInt B = {{1, 1, 1, 1}, 4, NULL};
        BigInt C;
        int res;

        /* Truncation. Nonzero low word case. */
        C.n = 4;
        res = mul(&A, &B, &C);
        assert(res == 1);
        assert(C.n == 4);
        assert(C.words[0] == 0x00000001);
        assert(C.words[1] == 0x00000002);
        assert(C.words[2] == 0x00000003);
        assert(C.words[3] == 0x00000004);
    }
    {
        BigInt A = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 4, NULL};
        BigInt B = {{2, 0, 0, 0}, 4, NULL};
        BigInt C;
        int res;

        /* Truncation. Nonzero high word case. */
        C.n = 4;
        res = mul(&A, &B, &C);
        assert(res == 1);
        assert(C.n == 4);
        assert(C.words[0] == 0xFFFFFFFE);
        assert(C.words[1] == 0xFFFFFFFF);
        assert(C.words[2] == 0xFFFFFFFF);
        assert(C.words[3] == 0xFFFFFFFF);
    }
    {
        BigInt A = {{0x00000001, 0x00000001, 0, 0}, 4, NULL};
        BigInt B = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 4, NULL};
        BigInt C;
        int res;

        /* Truncation. Carry to beyond case. */
        C.n = 4;
        res = mul(&A, &B, &C);
        assert(res == 1);
        assert(C.n == 4);
        assert(C.words[0] == 0xFFFFFFFF);
        assert(C.words[1] == 0xFFFFFFFE);
        assert(C.words[2] == 0xFFFFFFFF);
        assert(C.words[3] == 0xFFFFFFFF);
    }
    {
        BigInt A = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 4, NULL};
        BigInt B = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 4, NULL};
        BigInt C = {0};
        int borrow;

        borrow = sub(&A, &B, &C);

        assert(C.words[0] == 0xFFFFFFFF);
        assert(C.words[1] == 0xFFFFFFFF);
        assert(C.words[2] == 0xFFFFFFFF);
        assert(C.words[3] == 0xFFFFFFFF);
        assert(C.n == 4);
        assert(borrow == 1);
    }
    {
        BigInt A = {{0x12345678, 0x12345678, 0x12345678, 0x12345678}, 4, NULL};
        BigInt B = {{0x87654321, 0x87654321, 0x87654321, 0x87654321}, 4, NULL};
        int borrow;

        borrow = sub(&A, &B, &A);

        assert(A.words[0] == 0x8ACF1357);
        assert(A.words[1] == 0x8ACF1356);
        assert(A.words[2] == 0x8ACF1356);
        assert(A.words[3] == 0x8ACF1356);
        assert(A.n == 4);
        assert(borrow == 1);
    }
    {
        BigInt A = {{0x12345678, 0x22222222, 0x33333333, 0x44444444}, 4, NULL};
        BigInt B = {0};

        shl(&A, &B, 1);
        assert(B.words[0] == 0x2468ACF0);
        assert(B.words[1] == 0x44444444);
        assert(B.words[2] == 0x66666666);
        assert(B.words[3] == 0x88888888);
        assert(B.n == 4);

        shl(&A, &B, 2);
        assert(B.words[0] == 0x48D159E0);
        assert(B.words[1] == 0x88888888);
        assert(B.words[2] == 0xCCCCCCCC);
        assert(B.words[3] == 0x11111110);
        assert(B.n == 4);

        shl(&A, &B, 3);
        assert(B.words[0] == 0x91A2B3C0);
        assert(B.words[1] == 0x11111110);
        assert(B.words[2] == 0x99999999);
        assert(B.words[3] == 0x22222221);
        assert(B.n == 4);

        shl(&A, &B, 4);
        assert(B.words[0] == 0x23456780);
        assert(B.words[1] == 0x22222221);
        assert(B.words[2] == 0x33333332);
        assert(B.words[3] == 0x44444443);
        assert(B.n == 4);

        shl(&A, &B, 16);
        assert(B.words[0] == 0x56780000);
        assert(B.words[1] == 0x22221234);
        assert(B.words[2] == 0x33332222);
        assert(B.words[3] == 0x44443333);
        assert(B.n == 4);

        shl(&A, &B, 32);
        assert(B.words[0] == 0x00000000);
        assert(B.words[1] == 0x12345678);
        assert(B.words[2] == 0x22222222);
        assert(B.words[3] == 0x33333333);
        assert(B.n == 4);

        shl(&A, &B, 48);
        assert(B.words[0] == 0x00000000);
        assert(B.words[1] == 0x56780000);
        assert(B.words[2] == 0x22221234);
        assert(B.words[3] == 0x33332222);
        assert(B.n == 4);

        /* Test self shift */
        shl(&A, &A, 16);
        assert(A.words[0] == 0x56780000);
        assert(A.words[1] == 0x22221234);
        assert(A.words[2] == 0x33332222);
        assert(A.words[3] == 0x44443333);
        assert(A.n == 4);
    }
    {
        BigInt A = {{0x12345678, 0x22222222, 0x33333333, 0x44444444}, 4, NULL};
        BigInt B = {0};

        shr(&A, &B, 1);
        assert(B.words[0] == 0x091A2B3C);
        assert(B.words[1] == 0x91111111);
        assert(B.words[2] == 0x19999999);
        assert(B.words[3] == 0x22222222);
        assert(B.n == 4);

        shr(&A, &B, 2);
        assert(B.words[0] == 0x848D159E);
        assert(B.words[1] == 0xC8888888);
        assert(B.words[2] == 0x0CCCCCCC);
        assert(B.words[3] == 0x11111111);
        assert(B.n == 4);

        shr(&A, &B, 3);
        assert(B.words[0] == 0x42468ACF);
        assert(B.words[1] == 0x64444444);
        assert(B.words[2] == 0x86666666);
        assert(B.words[3] == 0x08888888);
        assert(B.n == 4);

        shr(&A, &B, 4);
        assert(B.words[0] == 0x21234567);
        assert(B.words[1] == 0x32222222);
        assert(B.words[2] == 0x43333333);
        assert(B.words[3] == 0x04444444);
        assert(B.n == 4);

        shr(&A, &B, 16);
        assert(B.words[0] == 0x22221234);
        assert(B.words[1] == 0x33332222);
        assert(B.words[2] == 0x44443333);
        assert(B.words[3] == 0x00004444);
        assert(B.n == 4);

        shr(&A, &B, 32);
        assert(B.words[0] == 0x22222222);
        assert(B.words[1] == 0x33333333);
        assert(B.words[2] == 0x44444444);
        assert(B.words[3] == 0x00000000);
        assert(B.n == 4);

        shr(&A, &B, 48);
        assert(B.words[0] == 0x33332222);
        assert(B.words[1] == 0x44443333);
        assert(B.words[2] == 0x00004444);
        assert(B.words[3] == 0x00000000);
        assert(B.n == 4);

        /* Test self shift */
        shr(&A, &A, 16);
        assert(A.words[0] == 0x22221234);
        assert(A.words[1] == 0x33332222);
        assert(A.words[2] == 0x44443333);
        assert(A.words[3] == 0x00004444);
        assert(A.n == 4);

        free(A.dummy);
        free(B.dummy);
    }
    {
        BigInt A = {{0x12345678, 0x9ABCDEF0, 0x0FEDCBA9, 0x87654321}, 4, NULL};
        BigInt B = {{0xCCCCCCCC, 0x33333333, 0x55555555, 0xAAAAAAAA}, 4, NULL};
        BigInt C = {0};
        BigInt bakA, bakB;

        /* AND */
        bakA = A;
        bakB = B;
        C.n = 4;
        /* Normal */
        and(&A, &B, &C);
        assert(C.words[0] == 0x00044448);
        assert(C.words[1] == 0x12301230);
        assert(C.words[2] == 0x05454101);
        assert(C.words[3] == 0x82200220);
        assert(C.n == 4);
        /* Different lengths */
        B.n = 2;
        and(&A, &B, &C);
        assert(C.words[0] == 0x00044448);
        assert(C.words[1] == 0x12301230);
        assert(C.words[2] == 0x00000000);
        assert(C.words[3] == 0x00000000);
        assert(C.n == 4);
        B.n = 4;
        /* Self assign */
        and(&bakA, &bakB, &bakA);
        assert(bakA.words[0] == 0x00044448);
        assert(bakA.words[1] == 0x12301230);
        assert(bakA.words[2] == 0x05454101);
        assert(bakA.words[3] == 0x82200220);
        assert(bakA.n == 4);

        /* OR */
        bakA = A;
        bakB = B;
        /* Normal */
        or(&A, &B, &C);
        assert(C.words[0] == 0xDEFCDEFC);
        assert(C.words[1] == 0xBBBFFFF3);
        assert(C.words[2] == 0x5FFDDFFD);
        assert(C.words[3] == 0xAFEFEBAB);
        assert(C.n == 4);
        /* Different lengths */
        B.n = 2;
        or(&A, &B, &C);
        assert(C.words[0] == 0xDEFCDEFC);
        assert(C.words[1] == 0xBBBFFFF3);
        assert(C.words[2] == 0x0FEDCBA9);
        assert(C.words[3] == 0x87654321);
        assert(C.n == 4);
        B.n = 4;
        /* Self assign */
        or(&bakA, &bakB, &bakA);
        assert(bakA.words[0] == 0xDEFCDEFC);
        assert(bakA.words[1] == 0xBBBFFFF3);
        assert(bakA.words[2] == 0x5FFDDFFD);
        assert(bakA.words[3] == 0xAFEFEBAB);
        assert(bakA.n == 4);

        /* XOR */
        bakA = A;
        bakB = B;
        /* Normal */
        xor(&A, &B, &C);
        assert(C.words[0] == 0xDEF89AB4);
        assert(C.words[1] == 0xA98FEDC3);
        assert(C.words[2] == 0x5AB89EFC);
        assert(C.words[3] == 0x2DCFE98B);
        assert(C.n == 4);
        /* Different lengths */
        B.n = 2;
        xor(&A, &B, &C);
        assert(C.words[0] == 0xDEF89AB4);
        assert(C.words[1] == 0xA98FEDC3);
        assert(C.words[2] == 0x0FEDCBA9);
        assert(C.words[3] == 0x87654321);
        assert(C.n == 4);
        B.n = 4;
        /* Self assign */
        xor(&bakA, &bakB, &bakA);
        assert(bakA.words[0] == 0xDEF89AB4);
        assert(bakA.words[1] == 0xA98FEDC3);
        assert(bakA.words[2] == 0x5AB89EFC);
        assert(bakA.words[3] == 0x2DCFE98B);
        assert(bakA.n == 4);

    }
    {
        BigInt ref = {{0x43573457, 0x98486223, 0x98236815, 0x99913852}, 4, NULL};
        BigInt less = {{0x43573457, 0x98486223, 0x88236815, 0x99913852}, 4, NULL};
        BigInt greater = {{0x03573457, 0x08486223, 0x9823681A, 0x99913852}, 4, NULL};

        assert(lessThan(&less, &ref));
        assert(!lessThan(&greater, &ref));
        assert(!lessThan(&ref, &ref));

        assert(!equal(&less, &ref));
        assert(!equal(&greater, &ref));
        assert(equal(&ref, &ref));
    }
    {
        BigInt dividend = {{0x12345678, 0x9ABCDEF0, 0xCCCCCCCC, 0xDDDDDDDD}, 4, NULL};
        BigInt divisor = {{0x77777777, 0x88888888, 0x00000000, 0x00000000}, 4, NULL};
        BigInt quotient = {0};
        BigInt remainder = {0};

        divMod(&dividend, &divisor, &quotient, &remainder);

        assert(quotient.words[0] == 0x14000001);
        assert(quotient.words[1] == 0xA0000000);
        assert(quotient.words[2] == 0x00000001);
        assert(quotient.words[3] == 0x00000000);

        assert(remainder.words[0] == 0x4EBCDF01);
        assert(remainder.words[1] == 0x08DF0112);
        assert(remainder.words[2] == 0x00000000);
        assert(remainder.words[3] == 0x00000000);

        memset(&quotient, 0, sizeof quotient);
        memset(&remainder, 0, sizeof remainder);

        /* It should work even if we only want the remainder. */
        divMod(&dividend, &divisor, NULL, &remainder);
        assert(remainder.words[0] == 0x4EBCDF01);
        assert(remainder.words[1] == 0x08DF0112);
        assert(remainder.words[2] == 0x00000000);
        assert(remainder.words[3] == 0x00000000);
    }
    {
        BigInt zero = {{0}, 2, NULL};
        BigInt nonzero = {{0x666, 0}, 2, NULL};
        BigInt quotient = {0}, remainder = {0};

        /* Zero division */
        divMod(&nonzero, &zero, &quotient, &remainder);
        assert(quotient.n == 2);
        assert(quotient.words[0] == 0xFFFFFFFF);
        assert(quotient.words[1] == 0xFFFFFFFF);
        assert(remainder.n == 2);
        assert(remainder.words[0] == 0x00000666);
        assert(remainder.words[1] == 0x00000000);

        /* Dividing zero */
        divMod(&zero, &nonzero, &quotient, &remainder);
        assert(quotient.n == 2);
        assert(quotient.words[0] == 0x00000000);
        assert(quotient.words[1] == 0x00000000);

        assert(remainder.n == 2);
        assert(remainder.words[0] == 0x00000000);
        assert(remainder.words[1] == 0x00000000);
    }
    {
        BigInt A = {{857656800}, 1, NULL};
        BigInt B = {{338888693}, 1, NULL};
        BigInt gcd = {0};
        BigInt bigA = {{0x8cc61a06, 0xbfe2f415, 0x09e0bd38}, 3, NULL}; /* 3057058046095595223711619590 */
        BigInt bigB = {{0x508f2706, 0xc06af417, 0x006fb794}, 3, NULL}; /* 135057703026874676266608390 */
        BigInt zero = {{0, 0}, 2, NULL};

        /* Quick sanity to see the algorithm works.*/
        gcdEuclidean(&A, &B, &gcd);
        assert(gcd.n == 1);
        assert(gcd.words[0] == 2431);

        /* Try something large. */
        gcdEuclidean(&bigA, &bigB, &gcd);
        assert(gcd.n == 3); /* 21169176758730 */
        assert(gcd.words[0] == 0xd542c9ca);
        assert(gcd.words[1] == 0x00001340);
        assert(gcd.words[2] == 0x00000000);

        /* Mess with zero. */
        gcdEuclidean(&bigA, &zero, &gcd);
        assert(gcd.n == 2);
        assert(gcd.words[0] == 0x8cc61a06);
        assert(gcd.words[1] == 0xbfe2f415);

        gcdEuclidean(&zero, &bigA, &gcd);
        assert(gcd.n == 3);
        assert(gcd.words[0] == 0x8cc61a06);
        assert(gcd.words[1] == 0xbfe2f415);
        assert(gcd.words[2] == 0x09e0bd38);

        free(A.dummy);
        free(B.dummy);
        free(gcd.dummy);
        free(bigA.dummy);
        free(bigB.dummy);
        free(zero.dummy);
    }
    {
        /* Quick sanity to see the extended euclidean works.*/
        BigInt A = {{2310}, 1, NULL};
        BigInt B = {{17017}, 1, NULL};
        BigInt X, Y, GCD;

        gcdExtendedEuclidean(&A, &B, &X, &Y, &GCD);
        assert(X.n == 1);
        assert(X.words[0] == (uint32_t)-81);
        assert(Y.n == 1);
        assert(Y.words[0] == 11);
        assert(GCD.n == 1);
        assert(GCD.words[0] == 77);

        free(A.dummy);
        free(B.dummy);
        free(X.dummy);
        free(Y.dummy);
        free(GCD.dummy);
    }
    {
        BigInt A = {{0x36547d80, 0x163a263d }, 2, NULL}; /* 1601634661830000000 */
        BigInt B = {{91091}, 1, NULL};
        BigInt X, Y, GCD;

        gcdExtendedEuclidean(&A, &B, &X, &Y, &GCD);
        assert(X.n == 1);
        assert(X.words[0] == (uint32_t)-197);
        assert(Y.n == 2);
        assert(Y.words[0] == 0x65b0abeb); /* 3463811225922539 */
        assert(Y.words[1] == 0x000c4e51);
        assert(GCD.n == 1);
        assert(GCD.words[0] == 49);

        free(X.dummy);
        free(Y.dummy);
        free(GCD.dummy);
        /* Reverse the arguments to see how it affect the word count: */
        gcdExtendedEuclidean(&B, &A, &X, &Y, &GCD);

        assert(X.n == 2);
        assert(X.words[0] == 0x65b0abeb); /* 3463811225922539 */
        assert(X.words[1] == 0x000c4e51);
        assert(Y.n == 1);
        assert(Y.words[0] == (uint32_t)-197);
        assert(GCD.n == 2);
        assert(GCD.words[0] == 49);
        assert(GCD.words[1] == 0);

        free(A.dummy);
        free(B.dummy);
        free(X.dummy);
        free(Y.dummy);
        free(GCD.dummy);
    }
    {
        BigInt zero = {{0}, 2, NULL};
        BigInt nonzero = {{42}, 1, NULL};
        BigInt X, Y, GCD;

        gcdExtendedEuclidean(&zero, &nonzero, &X, &Y, &GCD);
        /* 0x + 42y = 42 will be solved. X will be zero, Y will be 1. */
        assert(GCD.n == 1);
        assert(GCD.words[0] == 42);

        assert(X.n == 1);
        assert(X.words[0] == 0);

        assert(Y.n == 2);
        assert(Y.words[0] == 1);
        assert(Y.words[1] == 0);

        free(X.dummy);
        free(Y.dummy);
        free(GCD.dummy);
        /* Zero division testcase */
        /* 42x + 0y = 42 will be solvesd. Both X and Y will be 1.*/
        gcdExtendedEuclidean(&nonzero, &zero, &X, &Y, &GCD);
        assert(GCD.n == 2);
        assert(GCD.words[0] == 42);
        assert(GCD.words[1] == 0);

        assert(X.n == 2);
        assert(X.words[0] == 1);
        assert(X.words[1] == 0);

        assert(Y.n == 1);
        assert(Y.words[0] == 1);

        free(zero.dummy);
        free(nonzero.dummy);
        free(X.dummy);
        free(Y.dummy);
        free(GCD.dummy);
    }
    {
        BigInt base = {{3, 0}, 2, NULL};
        BigInt exponent = {{19}, 1, NULL};
        BigInt mod = {{2000000000}, 1, NULL};
        BigInt zero = {{0}, 1, NULL};
        BigInt res;
        int ret;

        /* Normal case. */
        ret = modPow(&base, &exponent, &mod, &res);
        assert(ret == 0);
        assert(res.n == 1);
        assert(res.words[0] == 1162261467); /* 3^19. */
        free(mod.dummy);
        free(res.dummy);

        /* Zero exponent case */
        ret = modPow(&base, &zero, &mod, &res);
        assert(ret == 0);
        assert(res.n == 1);
        assert(res.words[0] == 1); /* 3^0 = 1. */
        free(mod.dummy);
        free(res.dummy);

        /* Zero base case */
        ret = modPow(&zero, &exponent, &mod, &res);
        assert(ret == 0);
        assert(res.n == 1);
        assert(res.words[0] == 0); /* 0^19 = 0. */
        free(mod.dummy);
        free(res.dummy);

        /* Zero modulus case. */
        ret = modPow(&base, &exponent, &zero, &res);
        assert(ret == 0);
        assert(res.n == 1);
        assert(res.words[0] == 1162261467); /* 3^19. */
        free(zero.dummy);
        free(res.dummy);

        free(base.dummy);
        free(exponent.dummy);
        free(mod.dummy);
    }
    {
        BigInt base = {{42, 0}, 2, NULL};
        BigInt exponent = {{4242424242}, 1, NULL};
        BigInt modulus = {{1000000}, 1, NULL};
        BigInt res;
        int ret;

        /* Large exponent case */
        ret = modPow(&base, &exponent, &modulus, &res);
        assert(ret == 0);
        assert(res.n == 1);
        assert(res.words[0] == 880064);

        free(base.dummy);
        free(exponent.dummy);
        free(modulus.dummy);
        free(res.dummy);
    }
    {
        BigInt witness = {{1}, 1, NULL};
        uint32_t primeList[] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 997/*, 5471, 35381, 100019*/};
        uint32_t compositeList[] = {4, 16, 25, 121, 4557/*, 45957, 215441*/};

        BigInt toTest = {{0}, 1, NULL};
        uint32_t i, j;
        uint32_t nPasses;

        /* For prime numbers all witnesses must pass the test. */
        for (j = 1; j < sizeof(primeList) / sizeof(*primeList); j++)
        {
            toTest.words[0] = primeList[j];
            for (i = 1; i < toTest.words[0]; i++)
            {
                if (!(i % 100))
                {
                    printf("Prime: %d %d %% done          \r", primeList[j], i*100 / toTest.words[0]);
                    fflush(stdout);
                }
                witness.words[0] = i;
                if (!mrTest(&toTest, &witness))
                {
                    printf("Testing %d, with withness: %d fails.\n", toTest.words[0], i);
                    assert(!"Test failed!");
                }
            }
        }

        /* For composite numbers at most 25% of the test must pass. */
        for (j = 1; j < sizeof(compositeList) / sizeof(*compositeList); j++)
        {
            toTest.words[0] = compositeList[j];

            nPasses = 0;
            for (i = 1; i < toTest.words[0]; i++)
            {
                if (!(i % 100))
                {
                    printf("Composite: %d %d %% done          \r", compositeList[j], i*100 / toTest.words[0]);
                    fflush(stdout);
                }
                witness.words[0] = i;
                if (mrTest(&toTest, &witness))
                {
                    nPasses++;
                }
            }
            assert(4*nPasses <= toTest.words[0] - 1);
        }
        printf("\n");

        free(witness.dummy);
        free(toTest.dummy);
    }

    printf("ALL is OK! %s %s\n", __DATE__, __TIME__);
    return 0;
}

#endif