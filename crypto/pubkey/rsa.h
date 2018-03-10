#include "meta/templateheader.h"

#ifndef BIGNUM
    /* A structure representing a big integer. */
    #error Please define BIGNUM to represent unsigned integers.
#endif

#ifndef BIGNUM_RELEASE/*(bignum)*/
    /* This macro should release the memory associated with the big number. */
    #error Please define BIGNUM_RELEASE(bignum) to clean up big numbers.
#endif

#ifndef MUL/*(a, b, out)*/
    /* This macro should multiply two big numbers. Inputs: a,b. Output: out. Output is newly allocated, no overflow allowed. */
    #error Please define MUL(a, b, out) to multiply big numbers
#endif

#ifndef SUB/*(a, b, result)*/
    /* This macro should subtract two big numbers. Inputs: a,b. Output: result. Output is newly allocated.*/
    #error Please define SUB(a, b, result) to subtract big integers
#endif

#ifndef LCM
    /* Inputs: a,b, outputs: out. Output is newly allocated. */
    #error Please define LCM(a, b, out) to calculate the least common multiple.
#endif

#ifndef XGCD/*(a, b, x, y, gcd)*/
    /* This macro should solve ax + by = gcd(a,b) equation. Inputs: a,b. Outputs: x, y, gcd. Outputs are newly allocated.*/
    #error Please define XGCD(a,b, x, y, gcd) as a way to run the extended euclidean algorithm.
#endif

#ifndef EQUAL/*(a,b)*/
    /* Inputs: a,b. Returns non-zero if the two numbers are equal, zero otherwise. */
    #error Please define EQUAL(a, b) to compare two big numbers.
#endif

#ifndef ONE
    #error Please define ONE to be the pointer to a big integer representing 1.
#endif

#ifdef DECLARE_STUFF

/**
 * Generates the private key and modulus from the provided arguments.
 *
 * prime, prime2 (in): The two primes used for the generation.
 * pubExponent (in): The public exponent to use.
 * privExponent (out): The private key calculated.
 * modulus (out): The modulus.
 *
 * Returns 0 on success.
 * Returns -1 if privExponent divides the modulus.
 *
 * It will not check if the primes are really primes, it's assumed the caller verified them before calling this function.
 */
SPECIFIER int FN(rsaMakeKeys)(const BIGNUM *prime1, const BIGNUM *prime2, const BIGNUM *pubExponent, BIGNUM *privExponent, BIGNUM *modulus);

#endif

#ifdef DEFINE_STUFF

SPECIFIER int FN(rsaMakeKeys)(const BIGNUM *prime1, const BIGNUM *prime2, const BIGNUM *pubExponent, BIGNUM *privExponent, BIGNUM *modulus)
{
    BIGNUM totient;
    BIGNUM pM1, pM2;
    BIGNUM k,gcd;
    int retVal = 0;

    MUL(prime1, prime2, modulus); /* n = pq*/
    SUB(prime1, ONE, &pM1);
    SUB(prime2, ONE, &pM2);
    LCM(&pM1, &pM2, &totient); /* totient = lcm(p-1, q-1) */

    XGCD(pubExponent, &totient, privExponent, &k, &gcd);

    if (!EQUAL(&gcd, ONE))
    {
        retVal = -1;
        BIGNUM_RELEASE(privExponent);
        BIGNUM_RELEASE(modulus);
        goto cleanup;
    }

cleanup:
    BIGNUM_RELEASE(&totient);
    BIGNUM_RELEASE(&pM1);
    BIGNUM_RELEASE(&pM2);
    BIGNUM_RELEASE(&k);
    return retVal;
}

#endif


#include "meta/templatefooter.h"

#undef BIGNUM
#undef MUL
#undef SUB
#undef XGCD
#undef EQUAL
#undef ONE

