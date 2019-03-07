#pragma once



struct datastruct
{
	char *dataptr;
	int datalen;
};

struct ipstruct
{
	CString ip;
	int port;
};




class MySocket
{
public:
	MySocket(void);
	int Init(void);
	int End(void);
	int GetMyIPs(CString ips[10]);
	int OpenUdp(CString myip, int myport);
	int CloseSocket(void);
	int SendData(char * data, int datalen,CString desip,int desport);
	int ReadData(char * buff, int bufflen, ipstruct &srcip,int &reallen);
	int SetTimeOut(int time);
	ULONG IpstrToUlong(CString buf);
	CString IpulongToStr(ULONG ip);
	int geterr(void);
	int getsocket(void) { return mySocket; };
private:
	WSADATA myWsaData;
	SOCKET mySocket = { 0 };
	int err = ~0;
	CString myMode;
};





