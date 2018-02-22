/**
 * Basic arithmetic operations and various theory functions on arbitrary precision integers.
 */

#include "meta/templateheader.h"

#include <stddef.h>

#ifndef WORD_TYPE
    #error Please define WORD_TYPE to the largest unsigned type your CPU natively supports.
#endif

#ifndef WORD_BITS
    #error Please define WORD_BITS to be the bits of the word.  (Eg. for a 32 bit unsigned integer it should be 32).
#endif

#ifndef BIGINT_TYPE
    /* It must be a value type. It shouldn't be a handle. */
    #error Please defined BIGINT_TYPE to be the type that represents a big integer.
#endif

#ifndef BIGINT_GETWORD
    /* Example: #define BIGINT_GETWORD(bi, i) ((bi).words[i]) */
    #error Please define BIGINT_GETWORD to be able to read a word in the bigint.
#endif

#ifndef BIGINT_SETWORD
    /* Example: #define BIGINT_SETWORD(bi, i, word) ((bi).words[i] = (word)) */
    #error Please define BIGINT_SETWORD to be able to set a word in the bigint.
#endif

#ifndef BIGINT_GETNWORDS
    /* Example: #define BIGINT_GETNWORDS(bi) (bi.nWords) */
    #error Please define BIGINT_GETNWORDS to be the macro the queries the number of words in a bigint.
#endif

#ifndef BIGINT_SETNWORDS
    /* Example: #define BIGINT_SETNWORDS(bi, n) ((bi).nWords = (n)) */
    #error Please define BIGINT_SETNWORDS to be the macro the set the number of words in a bigint.
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
 * a,b (in): The two bigints to add.
 * result (out): The result, the number of the words in the result is the number of the words in the longer input (that has more words)
 *
 * The function works correctly if the output matches one of its inputs.
 *
 * Returns the carry.
 **/
SPECIFIER int FN(addBigint)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *result);


/**
 * Subtracts two big integers.
 *
 * a,b (in): The two bigints to subtract (a-b) will be calculated.
 * result (out): The result, the number of the words in the result is the number of the words in the longer input (that has more words)
 *
 * The function works correctly if the output matches one of its inputs.
 *
 * Returns the borrow.
 **/
SPECIFIER int FN(subBigint)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *result);

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
 * quotient (opt), remainder (out): as their name suggests... The quotient can be NULL if only the modulus is needed.
 *
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

#ifdef NUM_THEORY
#endif


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


SPECIFIER int FN(addBigint)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *result)
{
    size_t i;
    int carry = 0;
    WORD_TYPE r;
    WORD_TYPE s;
    size_t nA = BIGINT_GETNWORDS(*a);
    size_t nB = BIGINT_GETNWORDS(*b);
    size_t n = nA > nB ? nA : nB;

    BIGINT_SETNWORDS(*result, n);

    for (i = 0; i < n; i++)
    {
        WORD_TYPE aWord = i < nA ? BIGINT_GETWORD(*a, i) : 0;
        WORD_TYPE bWord = i < nB ? BIGINT_GETWORD(*b, i) : 0;
        WORD_TYPE rWord;

        s = carry;
        carry = FN(addDigit)(aWord, bWord, &r);
        rWord = s + r;
        if (rWord < s) carry = 1;
        BIGINT_SETWORD(*result, i, rWord);
    }

    return carry;
}


SPECIFIER int FN(subBigint)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *result)
{
    size_t i;
    int borrow = 0;
    WORD_TYPE r;
    WORD_TYPE s;
    size_t nA = BIGINT_GETNWORDS(*a);
    size_t nB = BIGINT_GETNWORDS(*b);
    size_t n = nA > nB ? nA : nB;

    for (i = 0; i < n; i++)
    {
        WORD_TYPE aWord = BIGINT_GETWORD(*a, i);
        WORD_TYPE bWord = BIGINT_GETWORD(*b, i);
        WORD_TYPE rWord;

        s = borrow;
        borrow = FN(subDigit)(aWord, bWord, &r);
        rWord = r - s;
        if (rWord > r) borrow = 1;

        BIGINT_SETWORD(*result, i, rWord);
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
    (void)dividend;
    (void)divisor;
    (void)quotient;
    (void)remainder;
    (void)n;
    #if 0
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
    #endif
}


#ifdef NUM_THEORY
#endif

#endif


#include "meta/templatefooter.h"
#undef WORD_TYPE
#undef WORD_BITS
#undef BIGINT_TYPE
#undef BIGINT_GETWORD
#undef BIGINT_SETWORD
#undef BIGINT_GETNWORDS
#undef BIGINT_SETNWORDS
#undef HALF_WORD_BITS
#undef HALF_WORD_BASE
#undef HALF_WORD_MASK
#undef DECLARE_STUFF
#undef DEFINE_STUFF
#undef NUM_THEORY

