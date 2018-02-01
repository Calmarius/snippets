#include "meta/templateheader.h"

#include <stddef.h>

#ifndef ELEM
    #error Handle to the element type is not defined. (The handle to the structure that contains the links to the next and previous elements, and the payload itself)
#endif

#ifndef LIST_TYPE
    #error Handle to the list must be defined. (Handle to a structure that contains the list head and tail.)
#endif


#ifndef GET_HEAD
    /* GET_HEAD(list) --> ELEM: Gets the head element of the list. */
    #error GET_HEAD is not defined
#endif

#ifndef SET_HEAD
    /* SET_HEAD(list, ELEM elem): Sets the head element of the list. */
    #error SET_HEAD is not defined
#endif

/* Gets and sets the link to the next element. */
#ifndef GET_NEXT
    #define GET_NEXT(e) (e)->next
#endif

#ifndef SET_NEXT
    #define SET_NEXT(e, newNext) ((e)->next = (newNext))
#endif

#ifndef GET_TAIL
    /* GET TAIL(list) --> ELEM: Gets the tail element of the list. */
#endif

#ifndef SET_TAIL
    /* SET_TAIL(list, ELEM elem): Sets the head element of the list. */
#endif

/* Gets and sets the link to the previous element, optional. */
#ifndef GET_PREV
    /* #define GET_PREV(e) (e)->prev    */
#endif

#ifndef SET_PREV
    /* #define SET_PREV(e, newPrev) ((e)->prev = (newPrev))*/
#endif

/* Macro to compare two node, needed for sorting. */
#ifndef LESS
    /*  #define LESS(elem1, elem2) (elem1)->payload < (elem2)->payload */
#endif

/* The pointer that doesn't point any node. */
#ifndef NULL_LINK
    #define NULL_LINK NULL
#endif

#ifdef DECLARE_STUFF

/* Initializes empty list. */
SPECIFIER void FN(initList)(LIST_TYPE list);

/* Adds new element to the list at the head. */
SPECIFIER FN(addHead)(LIST_TYPE list, ELEM elem);

/* Adds new element to the list at the tail. */
#if defined(GET_TAIL)  && defined(SET_TAIL)
    SPECIFIER FN(addTail)(LIST_TYPE list, ELEM elem);
#endif

/* Insert an element after a given element. Note: it's doesn't update any head an tail pointers.*/
    SPECIFIER void FN(insertAfter)(ELEM elem, ELEM toBeInserted);

/* Performs merge sort on the list. */
    SPECIFIER FN(sort)(LIST_TYPE list);

/* Removes the element at the head. It updates the head pointer, and the tail pointer if necessary. */
    SPECIFIER void FN(removeHead)(LIST_TYPE list);

#ifdef SET_TAIL
/* Removes the element at the tail. It updates the tail pointer, and the head pointer if necessary. */
    SPECIFIER void FN(removeTail)(LIST_TYPE list);
#endif

#ifdef LESS
/* Sorts the linked list according to the sorting criteria specified by the LESS macro. */
    SPECIFIER void FN(sort)(LIST_TYPE list);
#endif


#endif /*DECLARE_STUFF*/


#ifdef DEFINE_STUFF

SPECIFIER void FN(initList)(LIST_TYPE list)
{
    SET_HEAD(list, NULL_LINK);
    #ifdef SET_TAIL
        SET_TAIL(list, NULL_LINK);
    #endif
}

SPECIFIER void FN(addHead)(LIST_TYPE list, ELEM elem)
{
    ELEM head = GET_HEAD(list);

    SET_NEXT(elem, head);
    SET_HEAD(list, elem);

    #if defined(GET_TAIL) && defined(SET_TAIL)
    if (GET_TAIL(list) == NULL_LINK) SET_TAIL(list, elem);
    #endif

    #if defined(GET_PREV) && defined(SET_PREV)
    if (head != NULL_LINK) SET_PREV(head, elem);
    SET_PREV(elem, NULL_LINK);
    #endif
}


SPECIFIER void FN(insertAfter)(ELEM elem, ELEM toBeInserted)
{
    ELEM next = NULL_LINK;

    /* Attempting to insert after a NULL element will just null out the pointers of the inserted element. This is necessary to be able to seamlessly insert into empty lists. */
    if (elem != NULL_LINK)
    {
        next = GET_NEXT(elem);
        SET_NEXT(elem, toBeInserted);
    }

    SET_NEXT(toBeInserted, next);
    #ifdef SET_PREV
        SET_PREV(toBeInserted, elem);
        if (next != NULL_LINK) SET_PREV(next, toBeInserted);
    #endif
}


