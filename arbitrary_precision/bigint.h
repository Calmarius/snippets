#include "meta/templateheader.h"

#include <stddef.h>

#ifndef WORD_TYPE
    #error Please define WORD_TYPE to the largest unsigned type your CPU natively supports.
#endif

#ifndef WORD_BITS
    #error Please define WORD_BITS to be the bits of the word.  (Eg. for a 32 bit unsigned integer it should be 32).
#endif

#define HALF_WORD_BITS (WORD_BITS / 2)

#define HALF_WORD_BASE (1 << HALF_WORD_BITS)

#define HALF_WORD_MASK (HALF_WORD_BASE - 1)

#ifdef DECLARE_STUFF
/**
 * Adds two words together.
 *
 * a, b (in): The two digits to add.
 * result (out): The sum.
 *
 * Returns the carry.
 */
SPECIFIER int FN(addDigit)(WORD_TYPE a, WORD_TYPE b, WORD_TYPE *result);

/**
 * Subtracts two words.
 *
 * a, b (in): The two digits to subtract.
 * result (out): The difference.
 *
 * Returns the borrow.
 */
SPECIFIER int FN(subDigit)(WORD_TYPE a, WORD_TYPE b, WORD_TYPE *result);


/**
 * Multiplies two words together, returns the result in two parts.
 *
 * a, b (in): The two words to multiply.
 * resultHigh, resultLow: The result in two parts.
 */
SPECIFIER void FN(mulDigit)(WORD_TYPE a, WORD_TYPE b, WORD_TYPE *resultHigh, WORD_TYPE *resultLow);


/**
 * Adds two big integers together.
 *
 * aWords (in): The words in the first number in little endian order.
 * bWords (in): The words in the second number in little endian order.
 * result (out): The result words in little endian order.
 * n (in): The number of words in each of these numbers.
 *
 * Returns the carry.
 *
 * The output can be one of the inputs.
 */
SPECIFIER int FN(addBigint)(const WORD_TYPE *aWords, const WORD_TYPE *bWords, WORD_TYPE *result, size_t n);


/**
 * Subtracts two big integers.
 *
 * aWords, bWords (in): The two arrays of words little endian order representing the operands.
 * result (out): The array of words containing the result.
 * n (in): The number of words in each of the arrays.
 *
 * Returns the borrow.
 *
 * The output can be one of the inputs.
 */
SPECIFIER int FN(subBigint)(const WORD_TYPE *aWords, const WORD_TYPE *bWords, WORD_TYPE *result, size_t n);

/**
 * Adds big integers whose lengths differ.
 *
 * aWords (in): The first number. Words in little endian order.
 * nA (in): Number of words in the first number.
 * bWords (in): The second number. Words in little endian order.
 * nB (in): Number of words in the second number.
 * result (out): The result. It should contain at least as many words as the larger number.
 *
 * The output can be the longer input.
 */
SPECIFIER int FN(addBigintEx)(
    const WORD_TYPE *aWords,
    size_t nA,
    const WORD_TYPE *bWords,
    size_t nB,
    WORD_TYPE *result);

/**
 * Multiplies two big integers.
 *
 * aWords (in): The first number. Words in little endian order.
 * bWords (in): The second number. Words in little endian order.
 * result (out): The result of the multiplication. Words in little endian order.
 *      This array must be able to contain twice as many words as the two arguemnts.
 * n (in): The number of words in each array.
 */
SPECIFIER void FN(mulBigint)(const WORD_TYPE *aWords, const WORD_TYPE *bWords, WORD_TYPE *result, size_t n);


/**
 * Shifts big integer left.
 *
 * in (in): The input big integer.
 * out (out): The output big integer.
 * n (in): The number of words in the argument.
 * shiftAmount (in): The amount to shift.
 *
 * Words in little endian order.
 * The output can be the same as input.
 */
SPECIFIER void FN(shlBigint)(const WORD_TYPE *in, WORD_TYPE *out, size_t n, unsigned shiftAmount);


/**
 * Shifts big integer right. It's not sign preserving.
 *
 * in (in): The input big integer.
 * out (out): The output big integer.
 * n (in): The number of words in the argument.
 * shiftAmount (in): The amount to shift.
 *
 * Words in little endian order.
 * The output can be the same as input.
 */
SPECIFIER void FN(shrBigint)(const WORD_TYPE *in, WORD_TYPE *out, size_t n, unsigned shiftAmount);


/**
 * Bitwise AND of two big integers.
 *
 * in1, in2 (in): The two arguments.
 * out (out): The result.
 * n (in): The number of word in each arguments.
 *
 * Words in little endian order.
 */
