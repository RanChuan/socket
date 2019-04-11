#include "stdafx.h"
#include "mySocket.h"
#include "hex2bin.h"
#include "COM\My_Config.h"
#include "WK_Thread.h"



DWORD WINAPI WK_ReciveThread(LPVOID param);

//�����¿����罻�����̣߳������Ǵ����̴߳��ڵ�ָ��
DWORD WINAPI WK_Thread(LPVOID param)
{
	MySocket s;
	DWORD dwThreadID=0;
	MSG msg;
	ipstruct *ip;
	datastruct *data;
	CDialogEx *win;
	HANDLE handleFirst = 0;
	win = (CDialogEx *)param;
	s.Init();
	win->PostMessage(WK_ERR_RETURN, 0, s.geterr());
	while (GetMessage(&msg, 0, 0, 0))
	{
		switch (msg.message)
		{
		case WK_OPEN_UDP:
			ip = (ipstruct *)msg.lParam;
			s.CloseSocket();
			s.OpenUdp(ip->ip, ip->port);
			if (dwThreadID != 0)
			{
				PostThreadMessage(dwThreadID, WK_EXIT_TH, 0, 0);
			}
			{
				handleFirst = CreateThread(NULL, 0, WK_ReciveThread,
				(LPVOID)&s,//ר��socket��ָ��
				0, &dwThreadID);
			}

			break;
		case WK_CLOSE_SOCKET:
			s.CloseSocket();
			break;
		case WK_SEND_DATA:
			ip = (ipstruct *)msg.wParam;
			data = (datastruct *)msg.lParam;
			s.SendData(data->dataptr, data->datalen, ip->ip, ip->port);
			break;
		default:
			break;
		}
		win->PostMessage(WK_ERR_RETURN,0, s.geterr());

	}
	s.End();
	return 0;
}






//�����¿����罻�����̣߳������Ǵ����̴߳��ڵ�ָ��
DWORD WINAPI WK_ReciveThread(LPVOID param)
{
	ipstruct srcip = { 0 };
	datastruct data = { 0 };
	MySocket *s = 0;
	MSG msg;
	int lear=0;
	bool run = true;
	char recvdata[20480];
	s = (MySocket *)param;
	while (run)
	{
		lear = 0;
		s->ReadData(recvdata, 20480, srcip, lear);
		if (lear)
		{
			data.dataptr = recvdata;
			data.datalen = lear;
			AfxGetApp()->m_pMainWnd->SendMessage(WK_RECEIVED,(WPARAM) &data,(LPARAM)&srcip);
		}
		if (PeekMessage(&msg, 0, 0, 0, 1))//����֮��ɾ����Ϣ
		{
			switch (msg.message)
			{
			case WK_EXIT_TH:
				run = false;
				break;
			default:
				break;
			}
		}
	}
	return 0;
}











/*

//����bin�ļ��������ļ���
int download_bin(CString buf, DWORD dwThreadID)
{
	HANDLE hfile;
	DWORD filesize;//�ļ��ܳ���
	int ret = 0;
	unsigned char data[100] = { 0 };

	hfile = CreateFile(buf, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hfile == (HANDLE)-1)
	{
		return 1;
	}
	filesize = GetFileSize(hfile, NULL);
	DWORD  real_read;//�ļ���ȡʵ�ʳ��ȱ���
	ret = ReadFile(hfile, data, 4, &real_read, NULL);
	DWORD truck = ((data[0]) | (data[1] << 8) | (data[2] << 16) | (data[3] << 24));
	if ((truck & 0x2FFE0000) != 0x20000000)
	{
		CloseHandle(hfile);
		//�ļ����Ϸ�
		return 1;
	}
	ret = ReadFile(hfile, data + 4, 40, &real_read, NULL);
	int iap_i = 0;//�������ִ�������
	CString iap_txt;
	while (1)
	{
		iap_txt.Format(_T("�������� %d/%d"), iap_i, 20);
		mywind->SetDlgItemText(IDC_BUTTON11, iap_txt);
		ret = cfg.my_enteriap(filesize);//������Ҫд������ݳ���
		if ((ret <50) && (ret >= 0))break;
		if (ret == -1)
		{
			iap_i++;
			if (iap_i > 20) break;
		}
		else
		{
			break;
		}
	}
	int num_all = 0;
	int num_now = 1;
	num_all = filesize / 44;
	if (filesize % 44) num_all++;
	real_read = 44;
	int iaping = 0;
	while (ret == 0)
	{
		iap_txt.Format(_T("%d/%d"), num_now, num_all);
		mywind->SetDlgItemText(IDC_BUTTON11, iap_txt);
		ret = cfg.my_updataing(real_read, num_now, num_all, data);
		if (ret == -1)
		{
			iaping++;
			if (iaping > 20)//�ظ����
			{
				break;
			}
			ret = 0;
		}
		else if ((ret <50) && (ret > 0))//
		{
			break;
		}
		else
		{
			iaping = 0;//��λ���Դ���
			if (num_all == num_now) break;
			num_now++;
			for (int a = 0; a < 100; a++) { data[a] = 0; }
			ReadFile(hfile, data, 44, &real_read, NULL);
		}

	}
	if (ret != 0)
	{
		buf = _T("���س���ʧ�ܣ�\r\n��������Ϊ��") + cfg.my_geterrtype(ret);
		mywind->MessageBox(buf, _T("����"));
	}
	else
	{
		buf = _T("���س�����ɣ�");
		mywind->MessageBox(buf, _T("��ʾ"));
	}
	CloseHandle(hfile);
	iap_txt = _T("���س���");
	mywind->SetDlgItemText(IDC_BUTTON11, iap_txt);
	return  0;

}


*/

