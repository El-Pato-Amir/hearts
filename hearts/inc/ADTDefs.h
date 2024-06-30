#ifndef __ADTDEFS_H__
#define __ADTDEFS_H__
#include <stdio.h>
#include <stdlib.h>
typedef enum
{

	ERR_OK =0,
	ERR_POINTER_NULL,
	ERR_GENERAL,
	ERR_NOT_INITIALIZED,
	ERR_ALLOCATION_FAILED,
	ERR_REALLOCATION_FAILED,
	ERR_UNDERFLOW,
	ERR_OVERFLOW,
	ERR_WRONG_INDEX,
	ERR_NO_ITEMS,

/* Vector Errors */

/* Stack Errors */

ERR_STACK_BBBBB = 30,

/* LinkedList Errors */
	ERR_DUPLICATE
/* Bit Vector*/

} ADTErr;

void HandleErr(ADTErr errNum, char *msg);
#endif /*__ADTDEFS_H__*/
