#ifdef UNIT_TEST

#include <inttypes.h>
#include <assert.h>

#define WORD_TYPE uint32_t
#define HALF_WORD_BITS 16
#define DECLARE_STUFF
#define DEFINE_STUFF
#include "bigint.h"

#include <stdio.h>

int main()
{
    uint32_t high, low;
    uint32_t result;
    int carry;
    uint32_t C[4];

    carry = addDigit(0xFFFFFFFF, 0xFFFFFFFF, &result);
    assert(carry == 1);
    assert(result == 0xFFFFFFFE);

    mulDigit(0xFFFFFFFF, 0xFFFFFFFF, &high, &low);
    assert(high == 0xFFFFFFFE);
    assert(low == 1);

    {
        uint32_t A[4] = {0x87654321, 0x2468ACE0, 0x369CF258, 0x48C048C0};
        uint32_t B[4] = {0x88888888, 0xF2222222, 0x33333333, 0x44444444};

        carry = addBigint(A, B, C, 4);
        assert(C[0] == 0x0FEDCBA9);
        assert(C[1] == 0x168ACF03);
        assert(C[2] == 0x69D0258C);
        assert(C[3] == 0x8D048D04);
        assert(carry == 0);
    }
    {
        uint32_t A[4] = {0xFFFFFFFF, 0xEEEEEEEE, 0xDDDDDDDD, 0xCCCCCCCC};
        uint32_t B[4] = {0xFFFFFFFF, 0xEEEEEEEE, 0xDDDDDDDD, 0xCCCCCCCC};

        carry = addBigint(A, B, C, 4);
        assert(C[0] == 0xFFFFFFFE);
        assert(C[1] == 0xDDDDDDDD);
        assert(C[2] == 0xBBBBBBBB);
        assert(C[3] == 0x99999999);
        assert(carry == 1);

        carry = addBigintEx(A, 4, B, 1, C);
        assert(C[0] == 0xFFFFFFFE);
        assert(C[1] == 0xEEEEEEEF);
        assert(C[2] == 0xDDDDDDDD);
        assert(C[3] == 0xCCCCCCCC);
        assert(carry == 0);

        /* Check if it works, when output matches input, (it should).*/
        carry = addBigint(A, B, A, 4);
        assert(A[0] == 0xFFFFFFFE);
        assert(A[1] == 0xDDDDDDDD);
        assert(A[2] == 0xBBBBBBBB);
        assert(A[3] == 0x99999999);
        assert(carry == 1);
    }
    {
        uint32_t A[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        uint32_t B[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        uint32_t C[8];

        mulBigint(A, B, C, 4);
        assert(C[0] == 0x00000001);
        assert(C[1] == 0x00000000);
        assert(C[2] == 0x00000000);
        assert(C[3] == 0x00000000);
        assert(C[4] == 0xFFFFFFFE);
        assert(C[5] == 0xFFFFFFFF);
        assert(C[6] == 0xFFFFFFFF);
        assert(C[7] == 0xFFFFFFFF);
    }

    {
        uint32_t A[4] = {0x12345678, 0x12345678, 0x12345678, 0x12345678};
        uint32_t B[4] = {0x87654321, 0x87654321, 0x87654321, 0x87654321};
        uint32_t C[8];

        mulBigint(A, B, C, 4);
        assert(C[0] == 0x70b88d78);
        assert(C[1] == 0xeb11e7f5);
        assert(C[2] == 0x656b4272);
        assert(C[3] == 0xdfc49cf0);
        assert(C[4] == 0x78acdc7d);
        assert(C[5] == 0xfe538200);
        assert(C[6] == 0x83fa2782);
        assert(C[7] == 0x09a0cd05);
    }
    {
        uint32_t A[4] = {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        uint32_t B[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        uint32_t C[4];
        int borrow;

        borrow = subBigint(A, B, C, 4);

        assert(C[0] == 0xFFFFFFFF);
        assert(C[1] == 0xFFFFFFFF);
        assert(C[2] == 0xFFFFFFFF);
        assert(C[3] == 0xFFFFFFFF);
        assert(borrow == 1);
    }
    {
        uint32_t A[4] = {0x12345678, 0x12345678, 0x12345678, 0x12345678};
        uint32_t B[4] = {0x87654321, 0x87654321, 0x87654321, 0x87654321};
        int borrow;

        borrow = subBigint(A, B, A, 4);

        assert(A[0] == 0x8ACF1357);
        assert(A[1] == 0x8ACF1356);
        assert(A[2] == 0x8ACF1356);
        assert(A[3] == 0x8ACF1356);
        assert(borrow == 1);
    }


    printf("ALL is OK!\n");
}

#endif