#include "stdafx.h"
#include "ThisComm.h"


ThisComm::ThisComm()
{
}


ThisComm::~ThisComm()
{
}


void ThisComm::OpenComm(LPCWSTR lpcomid)
{
	hcom = CreateFile(lpcomid, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hcom == (HANDLE)-1)
	{
		MessageBox(NULL,_T("��COMʧ�ܣ�"),NULL,0);
		if_open = false;
		return;
	}
	if_open = true;
	GetCommState
	(
		hcom,  // ͨѶ�豸�ľ�� 
		&comstate   // �豸���ƿ飬ָ��һ��DCB�ṹ��ָ��  
	);
	comstate.BaudRate = CBR_115200;
	comstate.ByteSize = 8;
	comstate.StopBits = ONESTOPBIT;
	comstate.Parity = NOPARITY;
	SetCommState
	(
		hcom,  // ͨѶ�豸�ľ�� 
		&comstate   // �豸���ƿ飬ָ��һ��DCB�ṹ��ָ��  
	);

}


void ThisComm::SetComm(LPDCB lpcombcd)
{

}


void ThisComm::CloseComm()
{
	if (if_open) CloseHandle(hcom);
	if_open = 0;
}






//16����ת�ַ���
void HexToAscii(unsigned char * pHex, unsigned char * pAscii, int nLen)
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
	*pAscii++ = 0;//��β��

}


















int ThisComm::SendComm(unsigned char * data, unsigned int length)
{
	DWORD realleng;
	unsigned char buff[100] = { 0 };
	if (if_open)
	{
		PurgeComm(hcom, PURGE_RXCLEAR);//��ս��ջ�����
//		RecvComm(buff, 100);//����֮ǰ��ȡ��ǰ���յ����ݣ���ֹ����
		WriteFile(hcom, data, length, &realleng, NULL);


						//���淢�͹�������ļ�
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
		buf=buf+_T("My_History.txt");
		hfile=CreateFile(buf, GENERIC_READ| GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hfile==(HANDLE)-1) hfile = CreateFile(buf, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL, NULL);
		if (hfile != (HANDLE)-1)
		{
			filesize = GetFileSize(hfile, NULL);
			char *file_data;// = new char[filesize];
			DWORD real_read = 0;
			CTime tm; tm = CTime::GetCurrentTime();
			file_txt =  _T("\r\n");
			buf = tm.Format(_T("Send: %Y,%m,%d---%X\r\n\t"));
			file_txt = file_txt + buf;

			unsigned char string[200] = { 0 };
			HexToAscii(data, string, length);
			buf = string;
			file_txt = file_txt + buf;
			file_data=new char[file_txt.GetLength()];
			for (int i = 0; i < file_txt.GetLength(); i++) { file_data[i] = file_txt[i]; }
			SetFilePointer(hfile, NULL, (PLONG)&real_read, FILE_END);
			WriteFile(hfile, file_data, file_txt.GetLength(), &real_read, NULL);
			CloseHandle(hfile);
			delete file_data;

		}

	}
	else
	{
//		MessageBox(NULL, _T("���һ�����ڣ�"), NULL, 0);
		return 1;
	}
	return 0;
}

int ThisComm::RecvComm(unsigned char * data, unsigned int length)
{
	DWORD len=0;
	COMMTIMEOUTS t;
	t.ReadTotalTimeoutConstant = 5;
	t.ReadTotalTimeoutMultiplier = 0;
	SetCommTimeouts(hcom, &t);
	
	ReadFile(hcom, data, length, &len, 0);


				//������յķ��ص��ļ�
	length = len;
	if (length)
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
		buf = buf + _T("My_History.txt");
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
			HexToAscii(data, string, length);
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



	return len;
}
