/**
 * Basic arithmetic operations and various theory functions on arbitrary precision integers.
 * The functions are operating on UNSIGNED big integers, unless noted otherwise.
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

#ifndef ZERO_BIGINT
    /* Example: #define ZEROBIGINT(bi) (memset(bi, 0, (bi).n * sizeof(WORD_TYPE))*/
    /* This macro should zero all words in the specified bigint. */
    #error Please define ZERO_BIGINT to be the macro that zeroes big integers.
#endif

#ifndef SETNWORDS
    /* Example: #define SETNWORDS(bi, n) ((bi)->nWords = (n)) */
    #error Please define SETNWORDS to be the macro the set the number of words in a bigint. This can be defined to be empty if the number of words is fixed.
#endif

#ifndef DUMP_BIGINT
	/* Use for debugging.*/
	#define DUMP_BIGINT(bigint, misc_string)
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
 * Returns non-zero if the passed number is zero.
 *
 * x (in): The number to check.
 */
SPECIFIER int FN(isZero)(const BIGINT_TYPE *x);

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
SPECIFIER int FN(add)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *result);


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
SPECIFIER int FN(sub)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *result);


/**
 * Multiplies two big integers.
 *
 * a (in): The first number.
 * b (in): The second number.
 * result (out): The result of the multiplication. Words in little endian order.
 *      The number of the words in the result will be the sum of the number of the words in the two arguments.
 *      If it cannot hold that many words the result will be truncated.
 *
 * Outputs must not point to the inputs.
 *
 * Returns non-zero if the result is truncated.
 */
SPECIFIER int FN(mul)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *result);


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
SPECIFIER void FN(shl)(const BIGINT_TYPE *in, BIGINT_TYPE *out, unsigned shiftAmount);


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
SPECIFIER void FN(shr)(const BIGINT_TYPE *in, BIGINT_TYPE *out, unsigned shiftAmount);


/**
 * Bitwise AND of two big integers.
 *
 * in1, in2 (in): The two arguments.
 * out (out): The result.
 *
 * Words in little endian order.
 * The output can be the same as input.
 */
SPECIFIER void FN(and)(const BIGINT_TYPE *in1, const BIGINT_TYPE *in2, BIGINT_TYPE *out);


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
SPECIFIER void FN(or)(const BIGINT_TYPE *in1, const BIGINT_TYPE *in2, BIGINT_TYPE *out);


/**
 * Bitwise XOR of two big integers.
 *
 * in1, in2 (in): The two arguments.
 * out (out): The result.
 *
 * Words in little endian order.
 * The output can be the same as input.
 */
SPECIFIER void FN(xor)(const BIGINT_TYPE *in1, const BIGINT_TYPE *in2, BIGINT_TYPE *out);


/**
 * Compares two bigints.
 *
 * a, b (in): The two numbers to compare.
 *
 * Returns non-zero if 'a' is less than 'b'.
 *
 * Words in little endian order.
 */
SPECIFIER int FN(lessThan)(const BIGINT_TYPE *a, const BIGINT_TYPE *b);


/**
 * Compares two bigints.
 *
 * a, b (in): The two numbers to compare.
 *
 * Returns non-zero if 'a' equals 'b'.
 *
 * Words in little endian order.
 */
SPECIFIER int FN(equal)(const BIGINT_TYPE *a, const BIGINT_TYPE *b);


/**
 * Bigint long division algorithm.
 *
 * dividend, divisor (in): as their name suggests...
 * quotient (opt), remainder (out): as their name suggests... The quotient can be NULL if only the modulus is needed.
 *
 * The quotient must be able to hold the same amount of words as the dividend.
 * The remainder must be able to hold the same amount of words as the divisor.
 *
 * Words in little endian order.
 *
 * There is no check against zero division. Because it's expected that the user already does that.
 * Anyway at the end of the algorithm it is indicated by remainder == dividend and quotient == all 1 bits.
 */
SPECIFIER void FN(divMod)(
    const BIGINT_TYPE *dividend,
    const BIGINT_TYPE *divisor,
    BIGINT_TYPE *quotient,
    BIGINT_TYPE *remainder
);


/**
 * Performs integer exponentiation modulo a given number.
 *
 * base, exponent, modulo (in): The inputs.
 * result (out): The result. It will hold the same amount of words as the modulo holds.
 *
 * Returns non-zero if truncation occured during the calculation this also indicates the result is incorrect.
 */
