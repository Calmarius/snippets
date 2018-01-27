#ifdef UNIT_TEST

#include <stdint.h>
#include <assert.h>

#define WORD_TYPE uint32_t
#define HALF_WORD_BITS 16
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
    }

    printf("ALL is OK!\n");
}

#endif