SPECIFIER void FN(andBigint)(const WORD_TYPE *in1, const WORD_TYPE *in2, WORD_TYPE *out, size_t n);


/**
 * Bitwise OR of two big integers.
 *
 * in1, in2 (in): The two arguments.
 * out (out): The result.
 * n (in): The number of word in each arguments.
 *
 *
 * Words in little endian order.
 */
SPECIFIER void FN(orBigint)(const WORD_TYPE *in1, const WORD_TYPE *in2, WORD_TYPE *out, size_t n);


/**
 * Bitwise XOR of two big integers.
 *
 * in1, in2 (in): The two arguments.
 * out (out): The result.
 * n (in): The number of words in each argument.
 *
 * Words in little endian order.
 */
SPECIFIER void FN(xorBigint)(const WORD_TYPE *in1, const WORD_TYPE *in2, WORD_TYPE *out, size_t n);


/**
 * Compares two bigints.
 *
 * a, b (in): The two numbers to compare.
 * n (in): The number of words in each argument.
 *
 * Returns non-zero if 'a' is less than 'b'.
 *
 * Words in little endian order.
 */
SPECIFIER int FN(lessThanBigint)(const WORD_TYPE *a, const WORD_TYPE *b, size_t n);


/**
 * Compares two bigints.
 *
 * a, b (in): The two numbers to compare.
 * n (in): The number of words in each argument.
 *
 * Returns non-zero if 'a' equals 'b'.
 *
 * Words in little endian order.
 */
SPECIFIER int FN(equalBigint)(const WORD_TYPE *a, const WORD_TYPE *b, size_t n);


/**
 * Bigint long division algorithm.
 *
 * dividend, divisor (in): as their name suggests...
 * quotient, remainder (out): as their name suggests...
 *
 * Words in little endian order.
 *
 * Zero division is indicated by remainder == dividend and quotient == all 1 bits.
 */
SPECIFIER void FN(divModBigint)(
    const WORD_TYPE *dividend,
    const WORD_TYPE *divisor,
    WORD_TYPE *quotient,
    WORD_TYPE *remainder,
    size_t n
);

#endif

#ifdef DEFINE_STUFF

SPECIFIER int FN(addDigit)(WORD_TYPE a, WORD_TYPE b, WORD_TYPE *result)
{
    *result = a + b;
    return *result < a;
}


SPECIFIER int FN(subDigit)(WORD_TYPE a, WORD_TYPE b, WORD_TYPE *result)
{
    *result = a - b;
    return *result > a;
}


SPECIFIER void FN(mulDigit)(WORD_TYPE a, WORD_TYPE b, WORD_TYPE *resultHigh, WORD_TYPE *resultLow)
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


SPECIFIER int FN(addBigint)(const WORD_TYPE *aWords, const WORD_TYPE *bWords, WORD_TYPE *result, size_t n)
{
    size_t i;
    int carry = 0;
    WORD_TYPE r;
    WORD_TYPE s;

    for (i = 0; i < n; i++)
    {
        s = carry;
        carry = FN(addDigit)(aWords[i], bWords[i], &r);
        result[i] = s + r;
        if (result[i] < s) carry = 1;
    }

    return carry;
}


SPECIFIER int FN(subBigint)(const WORD_TYPE *aWords, const WORD_TYPE *bWords, WORD_TYPE *result, size_t n)
{
    size_t i;
    int borrow = 0;
    WORD_TYPE r;
    WORD_TYPE s;

    for (i = 0; i < n; i++)
    {
        s = borrow;
        borrow = FN(subDigit)(aWords[i], bWords[i], &r);
        result[i] = r - s;
        if (result[i] > r) borrow = 1;
    }

    return borrow;
}


SPECIFIER int FN(addBigintEx)(
    const WORD_TYPE *aWords,
    size_t nA,
    const WORD_TYPE *bWords,
    size_t nB,
    WORD_TYPE *result)
{
    size_t i;
    int carry = 0;
    size_t n = nA > nB ? nA : nB;
    WORD_TYPE r;
    WORD_TYPE s;
    WORD_TYPE a;
    WORD_TYPE b;

    for (i = 0; i < n; i++)
    {
        s = carry;
        a = i >= nA ? 0 : aWords[i];
        b = i >= nB ? 0 : bWords[i];
        carry = FN(addDigit)(a, b, &r);
        result[i] = s + r;
        if (result[i] < s) carry = 1;
    }

    return carry;
}

SPECIFIER void FN(mulBigint)(const WORD_TYPE *aWords, const WORD_TYPE *bWords, WORD_TYPE *result, size_t n)
{
    size_t i, j, k;
    WORD_TYPE tmp[2];

    for (k = 0; k < 2*n; k++)
    {
        result[k] = 0;
    }

    /* long multiplication algorithm. */
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            size_t k = i + j;

            FN(mulDigit)(aWords[j], bWords[i], &tmp[1], &tmp[0]);
            FN(addBigintEx)(result + k, 2*n - k, tmp, 2, result + k);
        }
    }
}


