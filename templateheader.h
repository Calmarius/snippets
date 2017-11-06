#define CCAT2(x, y) x ## y
#define CCAT(x, y) CCAT2(x, y)

#define FN(x) CCAT(PREFIX, x)

#ifndef PREFIX
    #define PREFIX
#endif

#ifndef SPECIFIER
    #define SPECIFIER
#endif