//����ֵ��ʵ�ʶ�ȡ���ֽ���
DWORD GetData(unsigned char *datain, unsigned char *dataout, DWORD getlength, DWORD datain_length)
{
	static DWORD i = 0;
	DWORD a = 0;

	if (datain_length == 0) {
		i = 0;//�޸���һ�γ��������Ժ�ÿ�ζ�ʧ�ܵ�bug
		return 0;
	}
	for (a = 0; a< getlength; a++)
	{
		if (a + i == datain_length) break;
		dataout[a] = datain[a + i];
	}
	i += a;
	return a;
}






wk_threadstruct UpdataStruct;


//����hex�ļ��������ļ���
int download_hex(CString buf)
{
	Hex2Bin hex2bin;
	my_config cfg;
	CString rettxt;
	HANDLE hfile;
	DWORD filesize;//�ļ��ܳ���
	int ret = 0;
	int error = 0;//��¼�Ƿ����
	hfile = CreateFile(buf, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hfile == (HANDLE)-1)
	{
		rettxt = _T("���ļ�ʧ�ܣ�") + buf;
		if (UpdataStruct.m_MainDialog)
			UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
		error = 1;
		return 1;
	}

	filesize = GetFileSize(hfile, NULL);

	unsigned char *hexdata = new unsigned char[filesize];
	unsigned char *bindata = new unsigned char[filesize];
	DWORD hexaddr;
	DWORD real_read;
	ret = ReadFile(hfile, hexdata, filesize, &real_read, NULL);
	CloseHandle(hfile);
	CString hexdatastring; 
	hexdatastring=(char *)hexdata;
	delete[] hexdata;
	ret = hex2bin.hex2bin(bindata, hexdatastring, &hexaddr, &filesize);
	//hfile = CreateFile(_T("F:\\��~\\GitHub\\WK_JZQ_V2.6\\byMyApp.bin"), GENERIC_ALL, NULL, NULL, CREATE_NEW, NULL, NULL);
	//ret=WriteFile(hfile, bindata, filesize, &real_read, NULL);
	//CloseHandle(hfile);

	if (ret != 0)
	{
		rettxt = _T("hexתbinʧ�ܣ�");
		if (UpdataStruct.m_MainDialog)
			UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
		delete[] bindata;
		error = 2;
		return 2;
	}
	if (hexaddr < 0x08002800)//�����ַ����
	{
		rettxt = _T("�����ַ���ԣ�");
		if (UpdataStruct.m_MainDialog)
			UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
		delete[] bindata;
		error = 3;
		return 3;
	}

	DWORD truck = ((bindata[0]) | (bindata[1] << 8) | (bindata[2] << 16) | (bindata[3] << 24));
	if ((truck & 0x2FFE0000) != 0x20000000) //�ļ����Ϸ�
	{
		rettxt = _T("���ǿ����еĳ����ļ���");
		if (UpdataStruct.m_MainDialog)
			UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
		delete[] bindata;
		error = 4;
		return 4;
	}



	UpdataStruct.s.SetTimeOut(10000);//�������Ľ��ճ�ʱΪ1��

	ipstruct nocareip;//���淵�ص�ip��ַ
	char iapbuff[2048] = { 0 };//���淵�ص�����
	int iapbacklen;//ʵ�ʽ��ܵ������ݳ���
	int iaptry;//���Դ���
	iaptry = 20;
	//do
	//{
	//	char port13[] = "set dbgport 13";
	//	UpdataStruct.s.SendData((char *)port13, strlen(port13), UpdataStruct.decip.ip, UpdataStruct.decip.port);
	//	UpdataStruct.s.ReadData(iapbuff, 100, nocareip, iapbacklen);
	//	iaptry--;
	//	if (iaptry == 0) { ret = 5; break; }
	//} while (nocareip.port!=13);

	if (ret)
	{
		if (error == 0)
		{
			rettxt = _T("�˿�����ʧ��") ;
			if (UpdataStruct.m_MainDialog)
				UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
			error = 5;
		}
	}
	else
	{
		//UpdataStruct.decip.port = 13;
	}


	char data[2048];//һ�����ݰ�������
	int packlen;//һ�������ݵĳ���
	packlen = cfg.my_enteriap((char *)data, filesize);//������Ҫд������ݳ���
	iaptry = 2;
	do
	{
		UpdataStruct.s.SendData((char *)data, packlen,UpdataStruct.decip.ip, UpdataStruct.decip.port);
		iapbuff[0] = 0xff;
		iapbuff[1] = 0;
		UpdataStruct.s.ReadData(iapbuff, 2048, nocareip, iapbacklen);
		iaptry--;
		if (iaptry == 0) break;
	} while ((ret = cfg.my_checkreturn(iapbuff, 2048)) != 0);

	if (ret)
	{
		if (error == 0)
		{
			rettxt = _T("����ʧ�ܣ�������Ϊ��")+ret;
			if (UpdataStruct.m_MainDialog)
				UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
			error = 6;
		}
	}


	//hfile = CreateFile(_T("F:\\��~\\GitHub\\WK_JZQ_V2.6\\byMyApp.bin"), GENERIC_ALL, NULL, NULL, CREATE_NEW, NULL, NULL);
	
	int definepacketlen = UpdataStruct.packetsize;
	GetData(bindata,(unsigned char *) data, definepacketlen, 0);//���þ�̬����
	int num_all = 0;
	int num_now = 0;
	num_all = filesize / definepacketlen;
	if (filesize % definepacketlen) num_all++;
	do
	{
		if (ret == 0)
		{
			rettxt.Format( _T("���ؽ��ȣ�%d/%d") , num_now ,num_all);
			if (UpdataStruct.m_MainDialog)
				UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)num_now * 100 / num_all);
			if (num_all == num_now) break;
			num_now++;
			for (int a = 0; a < 2048; a++) { data[a] = 0; }
			real_read = GetData(bindata, (unsigned char *)data, definepacketlen, filesize);
			//WriteFile(hfile, data, real_read, &real_read, NULL);

		}
		else
		{
			
			break;
		}
		char lastdata[2048] = { 0 };
		for (int i = 0; i < packlen; i++) { lastdata[i] = data[i]; }
		packlen = cfg.my_updataing(real_read, num_now, num_all, (unsigned char *)data);
		iaptry = 2;
		do
		{
			if (ret == 0x000c)
			{
				//UpdataStruct.s.SendData((char *)lastdata, packlen, UpdataStruct.decip.ip, UpdataStruct.decip.port);
				break;
			}
			else if (ret == 0x0006)
			{
				break;
			}
			else
			{
				UpdataStruct.s.SendData((char *)data, packlen, UpdataStruct.decip.ip, UpdataStruct.decip.port);
			}
			iapbuff[1] = 0;
			UpdataStruct.s.ReadData(iapbuff, 2048, nocareip, iapbacklen);
			iaptry--;
			if (iaptry == 0) {
				 break;
			}
		} while ((ret=cfg.my_checkreturn(iapbuff, 2048)) != 0);
	}
	while(1);
	if (ret)
	{
		if (error == 0)
		{
			rettxt.Format(_T("����ʧ�ܣ�%X"), ret);
			if (UpdataStruct.m_MainDialog)
				UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
			error = 7;
		}
	}
	else
	{
		rettxt = _T("���سɹ�");
		if (UpdataStruct.m_MainDialog)
			UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
	}
	//CloseHandle(hfile);
	delete[] bindata;
	return  ret;

}















//���ص�Ƭ��������߳�
DWORD WINAPI ThreadUpdataApp(LPVOID param)
{
	int ret = 3;
	int addr = 0;//�����豸��ַʱ���豸�����
	CString *fileName;
	CString rettxt;
	fileName = (CString *)param;

	if (fileName->Right(4) == _T(".bin"))
	{
		//ret = download_bin(buf, mywind);
		rettxt = _T("�ݲ�֧��BIN��ʽ�ĳ�������");
		UpdataStruct.m_MainDialog->PostMessage(WK_ERR_RETURN, (WPARAM)&rettxt, 0);
	}
	else if (fileName->Right(4) == _T(".hex"))
	{
		ret = download_hex(*fileName);
	}
	else
	{
		rettxt = _T("��ѡ��һ�������ļ�");
		UpdataStruct.m_MainDialog->PostMessage(WK_ERR_RETURN, (WPARAM)&rettxt, 0);
	}
	return ret;
}








