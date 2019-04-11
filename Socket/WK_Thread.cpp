#include "stdafx.h"
#include "mySocket.h"
#include "hex2bin.h"
#include "COM\My_Config.h"
#include "WK_Thread.h"



DWORD WINAPI WK_ReciveThread(LPVOID param);

//用于温控网络交互的线程，参数是创建线程窗口的指针
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
				(LPVOID)&s,//专递socket的指针
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






//用于温控网络交互的线程，参数是创建线程窗口的指针
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
		if (PeekMessage(&msg, 0, 0, 0, 1))//处理之后删除消息
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

//下载bin文件，输入文件名
int download_bin(CString buf, DWORD dwThreadID)
{
	HANDLE hfile;
	DWORD filesize;//文件总长度
	int ret = 0;
	unsigned char data[100] = { 0 };

	hfile = CreateFile(buf, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hfile == (HANDLE)-1)
	{
		return 1;
	}
	filesize = GetFileSize(hfile, NULL);
	DWORD  real_read;//文件读取实际长度变量
	ret = ReadFile(hfile, data, 4, &real_read, NULL);
	DWORD truck = ((data[0]) | (data[1] << 8) | (data[2] << 16) | (data[3] << 24));
	if ((truck & 0x2FFE0000) != 0x20000000)
	{
		CloseHandle(hfile);
		//文件不合法
		return 1;
	}
	ret = ReadFile(hfile, data + 4, 40, &real_read, NULL);
	int iap_i = 0;//尝试握手次数变量
	CString iap_txt;
	while (1)
	{
		iap_txt.Format(_T("尝试握手 %d/%d"), iap_i, 20);
		mywind->SetDlgItemText(IDC_BUTTON11, iap_txt);
		ret = cfg.my_enteriap(filesize);//参数是要写入的数据长度
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
			if (iaping > 20)//重复五次
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
			iaping = 0;//复位重试次数
			if (num_all == num_now) break;
			num_now++;
			for (int a = 0; a < 100; a++) { data[a] = 0; }
			ReadFile(hfile, data, 44, &real_read, NULL);
		}

	}
	if (ret != 0)
	{
		buf = _T("下载程序失败！\r\n错误类型为：") + cfg.my_geterrtype(ret);
		mywind->MessageBox(buf, _T("错误"));
	}
	else
	{
		buf = _T("下载程序完成！");
		mywind->MessageBox(buf, _T("提示"));
	}
	CloseHandle(hfile);
	iap_txt = _T("下载程序");
	mywind->SetDlgItemText(IDC_BUTTON11, iap_txt);
	return  0;

}


*/

//返回值是实际读取的字节数
DWORD GetData(unsigned char *datain, unsigned char *dataout, DWORD getlength, DWORD datain_length)
{
	static DWORD i = 0;
	DWORD a = 0;

	if (datain_length == 0) {
		i = 0;//修复有一次程序下载以后每次都失败的bug
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


//下载hex文件，输入文件名
int download_hex(CString buf)
{
	Hex2Bin hex2bin;
	my_config cfg;
	CString rettxt;
	HANDLE hfile;
	DWORD filesize;//文件总长度
	int ret = 0;
	int error = 0;//记录是否出错
	hfile = CreateFile(buf, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hfile == (HANDLE)-1)
	{
		rettxt = _T("打开文件失败：") + buf;
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
	//hfile = CreateFile(_T("F:\\川~\\GitHub\\WK_JZQ_V2.6\\byMyApp.bin"), GENERIC_ALL, NULL, NULL, CREATE_NEW, NULL, NULL);
	//ret=WriteFile(hfile, bindata, filesize, &real_read, NULL);
	//CloseHandle(hfile);

	if (ret != 0)
	{
		rettxt = _T("hex转bin失败！");
		if (UpdataStruct.m_MainDialog)
			UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
		delete[] bindata;
		error = 2;
		return 2;
	}
	if (hexaddr < 0x08002800)//程序地址不对
	{
		rettxt = _T("程序地址不对！");
		if (UpdataStruct.m_MainDialog)
			UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
		delete[] bindata;
		error = 3;
		return 3;
	}

	DWORD truck = ((bindata[0]) | (bindata[1] << 8) | (bindata[2] << 16) | (bindata[3] << 24));
	if ((truck & 0x2FFE0000) != 0x20000000) //文件不合法
	{
		rettxt = _T("不是可运行的程序文件！");
		if (UpdataStruct.m_MainDialog)
			UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
		delete[] bindata;
		error = 4;
		return 4;
	}



	UpdataStruct.s.SetTimeOut(10000);//设置最大的接收超时为1秒

	ipstruct nocareip;//保存返回的ip地址
	char iapbuff[2048] = { 0 };//保存返回的数据
	int iapbacklen;//实际接受到的数据长度
	int iaptry;//重试次数
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
			rettxt = _T("端口设置失败") ;
			if (UpdataStruct.m_MainDialog)
				UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
			error = 5;
		}
	}
	else
	{
		//UpdataStruct.decip.port = 13;
	}


	char data[2048];//一个数据包的数据
	int packlen;//一个包数据的长度
	packlen = cfg.my_enteriap((char *)data, filesize);//参数是要写入的数据长度
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
			rettxt = _T("握手失败，错误码为：")+ret;
			if (UpdataStruct.m_MainDialog)
				UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
			error = 6;
		}
	}


	//hfile = CreateFile(_T("F:\\川~\\GitHub\\WK_JZQ_V2.6\\byMyApp.bin"), GENERIC_ALL, NULL, NULL, CREATE_NEW, NULL, NULL);
	
	int definepacketlen = UpdataStruct.packetsize;
	GetData(bindata,(unsigned char *) data, definepacketlen, 0);//重置静态变量
	int num_all = 0;
	int num_now = 0;
	num_all = filesize / definepacketlen;
	if (filesize % definepacketlen) num_all++;
	do
	{
		if (ret == 0)
		{
			rettxt.Format( _T("下载进度：%d/%d") , num_now ,num_all);
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
			rettxt.Format(_T("下载失败：%X"), ret);
			if (UpdataStruct.m_MainDialog)
				UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
			error = 7;
		}
	}
	else
	{
		rettxt = _T("下载成功");
		if (UpdataStruct.m_MainDialog)
			UpdataStruct.m_MainDialog->SendMessage(WK_ERR_RETURN, (WPARAM)&rettxt, (LPARAM)0);
	}
	//CloseHandle(hfile);
	delete[] bindata;
	return  ret;

}















//下载单片机程序的线程
DWORD WINAPI ThreadUpdataApp(LPVOID param)
{
	int ret = 3;
	int addr = 0;//配置设备地址时，设备不理会
	CString *fileName;
	CString rettxt;
	fileName = (CString *)param;

	if (fileName->Right(4) == _T(".bin"))
	{
		//ret = download_bin(buf, mywind);
		rettxt = _T("暂不支持BIN格式的程序下载");
		UpdataStruct.m_MainDialog->PostMessage(WK_ERR_RETURN, (WPARAM)&rettxt, 0);
	}
	else if (fileName->Right(4) == _T(".hex"))
	{
		ret = download_hex(*fileName);
	}
	else
	{
		rettxt = _T("请选择一个程序文件");
		UpdataStruct.m_MainDialog->PostMessage(WK_ERR_RETURN, (WPARAM)&rettxt, 0);
	}
	return ret;
}








