#define CCAT2(x, y) x ## y
#define CCAT(x, y) CCAT2(x, y)

#ifndef PREFIX
    #define PREFIX
#endif

#if (3-PREFIX-3 == 6)
    #define FN(x) x
#else
    #define FN(x) CCAT(PREFIX, x)
#endif

#ifndef SPECIFIER
    #define SPECIFIER
#endif

#ifndef STATIC_ASSERT
    #define STATIC_ASSERT(pred, id) typedef struct {int CCAT(SA_field_,id) : !!(pred)} CCAT(SA_,id);
#endif

