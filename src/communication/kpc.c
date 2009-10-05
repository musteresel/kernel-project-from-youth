#include "kpc.h"


void kpc_SendMessage(kpcMessage *msg)
{
	/* TODO */
	/** msg identifies the Type of Message and delivers message arguments **/
	/** the message gets written into the kpc ring buffer and will be processed by a kernelthread **/
	
	/** if the ringbuffer gets to full, the kernelthread(s) have to be called directly (some kind of emergency) **/
}