SPECIFIER void FN(removeHead)(LIST_TYPE list)
{
    ELEM head = GET_HEAD(list);
    ELEM next = GET_NEXT(head);

    #ifdef SET_PREV
        if (next != NULL_LINK) SET_PREV(next, NULL_LINK);
    #endif

    SET_HEAD(list, next);
    #ifdef SET_TAIL
        if (next == NULL_LINK) SET_TAIL(list, NULL_LINK);
    #endif
}


#ifdef GET_TAIL
SPECIFIER void FN(removeTail)(LIST_TYPE list)
{
    ELEM tail = GET_TAIL(list);
    ELEM prev = GET_PREV(tail);

    if (prev != NULL_LINK) SET_NEXT(prev, NULL_LINK);

    SET_TAIL(list, prev);
    if (prev == NULL_LINK) SET_HEAD(list, NULL_LINK);
}
#endif


#if defined(GET_TAIL)  && defined(SET_TAIL)
SPECIFIER void FN(addTail)(LIST_TYPE list, ELEM elem)
{
    ELEM tail = GET_TAIL(list);

    if (tail != NULL_LINK) SET_NEXT(tail, elem);
    SET_TAIL(list, elem);

    if (GET_HEAD(list) == NULL_LINK) SET_HEAD(list, elem);

    #if defined(GET_PREV) && defined(SET_PREV)
    SET_PREV(elem, tail);
    #endif
    SET_NEXT(elem, NULL_LINK);
}
#endif /*defined(GET_TAIL)  && defined(SET_TAIL)*/


#if defined(LESS)
SPECIFIER void FN(sort)(LIST_TYPE list)
{
    size_t segmentSize = 1;
    ELEM newList = GET_HEAD(list);
    ELEM prevList;
    #ifdef SET_TAIL
        ELEM prevTail = NULL_LINK;
    #endif

    for (;; segmentSize *= 2)
    {
        ELEM cursor;
        size_t i, j;
        int segCount = 0;
        ELEM segments[2];
        ELEM tail;

        prevList = newList;
        newList = NULL_LINK;
        #ifdef SET_TAIL
            prevTail = tail;
        #endif
        tail = NULL_LINK;
        cursor = prevList;

        while (cursor != NULL_LINK)
        {
            ELEM pc;

            segments[0] = NULL_LINK;
            segments[1] = NULL_LINK;

            for (j = 0; j < 2; j++)
            {
                /* Find the end of the first segment*/
                segments[j] = cursor;
                segCount++;
                for (i = 0; i < segmentSize; i++)
                {
                    pc = cursor;
                    cursor = GET_NEXT(cursor);
                    if (cursor == NULL_LINK) goto breakout; /* Not enough elements to make a segment go to the next phase. */
                }

                /* Cut the list */
                #ifdef SET_PREV
                    if (cursor != NULL_LINK) SET_PREV(cursor, NULL_LINK);
                #endif
                if (pc != NULL_LINK) SET_NEXT(pc, NULL_LINK);
            }
        breakout:
            if (segCount == 1) goto done; /* We couldn't make two segments from the list, the sorting is done.  */

            /* Do the merge.*/
            {
                ELEM s1 = segments[0];
                ELEM s2 = segments[1];
                ELEM tmp;

                while ((s1 != NULL_LINK) && (s2 != NULL_LINK))
                {
                    if (LESS(s1, s2))
                    {
                        tmp = GET_NEXT(s1);
                        FN(insertAfter)(tail, s1);
                        if (tail == NULL_LINK) newList = s1;
                        tail = s1;
                        s1 = tmp;
                    }
                    else
                    {
                        tmp = GET_NEXT(s2);
                        FN(insertAfter)(tail, s2);
                        if (tail == NULL_LINK) newList = s2;
                        tail = s2;
                        s2 = tmp;
                    }
                }
                while (s1 != NULL_LINK)
                {
                    tmp = GET_NEXT(s1);
                    FN(insertAfter)(tail, s1);
                    if (tail == NULL_LINK) newList = s1;
                    tail = s1;
                    s1 = tmp;
                }
                while (s2 != NULL_LINK)
                {
                    tmp = GET_NEXT(s2);
                    FN(insertAfter)(tail, s2);
                    if (tail == NULL_LINK) newList = s2;
                    tail = s2;
                    s2 = tmp;
                }
            }
        }
    }
done:
    SET_HEAD(list, prevList);
    #ifdef SET_TAIL
        SET_TAIL(list, prevTail);
    #endif
}
#endif /*defined(LESS)*/



#endif /*DEFINE_STUFF*/


#include "meta/templatefooter.h"

#undef ELEM
#undef LIST_TYPE
#undef GET_HEAD
#undef SET_HEAD
#undef GET_NEXT
#undef SET_NEXT

#undef GET_TAIL
#undef SET_TAIL
#undef GET_PREV
#undef SET_PREV

#undef DECLARE_STUFF
#undef DEFINE_STUFF
#undef NULL_LINK
#undef LESS