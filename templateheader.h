#define CCAT2(x, y) x ## y
#define CCAT(x, y) CCAT2(x, y)

#define FN(x) CCAT(PREFIX, x)

#ifndef PREFIX
    #define PREFIX
#endif

#ifndef SPECIFIER
    #define SPECIFIER
#endif

#ifndef STATIC_ASSERT
    #define STATIC_ASSERT(pred, id) typedef struct {int CCAT(SA_field_,id) : !!(pred)} CCAT(SA_,id);
#endif