SPECIFIER void FN(shlBigint)(const WORD_TYPE *in, WORD_TYPE *out, size_t n, unsigned shiftAmount)
{
    size_t dIndex = shiftAmount / WORD_BITS;
    size_t dShift = shiftAmount % WORD_BITS;
    size_t i = n;

    if (dShift)
    {
        while (i --> 0)
        {
            WORD_TYPE newWord = 0;
            WORD_TYPE part;

            part = i >= dIndex ? in[i - dIndex] : 0;
            newWord |= part << dShift;

            part = i >= dIndex + 1 ? in[i - dIndex - 1] : 0;
            newWord |= part >> (WORD_BITS - dShift);

            out[i] = newWord;
        }
    }
    else
    {
        /* Zero shift case can be simplified. Also we cam avoid out of range shifts. */
        while (i --> 0)
        {
            out[i] = i >= dIndex ? in[i - dIndex] : 0;
        }
    }
}


SPECIFIER void FN(shrBigint)(const WORD_TYPE *in, WORD_TYPE *out, size_t n, unsigned shiftAmount)
{
    size_t dIndex = shiftAmount / WORD_BITS;
    size_t dShift = shiftAmount % WORD_BITS;
    size_t i;

    if (dShift)
    {
        for (i = 0; i < n; i++)
        {
            WORD_TYPE newWord = 0;
            WORD_TYPE part;

            part = i + dIndex < n ? in[i + dIndex] : 0;
            newWord |= part >> dShift;

            part = i + dIndex + 1 < n ? in[i + dIndex + 1] : 0;
            newWord |= part << (WORD_BITS - dShift);

            out[i] = newWord;
        }
    }
    else
    {
        /* Zero shift case can be simplified. Also we cam avoid out of range shifts. */
        for (i = 0; i < n; i++)
        {
            out[i] = i + dIndex < n ? in[i + dIndex] : 0;
        }
    }
}


SPECIFIER void FN(andBigint)(const WORD_TYPE *in1, const WORD_TYPE *in2, WORD_TYPE *out, size_t n)
{
    size_t i;

    for (i = 0; i < n; i++)
    {
        out[i] = in1[i] & in2[i];
    }
}


SPECIFIER void FN(orBigint)(const WORD_TYPE *in1, const WORD_TYPE *in2, WORD_TYPE *out, size_t n)
{
    size_t i;

    for (i = 0; i < n; i++)
    {
        out[i] = in1[i] | in2[i];
    }
}


SPECIFIER void FN(xorBigint)(const WORD_TYPE *in1, const WORD_TYPE *in2, WORD_TYPE *out, size_t n)
{
    size_t i;

    for (i = 0; i < n; i++)
    {
        out[i] = in1[i] ^ in2[i];
    }
}


SPECIFIER int FN(lessThanBigint)(const WORD_TYPE *a, const WORD_TYPE *b, size_t n)
{
    while (n --> 0)
    {
        if (a[n] < b[n]) return 1;
    }
    return 0;
}


SPECIFIER int FN(equalBigint)(const WORD_TYPE *a, const WORD_TYPE *b, size_t n)
{
    while (n --> 0)
    {
        if (a[n] != b[n]) return 0;
    }
    return 1;
}


#include <stdio.h>


SPECIFIER void FN(divModBigint)(
    const WORD_TYPE *dividend,
    const WORD_TYPE *divisor,
    WORD_TYPE *quotient,
    WORD_TYPE *remainder,
    size_t n
)
{
    size_t i;

    i = 0;
    for (i = 0; i < n; i++)
    {
        quotient[i] = 0;
        remainder[i] = 0;
    }

    i = n;

    while (i --> 0)
    {
        size_t j = WORD_BITS;

        while (j --> 0)
        {

            /* Shift in the next digit of the dividend. */
            shlBigint(remainder, remainder, n, 1);
            remainder[0] |= !!(dividend[i] & (1 << j));

            /* Check if we can add one to the output. */
            if (!lessThanBigint(remainder, divisor, n))
            {
                quotient[i] |= 1 << j;
                subBigint(remainder, divisor, remainder, n);
            }
        }
    }
}




#endif


#include "meta/templatefooter.h"
#undef WORD_TYPE
#undef WORD_BITS
#undef HALF_WORD_BITS
#undef HALF_WORD_BASE
#undef HALF_WORD_MASK
#undef DECLARE_STUFF
#undef DEFINE_STUFF

