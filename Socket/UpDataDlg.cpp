// UpDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Socket.h"
#include "hex2bin.h"
#include "mySocket.h"
#include "WK_Thread.h"
#include "UpDataDlg.h"
#include "afxdialogex.h"


// UpDataDlg 对话框

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


// UpDataDlg 消息处理程序 


void UpDataDlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString buf;
	File.GetWindowText(buf);

	if (buf.Right(4) == _T(".bin"))
	{
		CmdNote.SetWindowText(_T("选择了一个BIN程序文件"));
	}
	else if (buf.Right(4) == _T(".hex"))
	{
		CmdNote.SetWindowText(_T("选择了一个HEX程序文件"));
	}
	else
	{
		CmdNote.SetWindowText(_T("所选文件不是程序文件，请重新选择"));
	}

}

	CString FileName;
	DWORD dwThreadID;

void UpDataDlg::OnBnClickedStartUpdata()
{
	// TODO: 在此添加控件通知处理程序代码
	CString buf;
	GetDlgItemText(IDC_BUTTON1, buf);
	if (buf == _T("开始下载"))
	{
		UpdataStruct.s.Init();

		UpdataStruct.s.OpenUdp(UpdataStruct.srcip.ip, UpdataStruct.srcip.port);

		File.GetWindowText(FileName);
		CreateThread(NULL, 0, ThreadUpdataApp,
			(LPVOID)&FileName,//专递socket的指针
			0, &dwThreadID);
		SetDlgItemText(IDC_BUTTON1, _T("停止下载"));

	}
	else if (buf==_T("停止下载"))
	{
		UpdataStruct.s.End();
		SetDlgItemText(IDC_BUTTON1, _T("开始下载"));
	}


}



//下载线程传回的进度,第一个参数是提示字符串，第二个参数是进度条进度百分比
afx_msg LRESULT UpDataDlg::OnWkErrReturn(WPARAM wParam, LPARAM lParam)
{
	int down, up;
	CString *txtbuf;
	txtbuf = (CString *)wParam;
	upDataIng.GetRange(down, up);
	upDataIng.SetStep((down + up) / 100);
	upDataIng.SetPos((int)lParam);
	CmdNote.SetWindowText(*txtbuf);
	if ((*txtbuf).Left(4) == _T("下载成功"))
	{
		UpdataStruct.s.End();
		SetDlgItemText(IDC_BUTTON1, _T("开始下载"));
	}
	return 0;
}
