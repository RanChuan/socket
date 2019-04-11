#include "stdafx.h"
#include "mySocket.h"









MySocket::MySocket(void)
{
	mySocket = -1;
	err = ~0;
}

int MySocket::Init(void)
{

	WSAStartup(2, &myWsaData);
	err= WSAGetLastError();
	return err;
}

int MySocket::End(void)
{
	WSACleanup();
	err = WSAGetLastError();
	return err;
}

int MySocket::GetMyIPs(CString ips[10])
{
	char zsHost[256] = { 0 };
	::gethostname(zsHost, 256);
	addrinfo ipiof;
	memset(&ipiof, 0, sizeof(struct addrinfo));
	ipiof.ai_family = AF_INET;
	PADDRINFOA ipout = 0;
	err=getaddrinfo(zsHost, NULL, &ipiof, &ipout);
	if (ipout != NULL)
	{
		sockaddr_in *iptemp = 0;
		for (int i = 0; ; i++)
		{
			char ipbuf[16] = { 0 };
			iptemp = (sockaddr_in*)ipout->ai_addr;
			ips[i]= inet_ntop(AF_INET, &iptemp->sin_addr, ipbuf, 16);//将IP地址的二进制转换为十进制
			if (i >= 9) break;
			if (ipout->ai_next != 0)
				ipout = ipout->ai_next;
			else
				break;
		}
	}
	return 0;
}

int MySocket::OpenUdp(CString myip, int myport)
{
	ULONG ipaddr = 0;
	myMode = _T("UDP");
	if (mySocket == -1)
	{
		mySocket = socket(AF_INET, SOCK_DGRAM, 0);
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = htons(myport);
		sin.sin_addr.S_un.S_addr = IpstrToUlong(myip);

		bind(mySocket, (LPSOCKADDR)&sin, sizeof(sin));

	}
	err = WSAGetLastError();
	return err;
}




int MySocket::CloseSocket(void)
{
	closesocket(mySocket);
	err = WSAGetLastError();
	mySocket = -1;
	return 0;
}








//16进制转字符串
static void HexToAscii(unsigned char * pHex, unsigned char * pAscii, int nLen)
{
	unsigned char Nibble[3];
	int maxlength = nLen;
	for (int i = 0; i < maxlength; i++)
	{
		Nibble[0] = (pHex[i] & 0xF0) >> 4;
		Nibble[1] = pHex[i] & 0x0F;
		for (int j = 0; j < 3; j++)
		{
			if (j < 2)
			{
				if (Nibble[j] < 10)
					Nibble[j] += 0x30;
				else
				{
					if (Nibble[j] < 16)
						Nibble[j] = Nibble[j] - 10 + 'A';
				}

			}
			else
			{
				Nibble[j] = ' ';

			}
			*pAscii++ = Nibble[j];
		}   // for (int j = ...)
	}   // for (int i = ...)
	*pAscii++ = 0;//结尾符

}













//发送数据
int MySocket::SendData(char * data, int datalen,CString desip, int desport)
{
	if (myMode = _T("UDP"))
	{
		sockaddr_in sto;
		sto.sin_family = AF_INET;
		sto.sin_port = htons(desport);
		sto.sin_addr.S_un.S_addr = IpstrToUlong(desip);
		bool opt = true;
		setsockopt(mySocket, SOL_SOCKET, SO_BROADCAST, (char FAR *)&opt, sizeof(bool));
		sendto(mySocket, data, datalen, 0, (LPSOCKADDR)&sto, sizeof(sto));
		err = WSAGetLastError();





		//保存发送过的命令到文件
		/*
		CString buf;
		CString file_txt;
		WCHAR file_dir[200];
		HANDLE hfile;
		DWORD filesize = 0;
		GetModuleFileName(NULL, file_dir, 200);
		buf = file_dir;
		while (buf[buf.GetLength() - 1] != _T('\\'))
		{
			buf = buf.Left(buf.GetLength() - 1);
		}
		buf = buf + _T("My_Socket.txt");
		hfile = CreateFile(buf, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hfile == (HANDLE)-1) hfile = CreateFile(buf, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL, NULL);
		if (hfile != (HANDLE)-1)
		{
			filesize = GetFileSize(hfile, NULL);
			char *file_data;// = new char[filesize];
			DWORD real_read = 0;
			CTime tm; tm = CTime::GetCurrentTime();
			file_txt = _T("\r\n");
			buf = tm.Format(_T("Send: %Y,%m,%d---%X\r\n\t"));
			file_txt = file_txt + buf;

			unsigned char string[200] = { 0 };
			HexToAscii((unsigned char *)data, string, datalen);
			buf = string;
			file_txt = file_txt + buf;
			file_data = new char[file_txt.GetLength()];
			for (int i = 0; i < file_txt.GetLength(); i++) { file_data[i] = file_txt[i]; }
			SetFilePointer(hfile, NULL, (PLONG)&real_read, FILE_END);
			WriteFile(hfile, file_data, file_txt.GetLength(), &real_read, NULL);
			CloseHandle(hfile);

		}
		*/





		return err;
	}
	else if (myMode = _T("TCPServer"))
	{

	}
	else if (myMode = _T("TCPClient"))
	{

	}
	return ~0;
}

