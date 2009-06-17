/////////////////////////////////////////////////////////////////////////
/* bio.c
Basic input an output operations.
Basic memory operations.

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////



#include "types.h"


/** function to set a part of the memory to a spezific value (32bit)**/
void memset32 (UINT* ad, UINT val, UINT count)
{
	UINT *tmp;
	tmp = ad;
	for (; count !=0; count--) {
		*tmp++ = val;
	}
}

/** function to set a part of the memory to a spezific value (8bit)**/
void memset8 (UINT* ad, UINT8 val, UINT count)
{
	UINT8 *tmp;
	tmp = (UINT8*)ad; //TODO correct adress translation?
	for (; count !=0; count--) {
		*tmp++ = val;
	}
}

/** function to copy a part of the memory to another part (32bit)**/
void memcpy32 (UINT* target, UINT* source, UINT count)
{
	UINT *tmp;
	UINT *tmp2;
	tmp = target;
	tmp2 = source;
	for (; count !=0; count--) {
		*tmp = *tmp2;
		tmp++;
		tmp2++;
	}
}

/** function to copy a part of the memory to another part (8bit)**/
void memcpy8 (UINT* target, UINT* source, UINT count)
{
	UINT8 *tmp;
	UINT8 *tmp2;
	tmp = (UINT8*)target;
	tmp2 = (UINT8*)source;
	for (; count !=0; count--) {
		*tmp = *tmp2;
		tmp++;
		tmp2++;
	}
}




/** function to write on a system port **/
void outportb (UINT16 _port, UINT8 _data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

/** function to read from a system port **/
UINT8 inportb (UINT16 _port)
{
	UINT8 ret;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (ret) : "dN" (_port));
	return ret;
}

