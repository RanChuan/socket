#pragma once


//�Զ�����Ϣ
#define WK_OPEN_UDP							(WM_USER+1 ) 
#define WK_OPEN_TCPSERVER					(WM_USER+2 ) 
#define WK_OPEN_TCPCLIENT					(WM_USER+3 )
#define WK_SEND_DATA						(WM_USER+4)
#define WK_CLOSE_SOCKET						(WM_USER+5)
#define WK_ERR_RETURN						(WM_USER+6)
#define WK_RECEIVED							(WM_USER+7)
#define WK_TARGETIP							(WM_USER+8)
#define WK_EXIT_TH							(WM_USER+9)		//�˳��߳�
#define WK_UPDATAINFO						(WM_USER+10)	//�����������

//#include "mySocket.h"

struct wk_threadstruct
{
	CDialogEx *m_MainDialog;
	struct ipstruct srcip;
	struct ipstruct decip;
	MySocket s;
};




extern wk_threadstruct UpdataStruct;



DWORD WINAPI WK_Thread(LPVOID param);

//���ص�Ƭ��������߳�
DWORD WINAPI ThreadUpdataApp(LPVOID param);

