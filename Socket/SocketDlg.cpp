
// SocketDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Socket.h"
#include "mySocket.h"
#include "SocketDlg.h"
#include "WK_Thread.h"
#include "UpDataDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSocketDlg �Ի���



CSocketDlg::CSocketDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SOCKET_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, NativeIP);
	DDX_Control(pDX, IDC_EDIT1, CmdNote);
	DDX_Control(pDX, IDC_EDIT2, SendBox);
	DDX_Control(pDX, IDC_EDIT3, RecvBox);
	DDX_Control(pDX, IDC_COMBO2, TargetIP);
	DDX_Control(pDX, IDC_BUTTON4, RecvBoxSlearBut);
	DDX_Control(pDX, IDC_EDIT4, NativPort);
}

BEGIN_MESSAGE_MAP(CSocketDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSocketDlg::OnBnClickedHelp)
	ON_MESSAGE(WK_ERR_RETURN, &CSocketDlg::OnWkErrReturn)
	ON_BN_CLICKED(IDC_BUTTON2, &CSocketDlg::OnBnClickedSend)
	ON_MESSAGE(WK_RECEIVED, &CSocketDlg::OnWkReceived)
	ON_BN_CLICKED(IDC_BUTTON3, &CSocketDlg::OnBnClickedUpData)
	ON_BN_CLICKED(IDC_BUTTON4, &CSocketDlg::OnBnClickedClearRecvBox)
	ON_EN_CHANGE(IDC_EDIT3, &CSocketDlg::OnEnChangeEdit3)
END_MESSAGE_MAP()


// CSocketDlg ��Ϣ�������

BOOL CSocketDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	MySocket sockettmp;
	CString myips[10];
	sockettmp.Init();
	sockettmp.GetMyIPs(myips);
	sockettmp.End();
	for (int i = 0; i < 10; i++)
	{
		if (!myips[i].IsEmpty()) 
			NativeIP.AddString(myips[i]);
	}


	NativeIP.SetCurSel(0);

	// ���Ҽ�������ô��ڲ������ں����õ�Combo Box�ؼ���
	void FindCommPort(CComboBox *pComboBox);
	FindCommPort(&NativeIP);

	for (int i = 0; i < 10; i++)
	{
	handleFirst[i] = CreateThread(NULL, 0, WK_Thread,
		AfxGetApp()->m_pMainWnd,//ר�������ڵ�ָ��
		0, &dwThreadID[i]);
	}



	TargetIP.AddString(_T("255.255.255.255"));
	TargetIP.SetCurSel(0);

	NativPort.SetWindowText(_T("7000"));



	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSocketDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSocketDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		CRect rect;
		GetClientRect(&rect);
		RecvBox.MoveWindow(15, rect.Height() / 2 + 5, rect.Width() - 30, rect.Height() / 2 - 53);
		CmdNote.MoveWindow(15, rect.Height() - 20, rect.Width()/2, 20);
		SendBox.MoveWindow(15, 60, rect.Width() - 30, rect.Height() / 2 - 60 - 5);
		RecvBoxSlearBut.MoveWindow(rect.Width() - 30 - 100, rect.Height() - 35, 100, 25);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSocketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


	WSADATA a;
	SOCKET mysocket;




//�����˰�����ť
void CSocketDlg::OnBnClickedHelp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString buf;
	if (dwThreadID[0] != 0)
	{
		GetDlgItemText(IDC_BUTTON1, buf);
		if (buf == _T("����"))
		{
			NativeIP.GetWindowText(myip.ip);
			if (myip.ip.Left(3) == _T("COM"))
			{
				SetDlgItemText(IDC_BUTTON1, _T("�Ͽ�����"));
			}
			else
			{
				NativPort.GetWindowText(buf);				
				myip.port = _ttoi(buf);
				PostThreadMessage(dwThreadID[0], WK_OPEN_UDP, 0, (LPARAM)&myip);
				SetDlgItemText(IDC_BUTTON1, _T("�Ͽ�����"));
			}
		}
		else if (buf == _T("�Ͽ�����"))
		{
			PostThreadMessage(dwThreadID[0], WK_CLOSE_SOCKET, 0, (LPARAM)&myip);
			SetDlgItemText(IDC_BUTTON1, _T("����"));
		}
	}

}


