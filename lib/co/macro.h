#ifdef __GNUC__
#define __UNUSED __attribute__((unused))
#else
#define __UNUSED
#endif

#define	USED(x)		if(x);else{}
