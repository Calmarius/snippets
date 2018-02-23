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
#define GETNWORDS(bi) ((bi).n)
#define SETNWORDS(bi, nWords) ((bi).n = (nWords))

#define BOUNDS_CHECK

#ifdef BOUNDS_CHECK
    #define ASSERT(x) assert(x)

    uint32_t getWord(const BigInt *bi, size_t i)
    {
        assert(i < WORD_COUNT);
        return bi->words[i];
    }

    #define GETWORD(bi, i) (getWord(&(bi), (i)))

    void setWord(BigInt *bi, size_t i, uint32_t word)
    {
        assert(i < WORD_COUNT);
        bi->words[i] = word;
    }

    #define SETWORD(bi, i, word) (setWord(&(bi), (i), (word)))

#else
    #define ASSERT
    #define GETWORD(bi, i) ((bi).words[i])
    #define SETWORD(bi, i, word) ((bi).words[i] = (word))
#endif

#define WORD_TYPE uint32_t
#define WORD_BITS 32
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
        BigInt C;

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
        BigInt C;

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
        BigInt C;

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
        BigInt C;

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
        BigInt C;
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
        uint32_t A[4] = {0x12345678, 0x22222222, 0x33333333, 0x44444444};
        uint32_t B[4];

        shlBigint(A, B, 4, 1);
        assert(B[0] == 0x2468ACF0);
        assert(B[1] == 0x44444444);
        assert(B[2] == 0x66666666);
        assert(B[3] == 0x88888888);

        shlBigint(A, B, 4, 2);
        assert(B[0] == 0x48D159E0);
        assert(B[1] == 0x88888888);
        assert(B[2] == 0xCCCCCCCC);
        assert(B[3] == 0x11111110);

        shlBigint(A, B, 4, 3);
        assert(B[0] == 0x91A2B3C0);
        assert(B[1] == 0x11111110);
        assert(B[2] == 0x99999999);
        assert(B[3] == 0x22222221);

        shlBigint(A, B, 4, 4);
        assert(B[0] == 0x23456780);
        assert(B[1] == 0x22222221);
        assert(B[2] == 0x33333332);
        assert(B[3] == 0x44444443);

        shlBigint(A, B, 4, 16);
        assert(B[0] == 0x56780000);
        assert(B[1] == 0x22221234);
        assert(B[2] == 0x33332222);
        assert(B[3] == 0x44443333);

        shlBigint(A, B, 4, 32);
        assert(B[0] == 0x00000000);
        assert(B[1] == 0x12345678);
        assert(B[2] == 0x22222222);
        assert(B[3] == 0x33333333);

        shlBigint(A, B, 4, 48);
        assert(B[0] == 0x00000000);
        assert(B[1] == 0x56780000);
        assert(B[2] == 0x22221234);
        assert(B[3] == 0x33332222);

        /* Test self shift */
        shlBigint(A, A, 4, 16);
        assert(A[0] == 0x56780000);
        assert(A[1] == 0x22221234);
        assert(A[2] == 0x33332222);
        assert(A[3] == 0x44443333);
    }
    {
        uint32_t A[4] = {0x12345678, 0x22222222, 0x33333333, 0x44444444};
        uint32_t B[4];

        shrBigint(A, B, 4, 1);
        assert(B[0] == 0x091A2B3C);
        assert(B[1] == 0x91111111);
        assert(B[2] == 0x19999999);
        assert(B[3] == 0x22222222);

        shrBigint(A, B, 4, 2);
        assert(B[0] == 0x848D159E);
        assert(B[1] == 0xC8888888);
        assert(B[2] == 0x0CCCCCCC);
        assert(B[3] == 0x11111111);

        shrBigint(A, B, 4, 3);
        assert(B[0] == 0x42468ACF);
        assert(B[1] == 0x64444444);
        assert(B[2] == 0x86666666);
        assert(B[3] == 0x08888888);

        shrBigint(A, B, 4, 4);
        assert(B[0] == 0x21234567);
        assert(B[1] == 0x32222222);
        assert(B[2] == 0x43333333);
        assert(B[3] == 0x04444444);

        shrBigint(A, B, 4, 16);
        assert(B[0] == 0x22221234);
        assert(B[1] == 0x33332222);
        assert(B[2] == 0x44443333);
        assert(B[3] == 0x00004444);

        shrBigint(A, B, 4, 32);
        assert(B[0] == 0x22222222);
        assert(B[1] == 0x33333333);
        assert(B[2] == 0x44444444);
        assert(B[3] == 0x00000000);

        shrBigint(A, B, 4, 48);
        assert(B[0] == 0x33332222);
        assert(B[1] == 0x44443333);
        assert(B[2] == 0x00004444);
        assert(B[3] == 0x00000000);

        /* Test self shift */
        shrBigint(A, A, 4, 16);
        assert(A[0] == 0x22221234);
        assert(A[1] == 0x33332222);
        assert(A[2] == 0x44443333);
        assert(A[3] == 0x00004444);

    }
    {
        uint32_t A[4] = {0x12345678, 0x9ABCDEF0, 0x0FEDCBA9, 0x87654321};
        uint32_t B[4] = {0xCCCCCCCC, 0x33333333, 0x55555555, 0xAAAAAAAA};
        uint32_t C[4];

        andBigint(A, B, C, 4);
        assert(C[0] == 0x00044448);
        assert(C[1] == 0x12301230);
        assert(C[2] == 0x05454101);
        assert(C[3] == 0x82200220);

        orBigint(A, B, C, 4);
        assert(C[0] == 0xDEFCDEFC);
        assert(C[1] == 0xBBBFFFF3);
        assert(C[2] == 0x5FFDDFFD);
        assert(C[3] == 0xAFEFEBAB);

        xorBigint(A, B, C, 4);
        assert(C[0] == 0xDEF89AB4);
        assert(C[1] == 0xA98FEDC3);
        assert(C[2] == 0x5AB89EFC);
        assert(C[3] == 0x2DCFE98B);
    }
    {
        uint32_t ref[4] = {0x43573457, 0x98486223, 0x98236815, 0x99913852};
        uint32_t less[4] = {0x43573457, 0x98486223, 0x88236815, 0x99913852};
        uint32_t greater[4] = {0x43573458, 0x98486223, 0x98236815, 0x99913852};

        assert(lessThanBigint(less, ref, 4));
        assert(!lessThanBigint(greater, ref, 4));
        assert(!lessThanBigint(ref, ref, 4));

        assert(!equalBigint(less, ref, 4));
        assert(!equalBigint(greater, ref, 4));
        assert(equalBigint(ref, ref, 4));
    }
    #if 0
    {
        uint32_t dividend[4] = {0x12345678, 0x9ABCDEF0, 0xCCCCCCCC, 0xDDDDDDDD};
        uint32_t divisor[4] = {0x77777777, 0x88888888, 0x00000000, 0x00000000};
        uint32_t quotient[4];
        uint32_t remainder[4];

        divModBigint(dividend, divisor, quotient, remainder, 4);

        assert(quotient[0] == 0x14000001);
        assert(quotient[1] == 0xA0000000);
        assert(quotient[2] == 0x00000001);
        assert(quotient[3] == 0x00000000);

        assert(remainder[0] == 0x4EBCDF01);
        assert(remainder[1] == 0x08DF0112);
        assert(remainder[2] == 0x00000000);
        assert(remainder[3] == 0x00000000);
    }
    #endif

    printf("ALL is OK! %s %s\n", __DATE__, __TIME__);
}

#endif