SPECIFIER int FN(modPow)(
    const BIGINT_TYPE *base,
    const BIGINT_TYPE *exponent,
    const BIGINT_TYPE *modulo,
    BIGINT_TYPE *result
);

#ifdef NUM_THEORY

#ifndef COPY_BIGINT
    /* Example: #define COPY_BIGINT(dst, src) *dst = *src */
    /* This macro should perform the copying of big ints it should make sure the previous content in destination is freed when needed.
     * On failed allocation the macro should jump to the 'cleanup' label, and possibly set a thread local error variable.
     */
    #error Please define COPY_BIGINT as a means to copy big integers.
#endif

#ifndef INIT_BIGINT
    /* Example: #define INIT_BIGINT(bi, nWords) ((bi)->n = (nWords);*/
    /* This (re)initializes big ints to hold the specified amount of words. It should release the previous content if needed.
     * On failed allocation the macro should jump to the 'cleanup' label, and possibly set a thread local error variable.
     */
    #error Please define INIT_BIGINT as a way to initialize space in big integers.
#endif

#ifndef DEINIT_BIGINT
    /* This is needed to deinit big integers cleanly. (only if memory management needed)*/
    #error Please define DEINIT_BIGINT as a way to clean up space in big integers.
#endif

/**
 * Calculates the greatest common divisor of the big integers.
 *
 * a, b (in): The two numbers of interest.
 * gcd (out): Their GCD.
 *
 * The number of words in GCD matches the number of words in b.
 *
 */
SPECIFIER void FN(gcdEuclidean)(
    const BIGINT_TYPE *a,
    const BIGINT_TYPE *b,
    BIGINT_TYPE *gcd
);

/**
 * Solves the ax + by = gcd(a,b) equation on integer numbers.
 *
 * a, b (in): The two input parameters.
 * x, y, gcd (out): The two unknown and the coefficient, they will be calculated by the algorithm.
 *
 * The x, y and gcd is initialized by this function.
 * The x will have the same amount of words as b.
 * The y will have the same amount of words as a.
 * The gcd will have the same amount of words as b.
 *
 */
SPECIFIER void FN(gcdExtendedEuclidean)(
    const BIGINT_TYPE *a,
    const BIGINT_TYPE *b,
    BIGINT_TYPE *x,
    BIGINT_TYPE *y,
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


SPECIFIER int FN(isZero)(const BIGINT_TYPE *x)
{
	size_t n = GETNWORDS(x);

	while (n --> 0)
	{
		if (GETWORD(x, n) != 0) return 0;
	}
	return 1;
}


SPECIFIER int FN(add)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *result)
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


SPECIFIER int FN(sub)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *result)
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


SPECIFIER int FN(mul)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *result)
{
    size_t i, j;
    size_t nA = GETNWORDS(a);
    size_t nB = GETNWORDS(b);
    size_t nR = GETNWORDS(result);
    int truncate = 0;

    ZERO_BIGINT(result);

    /* long multiplication algorithm. */
    for (i = 0; i < nA; i++)
    {
        for (j = 0; j < nB; j++)
        {
            size_t k = i + j;
            WORD_TYPE aWord;
            WORD_TYPE bWord;
            WORD_TYPE high, low;
            WORD_TYPE tmpLow, tmpHigh;
            int carryToHigh, carryToHigher;

            if (truncate && (k >= nR))
            {
                /* If we know we are truncating we don't need to care about what would go beyond the last word.. */
                continue;
            }

            aWord = GETWORD(a, i);
            bWord = GETWORD(b, j);

            FN(mulDigit)(aWord, bWord, &high, &low);

            if (k >= nR)
            {
                if (low != 0)
                {
                    /* The low word is nonzero and would go beyond the last word, this means truncation. */
                    /*printf("Truncate low case.\n");*/
                    truncate = 1;
                }
                continue;
            }
            carryToHigh = FN(addDigit)(GETWORD(result, k), low, &tmpLow);
            SETWORD(result, k, tmpLow);
            if (k + 1 >= nR)
            {
                if (!truncate && ((high != 0) || carryToHigh))
                {
                    /* We would carry a word or carry beyond the last word in the result this indicates truncation.*/
                    /*printf("Truncation high case:  i: %d, j: %d, k: %d, nR: %d, carryToHigh: %08x, high: %08x, aWord: %08x, bWord: %08x\n",
                        (int)i, (int)j, (int)k, (int)nR, carryToHigh, high, aWord, bWord);*/
                    truncate = 1;
                }
                continue;
            }
            carryToHigher = FN(addDigit)(GETWORD(result, k + 1), high, &tmpHigh);

            tmpHigh += carryToHigh;
            if (carryToHigh)
            {
                /* Check if tmpHigh overflowed, this would indicate another carry higher. */
                carryToHigher |= tmpHigh == 0;
            }

            SETWORD(result, k + 1, tmpHigh);
            for (k = k + 2; (k < nR && carryToHigher); k++)
            {
                WORD_TYPE tmp = GETWORD(result, k) + carryToHigher;

                carryToHigher = tmp == 0;
                SETWORD(result, k, tmp);
            }
            if ((k == nR) && (carryToHigher))
            {
                /* We would carry to higher than the last word. This indicates truncation. */
                /*printf("Truncation carryToHigher case. i: %d, j: %d, k: %d\n", (int)i, (int)j, (int)k);*/
                truncate = 1;
            }
        }
    }
    return truncate;
}


