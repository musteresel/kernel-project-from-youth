// Kernel - Process communication

#ifndef __KERNEL__KPC___HEADER____
#define __KERNEL__KPC___HEADER____

#include "types.h"
enum kpcMSG_TYPE {
kpcMSG_PAGEFAULT = 1,
kpcMSG_MEMREQUEST,
kpcMSG_MEMFREE,
kpcMSG_IOREQUEST };

typedef struct {
	enum kpcMSG_TYPE type;
	UINT args[5];
}kpcMessage;



#endif //(__KERNEL__KPC___HEADER____)

