
// SocketDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "mySocket.h"
#include "WK_Thread.h"

// CSocketDlg 对话框
class CSocketDlg : public CDialogEx
{
// 构造
public:
	CSocketDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOCKET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	DWORD dwThreadID[10] = { 0 };
	HANDLE handleFirst[10] = { 0 };
	ipstruct myip;
	datastruct mydata;
	char sendstring[2048];

	afx_msg void OnBnClickedHelp();
	// 本机IP地址选择框
	CComboBox NativeIP;
	// 命令执行结果提示
	CEdit CmdNote;
protected:
	afx_msg LRESULT OnWkErrReturn(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedSend();
	// 发送区，在这里面的数据被发送
	CEdit SendBox;
protected:
	afx_msg LRESULT OnWkReceived(WPARAM wParam, LPARAM lParam);
public:
	// 显示接收到的数据
	CEdit RecvBox;
	afx_msg void OnBnClickedUpData();
	// 目标机的ip地址
	CComboBox TargetIP;
	afx_msg void OnBnClickedClearRecvBox();
	// 接收区清空按钮
	CButton RecvBoxSlearBut;
	afx_msg void OnEnChangeEdit3();
	// UDP的端口号
	CEdit NativPort;
	afx_msg void OnEnChangeEdit2();
};
