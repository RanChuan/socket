#pragma once
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include "afxcmn.h"


// UpDataDlg 对话框

class UpDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(UpDataDlg)

public:
	UpDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~UpDataDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UPDATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 显示操作执行情况
	CEdit CmdNote;
	afx_msg void OnEnChangeMfceditbrowse1();
	// 从这里面打开程序文件
	CMFCEditBrowseCtrl File;
	afx_msg void OnBnClickedStartUpdata();
protected:
	afx_msg LRESULT OnWkErrReturn(WPARAM wParam, LPARAM lParam);
public:
	// 升级进度
	CProgressCtrl upDataIng;
};
