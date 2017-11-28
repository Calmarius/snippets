#include "templateheader.h"

#ifndef ARRAY
    #error the handle type for the array must be provided.
#endif

#ifndef T
    #error you must define the element type of the array.
#endif

/**
 * Accesses the array's buffer.
 */
#ifndef ARR_BUF
    #define AT(arr) arr.buf
#endif

/**
 * Function to set new buffer into the array.
 */
#ifndef SET_BUF
    #define SET_BUF(arr, newbuf) arr.buf = newbuf
#endif

/**
 * The number of allocated entries in the array.
 */
#ifndef N_ALLOCD(arr)
    #define N_ALLOCD(arr) arr.nAllocd
#endif

/**
 * The number of actual elements in the array.
 */
#ifndef N_ELEMENTS(arr)
    #define N_ELEMENTS(arr) arr.n
#endif


/**
 * Macro to reallocate a buffer to hold n elements.
 */
#ifndef REALLOCATE
    #define REALLOCATE(buf, n) realloc(buf, (n)*sizeof(T))
#endif

/**
 * The function that determines the new size of array when the adding increments it.
 */
#ifndef INCREMENT_FN
    #define INCREMENT_FN(x) (2 *(x) + 1)
#endif

#ifdef DECLARE_STUFF
    /**
     * Reserves the given area in the array.
     *
     * Returns 0 on success, nonzero on failure.
     *
     * If the requested number of elements is smaller than the number of already allocated elements, the function does nothing.
     * If the requested number of elements is larger then the number of elements, the function allocate
     */
    SPECIFIER int FN(reserve)(ARRAY arr, size_t nElements);

    /**
     * Adds elements to the array.
     * Enlarges it if needed.
     * Returns 0 on successful adding, non-zero otherwise.
     */
    SPECIFIER int FN(add)(ARRAY arr, T elem);
#endif

#ifdef DEFINE_STUFF

SPECIFIER int FN(reserve)(ARRAY arr, size_t nElements)
{
    T *newBuf = NULL;

    if (nElements <= N_ALLOCD(arr)) return 0;

    newBuf = REALLOCATE(ARR_BUF(arr), nElements);
    if (newBuf == NULL) return -1;

    SET_BUF(arr, newBuf);
    return 0;
}


SPECIFIER int FN(add)(ARRAY arr, const T *elem)
{
    size_t nElms = N_ELEMENTS(arr);

    if (nElms == N_ALLOCD(arr))
    {
        int res = FN(reserve)(arr, INCREMENT_FN(nElms));
        if (res) return res;
    }

    ARR_BUF(arr)[nElms] = *elem;
}


#endif



#include "templatefooter.h"
#undef ARRAY
#undef T
#undef ARR_BUF
#undef N_ALLOCD
#undef N_ELEMENTS
#undef REALLOCATE
#undef INCREMENT_FN
#undef DECLARE_STUFF
#undef DEFINE_STUFF
