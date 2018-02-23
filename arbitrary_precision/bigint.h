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
    #error Please define BIGINT_TYPE to be the type that represents a big integer.
#endif

#ifndef GETWORD
    /* Example: #define GETWORD(bi, i) ((bi)->words[i]) */
    #error Please define GETWORD to be able to read a word in the bigint. Words must be in lettle endian order.
#endif

#ifndef SETWORD
    /* Example: #define SETWORD(bi, i, word) ((bi)->words[i] = (word)) */
    #error Please define SETWORD to be able to set a word in the bigint. Words must be in lettle endian order.
#endif

#ifndef GETNWORDS
    /* Example: #define GETNWORDS(bi) (bi->nWords) */
    #error Please define GETNWORDS to be the macro the queries the number of words in a bigint. This can be defined to be a constant if the number of words is empty.
#endif

#ifndef SETNWORDS
    /* Example: #define SETNWORDS(bi, n) ((bi)->nWords = (n)) */
    #error Please define SETNWORDS to be the macro the set the number of words in a bigint. This can be defined to be empty if the number of words is fixed.
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
 * Multiplies two big integers.
 *
 * a (in): The first number.
 * b (in): The second number.
 * result (out): The result of the multiplication. Words in little endian order.
 *      The number of the words in the result will be the sum of the number of the words in the two argument.
 *
 * Outputs must not point to the inputs.
 */
SPECIFIER void FN(mulBigint)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *result);


/**
 * Shifts big integer left.
 *
 * in (in): The input big integer.
 * out (out): The output big integer.
 * shiftAmount (in): The amount to shift.
 *
 * Words in little endian order.
 * The output can be the same as input.
 */
SPECIFIER void FN(shlBigint)(const BIGINT_TYPE *in, BIGINT_TYPE *out, unsigned shiftAmount);


/**
 * Shifts big integer right. It's not sign preserving.
 *
 * in (in): The input big integer.
 * out (out): The output big integer.
 * shiftAmount (in): The amount to shift.
 *
 * Words in little endian order.
 * The output can be the same as input.
 */
SPECIFIER void FN(shrBigint)(const BIGINT_TYPE *in, BIGINT_TYPE *out, unsigned shiftAmount);


/**
 * Bitwise AND of two big integers.
 *
 * in1, in2 (in): The two arguments.
 * out (out): The result.
 *
 * Words in little endian order.
 * The output can be the same as input.
 */
SPECIFIER void FN(andBigint)(const BIGINT_TYPE *in1, const BIGINT_TYPE *in2, BIGINT_TYPE *out);


/**
 * Bitwise OR of two big integers.
 *
 * in1, in2 (in): The two arguments.
 * out (out): The result.
 *
 *
 * Words in little endian order.
 * The output can be the same as input.
 */
SPECIFIER void FN(orBigint)(const BIGINT_TYPE *in1, const BIGINT_TYPE *in2, BIGINT_TYPE *out);


/**
 * Bitwise XOR of two big integers.
 *
 * in1, in2 (in): The two arguments.
 * out (out): The result.
 *
 * Words in little endian order.
 * The output can be the same as input.
 */
SPECIFIER void FN(xorBigint)(const BIGINT_TYPE *in1, const BIGINT_TYPE *in2, BIGINT_TYPE *out);


/**
 * Compares two bigints.
 *
 * a, b (in): The two numbers to compare.
 *
 * Returns non-zero if 'a' is less than 'b'.
 *
 * Words in little endian order.
 */
SPECIFIER int FN(lessThanBigint)(const BIGINT_TYPE *a, const BIGINT_TYPE *b);


/**
 * Compares two bigints.
 *
 * a, b (in): The two numbers to compare.
 *
 * Returns non-zero if 'a' equals 'b'.
 *
 * Words in little endian order.
 */
SPECIFIER int FN(equalBigint)(const BIGINT_TYPE *a, const BIGINT_TYPE *b);


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
    const BIGINT_TYPE *dividend,
    const BIGINT_TYPE *divisor,
    BIGINT_TYPE *quotient,
    BIGINT_TYPE *remainder
);

#ifdef NUM_THEORY

#ifndef COPY_BIGINT
    /* Example: #define COPY_BIGINT(dst, src) *dst = *src */
    #error Please define COPY_BIGINT as a means to copy big integers.
#endif

SPECIFIER void FN(gcdEuclidean)(
    const BIGINT_TYPE *a,
    const BIGINT_TYPE *b,
    BIGINT_TYPE *gcd
);
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
    size_t nA = GETNWORDS(a);
    size_t nB = GETNWORDS(b);
    size_t n = nA > nB ? nA : nB;

    SETNWORDS(result, n);

    for (i = 0; i < n; i++)
    {
        WORD_TYPE aWord = i < nA ? GETWORD(a, i) : 0;
        WORD_TYPE bWord = i < nB ? GETWORD(b, i) : 0;
        WORD_TYPE rWord;

        s = carry;
        carry = FN(addDigit)(aWord, bWord, &r);
        rWord = s + r;
        if (rWord < s) carry = 1;
        SETWORD(result, i, rWord);
    }

    return carry;
}


