#include "templateheader.h"

#include <stddef.h>

#ifndef WORD_TYPE
    #error Please define WORD_TYPE to the largest unsigned type your CPU natively supports.
#endif

#ifndef HALF_WORD_BITS
    #error Please define HALF_WORD_BASE to be the half of the bits of the word.  (Eg. for a 32 bit unsigned integer it should be 16).
#endif

#define HALF_WORD_BASE (1 << HALF_WORD_BITS)

#define HALF_WORD_MASK (HALF_WORD_BASE - 1)

#ifdef DECLARE_STUFF
/**
 * Adds two words together, returns with the carry.
 */
static int FN(addDigit)(WORD_TYPE a, WORD_TYPE b, WORD_TYPE *result);

/**
 * Multiplies two words together, returns the result in two parts.
 */
static int FN(mulDigit)(WORD_TYPE a, WORD_TYPE b, WORD_TYPE *resultHigh, WORD_TYPE *resultLow);

/**
 * Adds two big integers together.
 *
 * The numbers and the result is an array of words in little endian order.
 * 'n' is the number of words in each array.
 */
static int FN(addBigint)(WORD_TYPE *aWords, WORD_TYPE *bWords, WORD_TYPE *result, size_t n)



#endif

#ifdef DEFINE_STUFF

static int FN(addDigit)(WORD_TYPE a, WORD_TYPE b, WORD_TYPE *result)
{
    *result = a + b;
    return *result < a;
}


static void FN(mulDigit)(WORD_TYPE a, WORD_TYPE b, WORD_TYPE *resultHigh, WORD_TYPE *resultLow)
{
    /*
        Split the numbers to two halves a_1 and a_0, to be able to do the multiplication.
        To get the result do this:
        Math: (a_1 H + a_0)(b_1 H + b_0) = a_1 b_1 H^2 + (a_0 b_1 + a_1 b_0) H + a_0 b_0
    */
    WORD_TYPE a_0 = a & HALF_WORD_MASK;
    WORD_TYPE a_1 = a >> HALF_WORD_BITS;
    WORD_TYPE b_0 = b & HALF_WORD_MASK;
    WORD_TYPE b_1 = b >> HALF_WORD_BITS;
    WORD_TYPE overlapping1 = a_0*b_1;
    WORD_TYPE overlapping2 = a_1*b_0;

    /* First the easy part. The initial values of the two halves.*/
    *resultHigh = a_1*b_1;
    *resultLow = a_0*b_0;

    /* Now apply the intermediates:*/

    *resultHigh += FN(addDigit)(*resultLow, overlapping1 << HALF_WORD_BITS, resultLow);
    *resultHigh += FN(addDigit)(*resultLow, overlapping2 << HALF_WORD_BITS, resultLow);
    FN(addDigit)(*resultHigh, overlapping1 >> HALF_WORD_BITS, resultHigh);
    FN(addDigit)(*resultHigh, overlapping2 >> HALF_WORD_BITS, resultHigh);
}


static int FN(addBigint)(WORD_TYPE *aWords, WORD_TYPE *bWords, WORD_TYPE *result, size_t n)
{
    size_t i;
    int carry = 0;
    WORD_TYPE r;

    for (i = 0; i < n; i++)
    {
        result[i] = carry;
        carry = FN(addDigit)(aWords[i], bWords[i], &r);
        result[i] += r;
    }
    
    return carry;
}

#endif


#include "templatefooter.h"
#undef WORD_TYPE
#undef HALF_WORD_BITS
#undef HALF_WORD_BASE
#undef HALF_WORD_MASK
#undef DECLARE_STUFF
#undef DEFINE_STUFF

