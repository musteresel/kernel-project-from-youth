/////////////////////////////////////////////////////////////////////////
/* debug-text.c
Contains functions to print out kernel debug messages.

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////


#include "bio.h"
#include "types.h"



UINT16 *video = (UINT16 *)0xB8000;
INT8 attribute = 0x0F;
INT8 cur_x = 0;
INT8 cur_y = 0;



static void scroll (void)
{
	UINT temp;
	if (cur_y >= 25)
	{
		temp = cur_y - 24;
		memcpy32( (UINT*)video, (UINT*)( video + temp * 80),(25 - temp) * 40);
		memset32 ((UINT*)(video + (25 - temp) * 80), (0x20 | (attribute << 8)), 40);
		cur_y = 24;
	}
}


static void move_hw_cur (void)
{
	UINT16 temp;
	temp = cur_y*80 + cur_x;
	outportb(0x3D4, 14);
	outportb(0x3D5, (UINT8)(temp>>8));
	outportb(0x3D4, 15);
	outportb(0x3D5, (UINT8)temp);
}

void putc (UINT8 c)
{
	UINT16 *place;
	UINT16 att = attribute<<8;
	if (c == 0x08) /* backspace */
	{
		if (cur_x != 0)
		{
			cur_x--;
		}
		place = video + (cur_y*80 + cur_x);
		*place = ' ' | att;
	} else if (c == 0x09) /* tab */
	{
		cur_x = (cur_x + 8) & ~(7);
	} else if (c == '\r') /* carriage return (CR) */
	{
		cur_x = 0;
	} else if (c == '\n') /* new line (with CR) */
	{
		cur_x = 0;
		cur_y++;
	} else if (c >= ' ') /* printable */
	{
		place = video + (cur_y*80 + cur_x);
		*place = c | att;
		cur_x++;
	}

	if (cur_x >= 80) /* end of line */
	{
		cur_x = 0;
		cur_y++;
	}
	
	scroll();
	move_hw_cur();
}



void puts (INT8* string)
{
	UINT16 count = 0;
	for (; string[count] != '\0'; count++)
	{
		putc( (UINT8) string[count] );
	}
}


void set_color (UINT8 foregr, UINT8 backgr)
{
	attribute = (backgr<<4) | foregr;
}



void int_to_string (INT8 *buf, UINT8 base, INT d)
{
	INT8 *p = buf;
	INT8 *p1, *p2;
	INT div = 10;
	UINT num = d;
	
	if (base == 'd' && d < 0)
	{
		*p++ = '-';
		buf++;
		num = -d;
	} else if ( base == 'x')
	{
		div = 16;
	} else if (base == 'b')
	{
		div = 2;
	}
	
	do
	{
		INT rem = num % div; //NOTE: not good...
		*p++ = (rem < 10) ? rem + '0' : rem + 'a' - 10;
	}while (num /= div);
	
	*p = 0;
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2)
	{
		INT8 temp = *p1;
		*p1 = *p2;
		*p2 = temp;
		p1++;
		p2--;
	}
}