SPECIFIER int FN(subBigint)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *result)
{
    size_t i;
    int borrow = 0;
    WORD_TYPE r;
    WORD_TYPE s;
    size_t nA = GETNWORDS(a);
    size_t nB = GETNWORDS(b);
    size_t n = nA > nB ? nA : nB;

    SETNWORDS(result, n);

    for (i = 0; i < n; i++)
    {
        WORD_TYPE aWord = GETWORD(a, i);
        WORD_TYPE bWord = GETWORD(b, i);
        WORD_TYPE rWord;

        s = borrow;
        borrow = FN(subDigit)(aWord, bWord, &r);
        rWord = r - s;
        if (rWord > r) borrow = 1;

        SETWORD(result, i, rWord);
    }

    return borrow;
}


SPECIFIER void FN(mulBigint)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *result)
{
    size_t i, j, k;
    size_t nA = GETNWORDS(a);
    size_t nB = GETNWORDS(b);
    size_t nR = nA + nB;

    for (k = 0; k < nR; k++)
    {
        SETWORD(result, k, 0);
    }

    SETNWORDS(result, nR);

    /* long multiplication algorithm. */
    for (i = 0; i < nA; i++)
    {
        for (j = 0; j < nB; j++)
        {
            size_t k = i + j;
            WORD_TYPE aWord = GETWORD(a, i);
            WORD_TYPE bWord = GETWORD(b, j);
            WORD_TYPE high, low;
            WORD_TYPE tmpLow, tmpHigh;
            int carryToHigh, carryToHigher;

            FN(mulDigit)(aWord, bWord, &high, &low);

            carryToHigh = FN(addDigit)(GETWORD(result, i + j), low, &tmpLow);
            carryToHigher = FN(addDigit)(GETWORD(result, i + j + 1), high, &tmpHigh);

            tmpHigh += carryToHigh;
            carryToHigher |= tmpHigh == 0;

            SETWORD(result, i + j, tmpLow);
            SETWORD(result, i + j + 1, tmpHigh);
            {
                for (k = i + j + 2; carryToHigher; k++)
                {
                    assert(k < nR);
                    WORD_TYPE tmp = GETWORD(result, k) + carryToHigher;

                    carryToHigher = tmp == 0;
                    SETWORD(result, k, tmp);
                }
            }
        }
    }
}


SPECIFIER void FN(shlBigint)(const BIGINT_TYPE *in, BIGINT_TYPE *out, unsigned shiftAmount)
{
    size_t dIndex = shiftAmount / WORD_BITS;
    size_t dShift = shiftAmount % WORD_BITS;
    size_t i = GETNWORDS(in);

    SETNWORDS(out, i);

    if (dShift)
    {
        while (i --> 0)
        {
            WORD_TYPE newWord = 0;
            WORD_TYPE part;

            part = i >= dIndex ? GETWORD(in, i - dIndex) : 0;
            newWord |= part << dShift;

            part = i >= dIndex + 1 ? GETWORD(in, i - dIndex - 1) : 0;
            newWord |= part >> (WORD_BITS - dShift);

            SETWORD(out, i, newWord);
        }
    }
    else
    {
        /* Zero shift case can be simplified. Also we cam avoid out of range shifts. */
        while (i --> 0)
        {
            SETWORD(out, i, i >= dIndex ? GETWORD(in, i - dIndex) : 0);
        }
    }
}


SPECIFIER void FN(shrBigint)(const BIGINT_TYPE *in, BIGINT_TYPE *out, unsigned shiftAmount)
{
    size_t dIndex = shiftAmount / WORD_BITS;
    size_t dShift = shiftAmount % WORD_BITS;
    size_t i;
    size_t n = GETNWORDS(in);

    SETNWORDS(out, n);

    if (dShift)
    {
        for (i = 0; i < n; i++)
        {
            WORD_TYPE newWord = 0;
            WORD_TYPE part;

            part = i + dIndex < n ? GETWORD(in, i + dIndex) : 0;
            newWord |= part >> dShift;

            part = i + dIndex + 1 < n ? GETWORD(in, i + dIndex + 1) : 0;
            newWord |= part << (WORD_BITS - dShift);

            SETWORD(out, i, newWord);
        }
    }
    else
    {
        /* Zero shift case can be simplified. Also we cam avoid out of range shifts. */
        for (i = 0; i < n; i++)
        {
            SETWORD(out, i, i + dIndex < n ? GETWORD(in, i + dIndex) : 0);
        }
    }
}


