#ifdef UNIT_TEST

#include <assert.h>
#include <stdio.h>

typedef struct Element
{
    int payload;
    struct Element *next;
    struct Element *prev;
} Element, *PElement;

typedef struct
{
    Element *head;
    Element *tail;
} List, *PList;

#define ELEM PElement
#define LIST_TYPE PList
#define GET_HEAD(list) (list)->head
#define SET_HEAD(list, elem) (list)->head = elem
#define PREFIX SL_
#define DEFINE_STUFF
#include "linkedlist.h"

Element *arrBase;

void dump(Element *elm, const char *caption)
{
    printf("%s: (%d) (index:  %d)\n", caption, elm ? elm->payload : -1, elm ? (int)(elm - arrBase) : -1);
}

#define ELEM PElement
#define LIST_TYPE PList
#define GET_HEAD(list) (list)->head
#define SET_HEAD(list, elem) (list)->head = (elem)
#define GET_TAIL(list) (list)->tail
#define SET_TAIL(list, elem) (list)->tail = (elem)
#define GET_PREV(elem) (elem)->prev
#define SET_PREV(elem, elem2) (elem)->prev = (elem2)
#define LESS(e1, e2) ((e1)->payload < (e2)->payload)
#define PREFIX DLT_
#define DEFINE_STUFF
#include "linkedlist.h"

/* For random. */
#include "simplerandom.c"

#include <time.h>

int main()
{
    /* Single linked, tailless. */
    List list;
    Element e1, e2, e3, e4, e5;

    e1.payload = 1;
    e2.payload = 2;
    e3.payload = 3;
    e4.payload = 4;
    e5.payload = 5;

    /* Testing init. */
    SL_initList(&list);
    assert(list.head == NULL);

    /* Testing add head: */
    SL_addHead(&list, &e1);
    assert(list.head == &e1);
    assert(list.head->next == NULL);
    assert(list.head->payload == 1);

    SL_addHead(&list, &e2);
    assert(list.head == &e2);
    assert(list.head->next == &e1);
    assert(list.head->payload == 2);

    SL_addHead(&list, &e3);
    assert(list.head == &e3);
    assert(list.head->next == &e2);
    assert(list.head->payload == 3);

    assert(list.head->next->next == &e1);
    assert(list.head->next->next->next == NULL);

    SL_insertAfter(&e2, &e4);
    assert(list.head == &e3);
    assert(list.head->next == &e2);
    assert(list.head->next->next == &e4);
    assert(list.head->next->next->next == &e1);
    assert(list.head->next->next->next->next == NULL);

    /* Doubly linked list with tail */

    /* Testing init.*/
    DLT_initList(&list);
    assert(list.head == NULL);
    assert(list.tail == NULL);

    /* Testing adding at the head: */
    DLT_addHead(&list, &e1);
    assert(list.head == &e1);
    assert(list.head->next == NULL);
    assert(list.head->prev == NULL);
    assert(list.head->payload == 1);
    assert(list.head == list.tail);

    DLT_addHead(&list, &e2);
    assert(list.head == &e2);
    assert(list.head->next == &e1);
    assert(list.head->prev == NULL);
    assert(list.head->payload == 2);
    assert(list.tail == &e1);
    assert(e1.prev == list.head);

    DLT_addHead(&list, &e3);
    assert(list.head == &e3);
    assert(list.head->next == &e2);
    assert(list.head->prev == NULL);
    assert(list.head->payload == 3);
    assert(e2.prev == list.head);

    assert(list.head->next->next == &e1);
    assert(list.head->next->next->next == NULL);

    assert(list.tail == &e1);
    assert(list.tail->prev == &e2);
    assert(list.tail->prev->prev == &e3);
    assert(list.tail->prev->prev->prev == NULL);

    DLT_insertAfter(&e2, &e4);
    assert(list.head == &e3);
    assert(list.head->next == &e2);
    assert(list.head->next->next == &e4);
    assert(list.head->next->next->next == &e1);
    assert(list.head->next->next->next->next == NULL);

    assert(list.tail == &e1);
    assert(list.tail->prev == &e4);
    assert(list.tail->prev->prev == &e2);
    assert(list.tail->prev->prev->prev == &e3);
    assert(list.tail->prev->prev->prev->prev == NULL);

    DLT_addTail(&list, &e5);
    assert(list.head == &e3);
    assert(list.head->next == &e2);
    assert(list.head->next->next == &e4);
    assert(list.head->next->next->next == &e1);
    assert(list.head->next->next->next->next == &e5);
    assert(list.head->next->next->next->next->next == NULL);

    assert(list.tail == &e5);
    assert(list.tail->prev == &e1);
    assert(list.tail->prev->prev == &e4);
    assert(list.tail->prev->prev->prev == &e2);
    assert(list.tail->prev->prev->prev->prev == &e3);
    assert(list.tail->prev->prev->prev->prev->prev == NULL);

    /* Sorting*/
    #define ELEMENTS 30
    {
        Element elm[ELEMENTS];
        size_t i;
        unsigned state = time(NULL);

        printf("Seed is: %u\n", state);

        /* Initialization. */
        for (i = 0; i < ELEMENTS; i++)
        {
            elm[i].payload = i;
        }

        /* Shuffling*/
        for (i = 0; i < ELEMENTS - 1; i++)
        {
            size_t with = i + (getRand(&state) % (ELEMENTS - i));
            Element tmp;

            tmp = elm[i];
            elm[i] = elm[with];
            elm[with] = tmp;
        }

        /* Debug print*/
        for (i = 0; i < ELEMENTS; i++)
        {
            printf("Elms[%d]: %d\n", (int)i, elm[i].payload);
        }

        /* Populate list */
        DLT_initList(&list);
        for (i = 0; i < ELEMENTS; i++)
        {
            DLT_addTail(&list, &elm[i]);
        }

        /* Asserts: */
        for (i = 0; i < ELEMENTS; i++)
        {
            if (i > 0)
            {
                assert(elm[i-1].next == &elm[i]);
                assert(elm[i].prev == &elm[i-1]);
            }
            else
            {
                assert(elm[i].prev == NULL);
            }

            if (i < ELEMENTS - 1)
            {
                assert(elm[i+1].prev == &elm[i]);
                assert(elm[i].next == &elm[i+1]);
            }
            else
            {
                assert(elm[i].next == NULL);
            }
        }
        assert(list.head == &elm[0]);
        assert(list.tail == &elm[ELEMENTS - 1]);

        arrBase = elm;

        /* Sort list. */
        DLT_sort(&list);
        printf("Beyond sort.\n");

        {
            Element *pElem = list.head;

            for (i = 0; i < ELEMENTS; i++)
            {
                assert(pElem);
                assert(pElem->payload == i);
                if (i > 0)
                {
                    assert(pElem->prev->payload == i - 1);
                }
                if (i < ELEMENTS - 1)
                {
                    assert(pElem->next->payload == i + 1);
                }
                pElem = pElem->next;
            }
        }


        /* Debug print*/
        {
            Element *pElem = list.head;

            while (pElem)
            {
                printf("pElem: %d\n", pElem->payload);
                pElem = pElem->next;
            }
        }



    }
    #undef ELEMENTS



    return 0;
}

#endif