afx_msg LRESULT CSocketDlg::OnWkErrReturn(WPARAM wParam, LPARAM lParam)
{
	CString buf;
	buf.Format(_T("������룺%d"), lParam);
	CmdNote.SetWindowText(buf);
	return 0;
}


void CSocketDlg::OnBnClickedSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString buf;
	SendBox.GetWindowText(buf);
	for (int i = 0; i < 2048; i++) { sendstring[i] = 0; }
	WideCharToMultiByte(CP_ACP, 0, buf, buf.GetLength(), sendstring, 2048, 0, 0);
	TargetIP.GetWindowText(myip.ip);
	myip.port = 12;
	mydata.datalen = strlen(sendstring);
	mydata.dataptr = sendstring;
	PostThreadMessage(dwThreadID[0], WK_SEND_DATA, (WPARAM)&myip, (LPARAM)&mydata);

}





afx_msg LRESULT CSocketDlg::OnWkReceived(WPARAM wParam, LPARAM lParam)
{
	ipstruct *srcip;
	datastruct *data;
	data = (datastruct *)wParam;
	srcip = (ipstruct *)lParam;
	CString temp;
	CString buf;
	buf=data->dataptr;
	RecvBox.GetWindowText(temp);
	temp = temp + buf;
	RecvBox.SetWindowText(temp);
	if (TargetIP.FindStringExact(0, srcip->ip) == CB_ERR)
	{
		TargetIP.AddString(srcip->ip);
		TargetIP.SelectString(0, srcip->ip);
	}
	
	return 0;
}





void CSocketDlg::OnBnClickedUpData()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpDataDlg dlg;
	UpDataDlg *m_pWnd = &dlg;
	UpdataStruct.m_MainDialog = m_pWnd;
	NativeIP.GetWindowText(UpdataStruct.srcip.ip);
	UpdataStruct.srcip.port = 7010;
	TargetIP.GetWindowText(UpdataStruct.decip.ip);
	UpdataStruct.decip.port = 12;
	if (UpdataStruct.decip.ip != _T("255.255.255.255"))
	{
		INT_PTR nResponse = dlg.DoModal();
	}
	else
	{
		CmdNote.SetWindowText(_T("�豸IP��ַ����Ϊ�㲥��ַ"));
	}
	UpdataStruct.m_MainDialog = 0;
}










// ���Ҽ�������ô��ڲ������ں����õ�Combo Box�ؼ���

void FindCommPort(CComboBox *pComboBox)

{

	HKEY hKey;



#ifdef _DEBUG

	ASSERT(pComboBox != NULL);

	pComboBox->AssertValid();

#endif



	LONG nRetVal = RegOpenKeyEx(HKEY_LOCAL_MACHINE,

		_T("Hardware\\DeviceMap\\SerialComm"), NULL,

		KEY_READ, &hKey);

	if (nRetVal == ERROR_SUCCESS)

	{

		int i = 0;

		TCHAR portName[256], commName[256];

		DWORD dwLong, dwSize;
		while (1)

		{

			dwLong = dwSize = sizeof(portName);

			nRetVal = RegEnumValue(hKey, i, portName, &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize);

			if (nRetVal == ERROR_NO_MORE_ITEMS) // ö�ٴ���

				break;



			CString strCommName;

			strCommName = commName;

	//		strCommName += _T("(����)");

			pComboBox->AddString(strCommName); // commName����������

			i++;

		}


		RegCloseKey(hKey);

	}

}










void CSocketDlg::OnBnClickedClearRecvBox()
{
	// TODO: 
	RecvBox.SetWindowText(_T(""));
}


void CSocketDlg::OnEnChangeEdit3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
