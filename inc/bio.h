// Basic input output operations

#ifndef __KERNEL__BIO___HEADER____
#define __KERNEL__BIO___HEADER____



extern void memset32 (unsigned int*, unsigned int, unsigned int);
extern void memset8 (unsigned int*, unsigned char, unsigned int);
extern void memcpy32 (unsigned int*, unsigned int*, unsigned int);
extern void memcpy8 (unsigned int*, unsigned int*, unsigned int);
extern void outportb (unsigned short, unsigned char);
extern unsigned char inportb (unsigned short);



#endif //(__KERNEL__BIO___HEADER____)