SPECIFIER void FN(shl)(const BIGINT_TYPE *in, BIGINT_TYPE *out, unsigned shiftAmount)
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


SPECIFIER void FN(shr)(const BIGINT_TYPE *in, BIGINT_TYPE *out, unsigned shiftAmount)
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


SPECIFIER void FN(and)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *out)
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


SPECIFIER void FN(or)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *out)
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


SPECIFIER void FN(xor)(const BIGINT_TYPE *a, const BIGINT_TYPE *b, BIGINT_TYPE *out)
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


SPECIFIER int FN(lessThan)(const BIGINT_TYPE *a, const BIGINT_TYPE *b)
{
    size_t nA = GETNWORDS(a);
    size_t nB = GETNWORDS(b);
    size_t n = nA > nB ? nA : nB;

    while (n --> 0)
    {
        WORD_TYPE aWord = n < nA ? GETWORD(a, n) : 0;
        WORD_TYPE bWord = n < nB ? GETWORD(b, n) : 0;

        if (aWord < bWord) return 1;
        if (aWord > bWord) return 0;
    }
    return 0;
}


SPECIFIER int FN(equal)(const BIGINT_TYPE *a, const BIGINT_TYPE *b)
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


SPECIFIER void FN(divMod)(
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
        ZERO_BIGINT(quotient);
    }
    ZERO_BIGINT(remainder);

    if (quotient) {SETNWORDS(quotient, nD);}
    SETNWORDS(remainder, nS);

    i = nD;

    while (i --> 0)
    {
        size_t j = WORD_BITS;
        WORD_TYPE dividendWord = GETWORD(dividend, i);
        WORD_TYPE quotientWord = 0;

        while (j --> 0)
        {
            /* Shift in the next digit of the dividend. */
            FN(shl)(remainder, remainder, 1);
            SETWORD(remainder, 0, GETWORD(remainder, 0) | !!(dividendWord & (1 << j)));

            /* Check if we can add one to the output. */

            if (!FN(lessThan)(remainder, divisor))
            {
                if (quotient) quotientWord |= 1 << j;
                FN(sub)(remainder, divisor, remainder);
            }
        }
        if (quotient) SETWORD(quotient, i, quotientWord);
    }
}


#ifdef NUM_THEORY

SPECIFIER void FN(gcdEuclidean)(
    const BIGINT_TYPE *a,
    const BIGINT_TYPE *b,
    BIGINT_TYPE *gcd
)
{
	BIGINT_TYPE high, low;

	COPY_BIGINT(&high, a);
	COPY_BIGINT(&low, b);

	for (;;)
	{
		FN(divMod)(&high, &low, NULL, gcd);
		if (FN(isZero(gcd)))
		{
			COPY_BIGINT(gcd, &low);
			goto cleanup;
		}

		COPY_BIGINT(&high, &low);
		COPY_BIGINT(&low, gcd);
	}
cleanup:
	DEINIT_BIGINT(&high);
	DEINIT_BIGINT(&low);
}

