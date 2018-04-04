#ifdef	GLOBAL_VARIABLES_HERE
#undef	EXTERN
#define	EXTERN
#endif

#include "../include/type.h"
#define MAX_SYSTEM_CALL 256

EXTERN u16 system_call[MAX_SYSTEM_CALL];