int MySocket::ReadData(char * buff, int bufflen, ipstruct &srcip, int &reallen)
{
	if (myMode = _T("UDP"))
	{
		sockaddr_in sfrom;
		int sfromsize = sizeof(sockaddr_in);
		reallen = 0;
		reallen=recvfrom(mySocket, buff, bufflen, 0, (LPSOCKADDR)&sfrom, &sfromsize);
		err = WSAGetLastError();
		if (err == 0)
		{
			//reallen = strlen(buff)+1;
			buff[reallen] = 0;
			srcip.ip = IpulongToStr(sfrom.sin_addr.S_un.S_addr);
			srcip.port = ntohs (sfrom.sin_port);






			//保存接收的返回到文件
			/*
			if (reallen)
			{
				CString buf;
				CString file_txt;
				WCHAR file_dir[200];
				HANDLE hfile;
				DWORD filesize = 0;
				GetModuleFileName(NULL, file_dir, 200);
				buf = file_dir;
				while (buf[buf.GetLength() - 1] != _T('\\'))
				{
					buf = buf.Left(buf.GetLength() - 1);
				}
				buf = buf + _T("My_Socket.txt");
				hfile = CreateFile(buf, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hfile == (HANDLE)-1) hfile = CreateFile(buf, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL, NULL);
				if (hfile != (HANDLE)-1)
				{
					filesize = GetFileSize(hfile, NULL);
					char *file_data;// = new char[filesize];
					DWORD real_read = 0;
					CTime tm; tm = CTime::GetCurrentTime();
					file_txt = _T("\r\n");
					buf = tm.Format(_T("Recv: %Y,%m,%d---%X\r\n\t"));
					file_txt = file_txt + buf;

					unsigned char string[200] = { 0 };
					HexToAscii((unsigned char *)buff, string, reallen);
					buf = string;
					file_txt = file_txt + buf;
					file_data = new char[file_txt.GetLength()];
					for (int i = 0; i < file_txt.GetLength(); i++) { file_data[i] = file_txt[i]; }
					SetFilePointer(hfile, NULL, (PLONG)&real_read, FILE_END);
					WriteFile(hfile, file_data, file_txt.GetLength(), &real_read, NULL);
					CloseHandle(hfile);
					delete file_data;

				}
			}
			*/






		}
		else
		{
			reallen = 0;
		}
		return err;
	}
	else if (myMode = _T("TCPServer"))
	{

	}
	else if (myMode = _T("TCPClient"))
	{

	}

	return ~0;
}

int MySocket::SetTimeOut(int time)
{
	int nNetTimeout = time;
	return setsockopt(mySocket,SOL_SOCKET, SO_RCVTIMEO,(char *)&nNetTimeout, sizeof(int));
	
}






//这个函数把用用分号分开的CString转化在数字数组中

ULONG MySocket::IpstrToUlong(CString buf)
{
	int offset = 0;
	CString temp;
	UCHAR ip[4] = { 0 };
	int trant_num = 0;//转换的数字个数
	while (!buf.IsEmpty())//如果字符串不为空
	{
		offset = buf.Find(_T("."), 0);//找到第一个";"出现的位置
		if (offset == -1)//没有找到分号,只有一个数字了
		{
			ip[trant_num] = _ttoi(buf);
			trant_num++;
			break;
		}
		else
		{
			if (offset == 0)//分号出现在第一位
			{
				buf.Delete(0, 1);
				continue;
			}
			temp = buf.Left(offset);//提取最前面的数字
			ip[trant_num] = _ttoi(temp);
			trant_num++;
			buf.Delete(0, offset + 1);


		}
	}
	return (ip[0] << 0) | (ip[1] << 8) | (ip[2] << 16) | (ip[3] << 24);
}

CString MySocket::IpulongToStr(ULONG ip)
{
	CString buf;
	UCHAR u8ip[4] = { 0 };
	u8ip[0] = ip & 0x000000ff;
	u8ip[1] = (ip & 0x0000ff00) >> 8;
	u8ip[2] = (ip & 0x00ff0000) >> 16;
	u8ip[3] = (ip & 0xff000000) >> 24;
	buf.Format(_T("%d.%d.%d.%d"), u8ip[0], u8ip[1], u8ip[2], u8ip[3]);
	return buf;
}

int MySocket::geterr(void)
{
	return err;
}





