#ifdef UNIT_TEST

#include <inttypes.h>
#include <assert.h>
#include <stddef.h>

#define WORD_COUNT 8

typedef struct
{
    uint32_t words[WORD_COUNT];
    size_t n;
} BigInt;

#define BIGINT_TYPE BigInt
#define GETNWORDS(bi) ((bi)->n)
#define SETNWORDS(bi, nWords) ((bi)->n = (nWords))

#define BOUNDS_CHECK

#ifdef BOUNDS_CHECK
    #define ASSERT(x) assert(x)

    uint32_t getWord(const BigInt *bi, size_t i)
    {
        assert(i < WORD_COUNT);
        return bi->words[i];
    }

    #define GETWORD(bi, i) (getWord((bi), (i)))

    void setWord(BigInt *bi, size_t i, uint32_t word)
    {
        assert(i < WORD_COUNT);
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
#define SETNWORDS(bi, words)
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
        BigInt A = {{0x87654321, 0x2468ACE0, 0x369CF258, 0x48C048C0}, 4};
        BigInt B = {{0x88888888, 0xF2222222, 0x33333333, 0xC4444444}, 4};
        BigInt C = {0};

        /* Normal addition */
        carry = addBigint(&A, &B, &C);
        assert(C.words[0] == 0x0FEDCBA9);
        assert(C.words[1] == 0x168ACF03);
        assert(C.words[2] == 0x69D0258C);
        assert(C.words[3] == 0x0D048D04);
        assert(C.n == 4);
        assert(carry == 1);

        B.n = 2;

        /* Test when one of the numbers is shorter. */
        carry = addBigint(&A, &B, &C);
        assert(C.words[0] == 0x0FEDCBA9);
        assert(C.words[1] == 0x168ACF03);
        assert(C.words[2] == 0x369CF259);
        assert(C.words[3] == 0x48C048C0);
        assert(C.n == 4);
        assert(carry == 0);
    }
    {
        BigInt A = {{0xFFFFFFFF, 0xEEEEEEEE, 0xDDDDDDDD, 0xCCCCCCCC}, 4};
        BigInt B = {{0xFFFFFFFF, 0xEEEEEEEE, 0xDDDDDDDD, 0xCCCCCCCC}, 4};
        BigInt C = {0};

        carry = addBigint(&A, &B, &C);
        assert(C.words[0] == 0xFFFFFFFE);
        assert(C.words[1] == 0xDDDDDDDD);
        assert(C.words[2] == 0xBBBBBBBB);
        assert(C.words[3] == 0x99999999);
        assert(C.n == 4);
        assert(carry == 1);

        /* Check if it works, when output matches input, (it should).*/
        carry = addBigint(&A, &B, &A);
        assert(A.words[0] == 0xFFFFFFFE);
        assert(A.words[1] == 0xDDDDDDDD);
        assert(A.words[2] == 0xBBBBBBBB);
        assert(A.words[3] == 0x99999999);
        assert(C.n == 4);
        assert(carry == 1);
    }
    {
        BigInt A = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 4};
        BigInt B = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 4};
        BigInt C = {0};

        mulBigint(&A, &B, &C);
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
        BigInt A = {{0x12345678, 0x12345678, 0x12345678, 0x12345678}, 4};
        BigInt B = {{0x87654321, 0x87654321, 0x87654321, 0x87654321}, 4};
        BigInt C = {0};

        mulBigint(&A, &B, &C);
        assert(C.words[0] == 0x70b88d78);
        assert(C.words[1] == 0xeb11e7f5);
        assert(C.words[2] == 0x656b4272);
        assert(C.words[3] == 0xdfc49cf0);
        assert(C.words[4] == 0x78acdc7d);
        assert(C.words[5] == 0xfe538200);
        assert(C.words[6] == 0x83fa2782);
        assert(C.words[7] == 0x09a0cd05);
        assert(C.n == 8);
    }
    {
        BigInt A = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 4};
        BigInt B = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 4};
        BigInt C = {0};
        int borrow;

        borrow = subBigint(&A, &B, &C);

        assert(C.words[0] == 0xFFFFFFFF);
        assert(C.words[1] == 0xFFFFFFFF);
        assert(C.words[2] == 0xFFFFFFFF);
        assert(C.words[3] == 0xFFFFFFFF);
        assert(C.n == 4);
        assert(borrow == 1);
    }
    {
        BigInt A = {{0x12345678, 0x12345678, 0x12345678, 0x12345678}, 4};
        BigInt B = {{0x87654321, 0x87654321, 0x87654321, 0x87654321}, 4};
        int borrow;

        borrow = subBigint(&A, &B, &A);

        assert(A.words[0] == 0x8ACF1357);
        assert(A.words[1] == 0x8ACF1356);
        assert(A.words[2] == 0x8ACF1356);
        assert(A.words[3] == 0x8ACF1356);
        assert(A.n == 4);
        assert(borrow == 1);
    }
    {
        BigInt A = {{0x12345678, 0x22222222, 0x33333333, 0x44444444}, 4};
        BigInt B = {0};

        shlBigint(&A, &B, 1);
        assert(B.words[0] == 0x2468ACF0);
        assert(B.words[1] == 0x44444444);
        assert(B.words[2] == 0x66666666);
        assert(B.words[3] == 0x88888888);
        assert(B.n == 4);

        shlBigint(&A, &B, 2);
        assert(B.words[0] == 0x48D159E0);
        assert(B.words[1] == 0x88888888);
        assert(B.words[2] == 0xCCCCCCCC);
        assert(B.words[3] == 0x11111110);
        assert(B.n == 4);

        shlBigint(&A, &B, 3);
        assert(B.words[0] == 0x91A2B3C0);
        assert(B.words[1] == 0x11111110);
        assert(B.words[2] == 0x99999999);
        assert(B.words[3] == 0x22222221);
        assert(B.n == 4);

        shlBigint(&A, &B, 4);
        assert(B.words[0] == 0x23456780);
        assert(B.words[1] == 0x22222221);
        assert(B.words[2] == 0x33333332);
        assert(B.words[3] == 0x44444443);
        assert(B.n == 4);

        shlBigint(&A, &B, 16);
        assert(B.words[0] == 0x56780000);
        assert(B.words[1] == 0x22221234);
        assert(B.words[2] == 0x33332222);
        assert(B.words[3] == 0x44443333);
        assert(B.n == 4);

        shlBigint(&A, &B, 32);
        assert(B.words[0] == 0x00000000);
        assert(B.words[1] == 0x12345678);
        assert(B.words[2] == 0x22222222);
        assert(B.words[3] == 0x33333333);
        assert(B.n == 4);

        shlBigint(&A, &B, 48);
        assert(B.words[0] == 0x00000000);
        assert(B.words[1] == 0x56780000);
        assert(B.words[2] == 0x22221234);
        assert(B.words[3] == 0x33332222);
        assert(B.n == 4);

        /* Test self shift */
        shlBigint(&A, &A, 16);
        assert(A.words[0] == 0x56780000);
        assert(A.words[1] == 0x22221234);
        assert(A.words[2] == 0x33332222);
        assert(A.words[3] == 0x44443333);
        assert(A.n == 4);
    }
    {
        BigInt A = {{0x12345678, 0x22222222, 0x33333333, 0x44444444}, 4};
        BigInt B = {0};

        shrBigint(&A, &B, 1);
        assert(B.words[0] == 0x091A2B3C);
        assert(B.words[1] == 0x91111111);
        assert(B.words[2] == 0x19999999);
        assert(B.words[3] == 0x22222222);
        assert(B.n == 4);

        shrBigint(&A, &B, 2);
        assert(B.words[0] == 0x848D159E);
        assert(B.words[1] == 0xC8888888);
        assert(B.words[2] == 0x0CCCCCCC);
        assert(B.words[3] == 0x11111111);
        assert(B.n == 4);

        shrBigint(&A, &B, 3);
        assert(B.words[0] == 0x42468ACF);
        assert(B.words[1] == 0x64444444);
        assert(B.words[2] == 0x86666666);
        assert(B.words[3] == 0x08888888);
        assert(B.n == 4);

        shrBigint(&A, &B, 4);
        assert(B.words[0] == 0x21234567);
        assert(B.words[1] == 0x32222222);
        assert(B.words[2] == 0x43333333);
        assert(B.words[3] == 0x04444444);
        assert(B.n == 4);

        shrBigint(&A, &B, 16);
        assert(B.words[0] == 0x22221234);
        assert(B.words[1] == 0x33332222);
        assert(B.words[2] == 0x44443333);
        assert(B.words[3] == 0x00004444);
        assert(B.n == 4);

        shrBigint(&A, &B, 32);
        assert(B.words[0] == 0x22222222);
        assert(B.words[1] == 0x33333333);
        assert(B.words[2] == 0x44444444);
        assert(B.words[3] == 0x00000000);
        assert(B.n == 4);

        shrBigint(&A, &B, 48);
        assert(B.words[0] == 0x33332222);
        assert(B.words[1] == 0x44443333);
        assert(B.words[2] == 0x00004444);
        assert(B.words[3] == 0x00000000);
        assert(B.n == 4);

        /* Test self shift */
        shrBigint(&A, &A, 16);
        assert(A.words[0] == 0x22221234);
        assert(A.words[1] == 0x33332222);
        assert(A.words[2] == 0x44443333);
        assert(A.words[3] == 0x00004444);
        assert(A.n == 4);
    }
    {
        BigInt A = {{0x12345678, 0x9ABCDEF0, 0x0FEDCBA9, 0x87654321}, 4};
        BigInt B = {{0xCCCCCCCC, 0x33333333, 0x55555555, 0xAAAAAAAA}, 4};
        BigInt C = {0};
        BigInt bakA, bakB;

        /* AND */
        bakA = A;
        bakB = B;
        /* Normal */
        andBigint(&A, &B, &C);
        assert(C.words[0] == 0x00044448);
        assert(C.words[1] == 0x12301230);
        assert(C.words[2] == 0x05454101);
        assert(C.words[3] == 0x82200220);
        assert(C.n == 4);
        /* Different lengths */
        B.n = 2;
        andBigint(&A, &B, &C);
        assert(C.words[0] == 0x00044448);
        assert(C.words[1] == 0x12301230);
        assert(C.words[2] == 0x00000000);
        assert(C.words[3] == 0x00000000);
        assert(C.n == 4);
        B.n = 4;
        /* Self assign */
        andBigint(&bakA, &bakB, &bakA);
        assert(bakA.words[0] == 0x00044448);
        assert(bakA.words[1] == 0x12301230);
        assert(bakA.words[2] == 0x05454101);
        assert(bakA.words[3] == 0x82200220);
        assert(bakA.n == 4);

        /* OR */
        bakA = A;
        bakB = B;
        /* Normal */
        orBigint(&A, &B, &C);
        assert(C.words[0] == 0xDEFCDEFC);
        assert(C.words[1] == 0xBBBFFFF3);
        assert(C.words[2] == 0x5FFDDFFD);
        assert(C.words[3] == 0xAFEFEBAB);
        assert(C.n == 4);
        /* Different lengths */
        B.n = 2;
        orBigint(&A, &B, &C);
        assert(C.words[0] == 0xDEFCDEFC);
        assert(C.words[1] == 0xBBBFFFF3);
        assert(C.words[2] == 0x0FEDCBA9);
        assert(C.words[3] == 0x87654321);
        assert(C.n == 4);
        B.n = 4;
        /* Self assign */
        orBigint(&bakA, &bakB, &bakA);
        assert(bakA.words[0] == 0xDEFCDEFC);
        assert(bakA.words[1] == 0xBBBFFFF3);
        assert(bakA.words[2] == 0x5FFDDFFD);
        assert(bakA.words[3] == 0xAFEFEBAB);
        assert(bakA.n == 4);

        /* XOR */
        bakA = A;
        bakB = B;
        /* Normal */
        xorBigint(&A, &B, &C);
        assert(C.words[0] == 0xDEF89AB4);
        assert(C.words[1] == 0xA98FEDC3);
        assert(C.words[2] == 0x5AB89EFC);
        assert(C.words[3] == 0x2DCFE98B);
        assert(C.n == 4);
        /* Different lengths */
        B.n = 2;
        xorBigint(&A, &B, &C);
        assert(C.words[0] == 0xDEF89AB4);
        assert(C.words[1] == 0xA98FEDC3);
        assert(C.words[2] == 0x0FEDCBA9);
        assert(C.words[3] == 0x87654321);
        assert(C.n == 4);
        B.n = 4;
        /* Self assign */
        xorBigint(&bakA, &bakB, &bakA);
        assert(bakA.words[0] == 0xDEF89AB4);
        assert(bakA.words[1] == 0xA98FEDC3);
        assert(bakA.words[2] == 0x5AB89EFC);
        assert(bakA.words[3] == 0x2DCFE98B);
        assert(bakA.n == 4);

    }
    {
        BigInt ref = {{0x43573457, 0x98486223, 0x98236815, 0x99913852}, 4};
        BigInt less = {{0x43573457, 0x98486223, 0x88236815, 0x99913852}, 4};
        BigInt greater = {{0x43573458, 0x98486223, 0x98236815, 0x99913852}, 4};

        assert(lessThanBigint(&less, &ref));
        assert(!lessThanBigint(&greater, &ref));
        assert(!lessThanBigint(&ref, &ref));

        assert(!equalBigint(&less, &ref));
        assert(!equalBigint(&greater, &ref));
        assert(equalBigint(&ref, &ref));
    }
    {
        BigInt dividend = {{0x12345678, 0x9ABCDEF0, 0xCCCCCCCC, 0xDDDDDDDD}, 4};
        BigInt divisor = {{0x77777777, 0x88888888, 0x00000000, 0x00000000}, 4};
        BigInt quotient;
        BigInt remainder;

        divModBigint(&dividend, &divisor, &quotient, &remainder);

        assert(quotient.words[0] == 0x14000001);
        assert(quotient.words[1] == 0xA0000000);
        assert(quotient.words[2] == 0x00000001);
        assert(quotient.words[3] == 0x00000000);

        assert(remainder.words[0] == 0x4EBCDF01);
        assert(remainder.words[1] == 0x08DF0112);
        assert(remainder.words[2] == 0x00000000);
        assert(remainder.words[3] == 0x00000000);
    }

    printf("ALL is OK! %s %s\n", __DATE__, __TIME__);
}

#endif