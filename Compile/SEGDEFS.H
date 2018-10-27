#define NOP

/* the usual booleans: true,false, on,off, yes,no*/
#ifndef TRUE
#define TRUE (~0)
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef ON
#define ON (~0)
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef YES
#define YES (~0)
#endif

#ifndef NO
#define NO 0
#endif


/* This macro modded from assert.h... similar but is used for fatal errors
that " should never happen" (ho ho ) */
#ifndef _FATAL_DEFINED

#ifndef NDEBUG


#define fatal(exp) { \
    fprintf(stderr, "Fatal error: %s, file %s, line %d\n", #exp, __FILE__, __LINE__); \
    fflush(stderr); \
    abort(); \
    }

#define where_am_i() fprintf(stderr,"At  file %s, line %d\n",\
  __FILE__, __LINE__)

#else

#define fatal(exp)
#define where_am_i()

#endif /* NDEBUG */

#define _FATAL_DEFINED

#endif /* _ASSERT_DEFINED */

#define new(x)  (x*)malloc(sizeof(x))

#define EVER ;;