SPECIFIER void FN(gcdExtendedEuclidean)(
    const BIGINT_TYPE *a,
    const BIGINT_TYPE *b,
    BIGINT_TYPE *x,
    BIGINT_TYPE *y,
    BIGINT_TYPE *gcd
)
{
	BIGINT_TYPE high, low;
    BIGINT_TYPE highHighCoeff, highLowCoeff;
    BIGINT_TYPE lowHighCoeff, lowLowCoeff;
    BIGINT_TYPE remHighCoeff, remLowCoeff;
    BIGINT_TYPE quotient;
    size_t nA = GETNWORDS(a);
    size_t nB = GETNWORDS(b);

	COPY_BIGINT(&high, a);
	COPY_BIGINT(&low, b);

    INIT_BIGINT(&highHighCoeff, nB);
    INIT_BIGINT(&lowHighCoeff, nB);
    INIT_BIGINT(&remHighCoeff, nB);
    INIT_BIGINT(&quotient, nA);

    INIT_BIGINT(&highLowCoeff, nA);
    INIT_BIGINT(&lowLowCoeff, nA);
    INIT_BIGINT(&remLowCoeff, nA);

    ZERO_BIGINT(&highLowCoeff);
    ZERO_BIGINT(&lowLowCoeff);
    ZERO_BIGINT(&remLowCoeff);
    ZERO_BIGINT(&highHighCoeff);
    ZERO_BIGINT(&lowHighCoeff);
    ZERO_BIGINT(&remHighCoeff);

    SETWORD(&highHighCoeff, 0, 1);
    SETWORD(&lowLowCoeff, 0, 1);

	for (;;)
	{
        BIGINT_TYPE tmp;

		FN(divMod)(&high, &low, &quotient, gcd);
		if (FN(isZero(gcd)))
		{
			COPY_BIGINT(gcd, &low);
            COPY_BIGINT(x, &lowHighCoeff);
            COPY_BIGINT(y, &lowLowCoeff);
			goto cleanup;
		}

        INIT_BIGINT(&tmp, GETNWORDS(&highHighCoeff));
        FN(mul)(&quotient, &lowHighCoeff, &tmp);
        FN(sub)(&highHighCoeff, &tmp, &remHighCoeff);

        INIT_BIGINT(&tmp, GETNWORDS(&highLowCoeff));
        FN(mul)(&quotient, &lowLowCoeff, &tmp);
        FN(sub)(&highLowCoeff, &tmp, &remLowCoeff);


		COPY_BIGINT(&high, &low);
        COPY_BIGINT(&highHighCoeff, &lowHighCoeff);
        COPY_BIGINT(&highLowCoeff, &lowLowCoeff);

		COPY_BIGINT(&low, gcd);
		COPY_BIGINT(&lowHighCoeff, &remHighCoeff);
		COPY_BIGINT(&lowLowCoeff, &remLowCoeff);
	}
cleanup:
    DEINIT_BIGINT(&highHighCoeff);
    DEINIT_BIGINT(&lowHighCoeff);
    DEINIT_BIGINT(&remHighCoeff);
    DEINIT_BIGINT(&quotient);

    DEINIT_BIGINT(&highLowCoeff);
    DEINIT_BIGINT(&lowLowCoeff);
    DEINIT_BIGINT(&remLowCoeff);

	DEINIT_BIGINT(&high);
	DEINIT_BIGINT(&low);
}


SPECIFIER int FN(modPow)(
    const BIGINT_TYPE *base,
    const BIGINT_TYPE *exponent,
    const BIGINT_TYPE *modulo,
    BIGINT_TYPE *result
)
{
    BIGINT_TYPE res;
    BIGINT_TYPE mulRes;
    int canStart = 0;
    int truncated = 0;

    INIT_BIGINT(&res, GETNWORDS(modulo));
    INIT_BIGINT(&mulRes, 2*GETNWORDS(base));

    ZERO_BIGINT(&res);
    SETWORD(&res, 0, 1);

    {
        size_t n = GETNWORDS(exponent);

        while (n --> 0)
        {
            size_t m = WORD_BITS;
            WORD_TYPE expWord = GETWORD(exponent, n);

            while (m --> 0)
            {
                if (expWord & (1 << m)) canStart = 1;
                if (!canStart) continue;

                /* Square the number. */
                truncated |= FN(mul)(&res, &res, &mulRes);
                FN(divMod)(&mulRes, modulo, NULL, &res);

                /* Multiply*/
                if (expWord & (1 << m))
                {
                    truncated |= FN(mul)(&res, base, &mulRes);
                    FN(divMod)(&mulRes, modulo, NULL, &res);
                }
            }
        }
        COPY_BIGINT(result, &res);
    }

goto cleanup;
cleanup:
    DEINIT_BIGINT(&res);
    DEINIT_BIGINT(&newRes);
    return truncated;
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
#undef INIT_BIGINT
#undef DUMP_BIGINT
#undef ZERO_BIGINT

#undef DECLARE_STUFF
#undef DEFINE_STUFF

