#pragma once
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include "afxcmn.h"


// UpDataDlg �Ի���

class UpDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(UpDataDlg)

public:
	UpDataDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~UpDataDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UPDATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ��ʾ����ִ�����
	CEdit CmdNote;
	afx_msg void OnEnChangeMfceditbrowse1();
	// ��������򿪳����ļ�
	CMFCEditBrowseCtrl File;
	afx_msg void OnBnClickedStartUpdata();
protected:
	afx_msg LRESULT OnWkErrReturn(WPARAM wParam, LPARAM lParam);
public:
	// ��������
	CProgressCtrl upDataIng;
};
