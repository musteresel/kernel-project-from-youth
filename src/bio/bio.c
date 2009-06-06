/////////////////////////////////////////////////////////////////////////
/* bio.c
Basic input an output operations.
Basic memory operations.

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////






/** function to set a part of the memory to a spezific value (32bit)**/
void memset32 (unsigned int* ad, unsigned int val, unsigned int count)
{
	unsigned int *tmp;
	tmp = ad;
	for (; count !=0; count--) {
		*tmp++ = val;
	}
}

/** function to set a part of the memory to a spezific value (8bit)**/
void memset8 (unsigned int* ad, unsigned char val, unsigned int count)
{
	unsigned char *tmp;
	tmp = (unsigned char*)ad; //TODO correct adress translation?
	for (; count !=0; count--) {
		*tmp++ = val;
	}
}

/** function to copy a part of the memory to another part (32bit)**/
void memcpy32 (unsigned int* target, unsigned int* source, unsigned int count)
{
	unsigned int *tmp;
	unsigned int *tmp2;
	tmp = target;
	tmp2 = source;
	for (; count !=0; count--) {
		*tmp = *tmp2;
		tmp++;
		tmp2++;
	}
}

/** function to copy a part of the memory to another part (8bit)**/
void memcpy8 (unsigned int* target, unsigned int* source, unsigned int count)
{
	unsigned char *tmp;
	unsigned char *tmp2;
	tmp = (unsigned char*)target;
	tmp2 = (unsigned char*)source;
	for (; count !=0; count--) {
		*tmp = *tmp2;
		tmp++;
		tmp2++;
	}
}




/** function to write on a system port **/
void outportb (unsigned short _port, unsigned char _data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

/** function to read from a system port **/
unsigned char inportb (unsigned short _port)
{
	unsigned char ret;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (ret) : "dN" (_port));
	return ret;
}

