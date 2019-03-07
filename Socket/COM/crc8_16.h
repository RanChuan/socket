#ifndef _crc16_H
#define _crc16_H
#include "afxwin.h"


#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int

void Get_Crc16(u8 *puchMsg,u16 usDataLen,u8 result[]);
u8 Get_Crc8(u8 *ptr,u16 len);
#endif