SPECIFIER void FN(andBigint)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *out)
{
    size_t i;
    size_t nA = GETNWORDS(a);
    size_t nB = GETNWORDS(b);
    size_t n = nA < nB ? nA : nB;
    size_t nMax = nA > nB ? nA : nB;

    for (i = 0; i < n; i++)
    {
        SETWORD(out,i, GETWORD(a, i) & GETWORD(b, i));
    }
    for (; i < nMax; i++)
    {
        SETWORD(out, i, 0);
    }

    SETNWORDS(out, nMax);
}


SPECIFIER void FN(orBigint)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *out)
{
    size_t i;
    size_t nA = GETNWORDS(a);
    size_t nB = GETNWORDS(b);
    size_t n = nA < nB ? nA : nB;
    size_t nMax = nA > nB ? nA : nB;
    const BIGINT_TYPE *longer = nA > nB ? a : b;

    for (i = 0; i < n; i++)
    {
        SETWORD(out, i, GETWORD(a, i) | GETWORD(b, i));
    }
    for (; i < nMax; i++)
    {
        SETWORD(out, i, GETWORD(longer, i));
    }

    SETNWORDS(out, nMax);
}


SPECIFIER void FN(xorBigint)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *out)
{
    size_t i;
    size_t nA = GETNWORDS(a);
    size_t nB = GETNWORDS(b);
    size_t n = nA < nB ? nA : nB;
    size_t nMax = nA > nB ? nA : nB;
    const BIGINT_TYPE *longer = nA > nB ? a : b;

    for (i = 0; i < n; i++)
    {
        SETWORD(out, i, GETWORD(a, i) ^ GETWORD(b, i));
    }
    for (; i < nMax; i++)
    {
        SETWORD(out, i, GETWORD(longer, i));
    }

    SETNWORDS(out, nMax);
}


SPECIFIER int FN(lessThanBigint)(const BIGINT_TYPE *a, const BIGINT_TYPE *b)
{
    size_t nA = GETNWORDS(a);
    size_t nB = GETNWORDS(b);
    size_t n = nA > nB ? nA : nB;

    while (n --> 0)
    {
        WORD_TYPE aWord = n < nA ? GETWORD(a, n) : 0;
        WORD_TYPE bWord = n < nB ? GETWORD(b, n) : 0;

        if (aWord < bWord) return 1;
    }
    return 0;
}


SPECIFIER int FN(equalBigint)(const BIGINT_TYPE *a, const BIGINT_TYPE *b)
{
    size_t nA = GETNWORDS(a);
    size_t nB = GETNWORDS(b);
    size_t n = nA > nB ? nA : nB;

    while (n --> 0)
    {
        WORD_TYPE aWord = n < nA ? GETWORD(a, n) : 0;
        WORD_TYPE bWord = n < nB ? GETWORD(b, n) : 0;

        if (aWord != bWord) return 0;
    }
    return 1;
}


SPECIFIER void FN(divModBigint)(
    const BIGINT_TYPE *dividend,
    const BIGINT_TYPE *divisor,
    BIGINT_TYPE *quotient,
    BIGINT_TYPE *remainder
)
{
    size_t i;
    size_t nD = GETNWORDS(dividend);
    size_t nS = GETNWORDS(divisor);

    i = 0;
    if (quotient)
    {
        for (i = 0; i < nD; i++)
        {
            SETWORD(quotient, i, 0);
        }
    }
    for (i = 0; i < nS; i++)
    {
        SETWORD(remainder, i, 0);
    }

    if (quotient) {SETNWORDS(quotient, nD);}
    SETNWORDS(remainder, nS);

    i = nD;

    while (i --> 0)
    {
        size_t j = WORD_BITS;
        WORD_TYPE dividendWord = GETWORD(dividend, i);

        while (j --> 0)
        {
            /* Shift in the next digit of the dividend. */
            FN(shlBigint)(remainder, remainder, 1);
            SETWORD(remainder, 0, GETWORD(remainder, 0) | !!(dividendWord & (1 << j)));

            /* Check if we can add one to the output. */

            if (!FN(lessThanBigint)(remainder, divisor))
            {
                if (quotient) SETWORD(quotient, i, GETWORD(quotient, i) | (1 << j));
                FN(subBigint)(remainder, divisor, remainder);
            }
        }
    }
}


#ifdef NUM_THEORY
SPECIFIER void FN(gcdEuclidean)(
    const BIGINT_TYPE *a,
    const BIGINT_TYPE *b,
    BIGINT_TYPE *gcd
)
{

}

#endif

#endif


#include "meta/templatefooter.h"
#undef WORD_TYPE
#undef WORD_BITS
#undef BIGINT_TYPE
#undef GETWORD
#undef SETWORD
#undef GETNWORDS
#undef SETNWORDS
#undef HALF_WORD_BITS
#undef HALF_WORD_BASE
#undef HALF_WORD_MASK
#undef NUM_THEORY
#undef COPY_BIGINT

#undef DECLARE_STUFF
#undef DEFINE_STUFF

