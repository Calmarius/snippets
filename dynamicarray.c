#ifdef UNIT_TEST

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    int *buffer;
    size_t nElements;
    size_t nAllocated;
} IntVec, *PIntVec;

#define ARRAY PIntVec
#define PREFIX Int_
#define T int
#define ARR_BUF(arr) (arr)->buffer
#define SET_BUF(arr, newBuf)  (arr)->buffer = (newBuf)
#define N_ELEMENTS(arr) (arr)->nElements
#define SET_N_ELEMENTS(arr, n) (arr)->nElements = (n)
#define N_ALLOCD(arr) (arr)->nAllocated
#define SET_N_ALLOCD(arr, n) (arr)->nAllocated = (n)
#define DEFINE_STUFF
#include "dynamicarray.h"

#include <assert.h>

int main()
{
    IntVec testVec;
    IntVec *pTestVec = &testVec;

    Int_init(pTestVec);
    assert(testVec.buffer == NULL);
    assert(testVec.nElements == 0);
    assert(testVec.nAllocated == 0);

    Int_add(pTestVec, 32);
    assert(testVec.buffer != NULL);
    assert(testVec.nElements == 1);
    assert(testVec.nAllocated >= 1);
    assert(testVec.buffer[0] == 32);

    Int_add(pTestVec, 42);
    assert(testVec.nElements == 2);
    assert(testVec.nAllocated >= 2);
    assert(testVec.buffer[0] == 32);
    assert(testVec.buffer[1] == 42);

    Int_reserve(pTestVec, 10);
    assert(testVec.nElements == 2);
    assert(testVec.nAllocated >= 10);

    Int_reserve(pTestVec, 6);
    assert(testVec.nElements == 2);
    assert(testVec.nAllocated >= 10);

    Int_deinit(pTestVec);

    return 0;
}


#endif