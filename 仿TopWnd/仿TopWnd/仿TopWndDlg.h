
// ��TopWndDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// C��TopWndDlg �Ի���
class C��TopWndDlg : public CDialogEx
{
// ����
public:
	C��TopWndDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOPWND_DIALOG };
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



	afx_msg LRESULT OnMouseMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:



	void C��TopWndDlg::StartHook();
	void C��TopWndDlg::StopHook();

	afx_msg void OnBnClickedButtonDo();
	afx_msg void OnBnClickedButtonUndo();
};
