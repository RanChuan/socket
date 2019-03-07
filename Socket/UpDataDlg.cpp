// UpDataDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Socket.h"
#include "hex2bin.h"
#include "mySocket.h"
#include "WK_Thread.h"
#include "UpDataDlg.h"
#include "afxdialogex.h"


// UpDataDlg �Ի���

IMPLEMENT_DYNAMIC(UpDataDlg, CDialogEx)

UpDataDlg::UpDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UPDATE, pParent)
{

}

UpDataDlg::~UpDataDlg()
{
}

void UpDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, CmdNote);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, File);
	DDX_Control(pDX, IDC_PROGRESS1, upDataIng);
}


BEGIN_MESSAGE_MAP(UpDataDlg, CDialogEx)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &UpDataDlg::OnEnChangeMfceditbrowse1)
	ON_BN_CLICKED(IDC_BUTTON1, &UpDataDlg::OnBnClickedStartUpdata)
	ON_MESSAGE(WK_ERR_RETURN, &UpDataDlg::OnWkErrReturn)
END_MESSAGE_MAP()


// UpDataDlg ��Ϣ������� 


void UpDataDlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString buf;
	File.GetWindowText(buf);

	if (buf.Right(4) == _T(".bin"))
	{
		CmdNote.SetWindowText(_T("ѡ����һ��BIN�����ļ�"));
	}
	else if (buf.Right(4) == _T(".hex"))
	{
		CmdNote.SetWindowText(_T("ѡ����һ��HEX�����ļ�"));
	}
	else
	{
		CmdNote.SetWindowText(_T("��ѡ�ļ����ǳ����ļ���������ѡ��"));
	}

}

	CString FileName;
	DWORD dwThreadID;

void UpDataDlg::OnBnClickedStartUpdata()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString buf;
	GetDlgItemText(IDC_BUTTON1, buf);
	if (buf == _T("��ʼ����"))
	{
		UpdataStruct.s.Init();

		UpdataStruct.s.OpenUdp(UpdataStruct.srcip.ip, UpdataStruct.srcip.port);

		File.GetWindowText(FileName);
		CreateThread(NULL, 0, ThreadUpdataApp,
			(LPVOID)&FileName,//ר��socket��ָ��
			0, &dwThreadID);
		SetDlgItemText(IDC_BUTTON1, _T("ֹͣ����"));

	}
	else if (buf==_T("ֹͣ����"))
	{
		UpdataStruct.s.End();
		SetDlgItemText(IDC_BUTTON1, _T("��ʼ����"));
	}


}



//�����̴߳��صĽ���,��һ����������ʾ�ַ������ڶ��������ǽ��������Ȱٷֱ�
afx_msg LRESULT UpDataDlg::OnWkErrReturn(WPARAM wParam, LPARAM lParam)
{
	int down, up;
	CString *txtbuf;
	txtbuf = (CString *)wParam;
	upDataIng.GetRange(down, up);
	upDataIng.SetStep((down + up) / 100);
	upDataIng.SetPos((int)lParam);
	CmdNote.SetWindowText(*txtbuf);
	if ((*txtbuf).Left(4) == _T("���سɹ�"))
	{
		UpdataStruct.s.End();
		SetDlgItemText(IDC_BUTTON1, _T("��ʼ����"));
	}
	return 0;
}
