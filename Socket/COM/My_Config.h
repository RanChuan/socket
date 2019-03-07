#pragma once
#include "afxwin.h"
#include "ThisComm.h"



struct CenterIP
{
	unsigned char my_ip[4];
	unsigned char fw_ip[4];
	unsigned char wg_ip[4];
	unsigned short my_com;
	unsigned short fw_com;
};


int str_to_int(CString buf, int *ints);




class my_config
{
public:
	char my_txt;
	int my_entercfg(char * buff,int befflen,unsigned short);
	int my_esccfg(char * buff, int befflen, unsigned short);
	int my_cfgaddr(char * buff, int befflen, unsigned short,unsigned short, unsigned char, unsigned char);
	int my_cfgstudy(unsigned short,  unsigned char);
	int my_cfgcenter(unsigned short, unsigned short *, unsigned short,unsigned char *);
	int my_cfgcenterip(unsigned short, CenterIP);
	int my_complitetime(unsigned char *);
	int my_enteriap(char *data, unsigned int);
	int my_updataing(unsigned short, unsigned short, unsigned short, unsigned char *);
	int my_reset(unsigned short);
	int my_checkreturn(char *data, int datalen);
	CString my_geterrtype(short);
};






