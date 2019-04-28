
// 仿TopWndDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// C仿TopWndDlg 对话框
class C仿TopWndDlg : public CDialogEx
{
// 构造
public:
	C仿TopWndDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOPWND_DIALOG };
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



	afx_msg LRESULT OnMouseMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:



	void C仿TopWndDlg::StartHook();
	void C仿TopWndDlg::StopHook();

	afx_msg void OnBnClickedButtonDo();
	afx_msg void OnBnClickedButtonUndo();
};
