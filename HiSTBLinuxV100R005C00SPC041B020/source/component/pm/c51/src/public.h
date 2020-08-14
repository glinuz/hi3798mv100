#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include "REG51.h"

/*version for pilot/mpw*/
#define VERSION_V101
//#define TEST_ON

/* struct */
typedef union 
{
	U32  val32;
	U16  val16[2];
	U8   val8[4];
}DATA_U32_S;


#endif
