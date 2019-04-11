
// SocketDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "mySocket.h"
#include "WK_Thread.h"

// CSocketDlg �Ի���
class CSocketDlg : public CDialogEx
{
// ����
public:
	CSocketDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOCKET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	// ����IP��ַѡ���
	CComboBox NativeIP;
	// ����ִ�н����ʾ
	CEdit CmdNote;
protected:
	afx_msg LRESULT OnWkErrReturn(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedSend();
	// ��������������������ݱ�����
	CEdit SendBox;
protected:
	afx_msg LRESULT OnWkReceived(WPARAM wParam, LPARAM lParam);
public:
	// ��ʾ���յ�������
	CEdit RecvBox;
	afx_msg void OnBnClickedUpData();
	// Ŀ�����ip��ַ
	CComboBox TargetIP;
	afx_msg void OnBnClickedClearRecvBox();
	// ��������հ�ť
	CButton RecvBoxSlearBut;
	afx_msg void OnEnChangeEdit3();
	// UDP�Ķ˿ں�
	CEdit NativPort;
	afx_msg void OnEnChangeEdit2();
};
