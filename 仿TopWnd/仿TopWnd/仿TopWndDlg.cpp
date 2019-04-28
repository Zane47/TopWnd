
// 仿TopWndDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "仿TopWnd.h"
#include "仿TopWndDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#define WM_MOUSEMESSAGE WM_USER + 100	// 自定义消息

#define STATE0 "状态：初始"
#define STATE1 "状态：等待用户选择程序"
#define STATE2 "状态：MouseHook.dll错误"
#define STATE3   _T("状态：置顶Success")
#define STATE3_1 _T("状态：置顶FAIL")
#define STATE4   _T("状态：恢复Success")
#define STATE4_1 _T("状态：恢复FAIL")

HINSTANCE __hInstanceDll = NULL;
BOOL      __IsDO = FALSE;

typedef BOOL(CALLBACK *StartHookMouse)(HWND hwnd);
typedef VOID(CALLBACK *StopHookMouse)();

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// C仿TopWndDlg 对话框



C仿TopWndDlg::C仿TopWndDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TOPWND_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);//图标
}

void C仿TopWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C仿TopWndDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()


	ON_MESSAGE(WM_MOUSEMESSAGE, &C仿TopWndDlg::OnMouseMessage) //消息映射

	ON_BN_CLICKED(IDC_BUTTON_DO, &C仿TopWndDlg::OnBnClickedButtonDo)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, &C仿TopWndDlg::OnBnClickedButtonUndo)
END_MESSAGE_MAP()


// C仿TopWndDlg 消息处理程序

BOOL C仿TopWndDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	SetDlgItemTextA(this->m_hWnd,IDC_STATIC_STATE, STATE0);

	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C仿TopWndDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C仿TopWndDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C仿TopWndDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




//启动鼠标钩子
void C仿TopWndDlg::StartHook()
{

	__hInstanceDll = LoadLibrary(_T("HookDll.dll"));

	if (__hInstanceDll == NULL)
	{
		::MessageBox(NULL, (LPCTSTR)GetLastError(), L"LoadLibrary() Error", 0);
		return;
	}

	StartHookMouse StartHook;
	StartHook = (StartHookMouse)::GetProcAddress(
		__hInstanceDll, "StartHookMouse");
	if (StartHook == NULL)
	{
		::MessageBox(NULL, (LPCTSTR)GetLastError(), L"GetProcAddress() Error", 0);
	}

	//HWND* v1 = this->m_hWnd();   //获取父窗口指针

	//将本程序的句柄传给 钩子DLL,用来通信
	if (StartHook(this->m_hWnd))
	{
		SetDlgItemTextA(this->m_hWnd,IDC_STATIC_STATE, STATE1);
	}
	else
	{
		SetDlgItemTextA(this->m_hWnd,IDC_STATIC_STATE, STATE2);
	}
}


//结束鼠标钩子
void C仿TopWndDlg::StopHook()
{
	// TODO: 在此添加控件通知处理程序代码

	StopHookMouse StopHook;
	__hInstanceDll = LoadLibrary(_T("HookDll.dll"));
	if (__hInstanceDll == NULL)
	{
		::MessageBox(NULL, (LPCTSTR)GetLastError(), L"LoadLibrary() Error", 0);
		return;
	}

	StopHook = (StopHookMouse) ::GetProcAddress(
		__hInstanceDll, "StopHookMouse");

	StopHook();//直接调用，否则鼠标卡顿一下（系统卸载Hook）

	if (__hInstanceDll != NULL)
	{
		::FreeLibrary(__hInstanceDll);
	}

}

//置顶button
void C仿TopWndDlg::OnBnClickedButtonDo()
{
	// TODO: 在此添加控件通知处理程序代码

	StartHook();
	
	
	GetDlgItem(IDC_BUTTON_DO)->EnableWindow(FALSE);
	
	
	
	//GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(TRUE);
	__IsDO = TRUE;
}




//恢复button
void C仿TopWndDlg::OnBnClickedButtonUndo()
{
	// TODO: 在此添加控件通知处理程序代码

	StartHook();
	GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(FALSE);

	//GetDlgItem(IDC_BUTTON_DO)->EnableWindow(TRUE);
	__IsDO = FALSE;

}


//实现
LRESULT C仿TopWndDlg::OnMouseMessage(
	WPARAM wParam, LPARAM lParam)
{
	CString v1;
	static PMSLLHOOKSTRUCT MouseHookStruct;
	MouseHookStruct = (PMSLLHOOKSTRUCT)lParam;

	if (wParam == WM_LBUTTONDOWN)
	{
		BOOL IsOk = FALSE;
		HWND hWnd = ::WindowFromPoint(MouseHookStruct->pt);


		//获得父窗口
		while (::GetParent(hWnd))
		{
			hWnd = ::GetParent(hWnd);
		}

			

		TCHAR title[MAX_PATH];
		::GetWindowText(hWnd, title, MAX_PATH);

		

		if (__IsDO == TRUE)
		{
			//HWND_TOPMOST 窗口置顶  
			//SWP_NOMOVE | SWP_NOSIZE 不改变窗口大小和位置
			
			IsOk = ::SetWindowPos(
				hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			//BringWindowToTop();
			GetDlgItem(IDC_BUTTON_DO)->EnableWindow(TRUE);
			if (IsOk)
			{
				if (wcscmp(title,L"") == 0)
				{
					v1.Format(_T("%s %s"), STATE3, L" no get title");
					SetDlgItemText(IDC_STATIC_STATE, v1);
				}
				else
				{
					v1.Format(_T("%s %s"), STATE3, title);
					SetDlgItemText(IDC_STATIC_STATE, v1);
				}
				
			}
			else
			{
				v1.Format(_T("%s %s"), STATE3_1, title);
				SetDlgItemText(IDC_STATIC_STATE, v1);
			}
			
		}
		else
		{
			IsOk = ::SetWindowPos(
				hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(TRUE);
			if (IsOk)
			{
				if (wcscmp(title, L"") == 0)
				{
					v1.Format(_T("%s %s"), STATE4, L" no get title");
					SetDlgItemText(IDC_STATIC_STATE, v1);
				}
				else
				{
					v1.Format(_T("%s %s"), STATE4, title);
					SetDlgItemText(IDC_STATIC_STATE, v1);
				}
				
			}
			else
			{
				v1.Format(_T("%s %s"), STATE4_1, title);
				SetDlgItemText(IDC_STATIC_STATE, v1);
			}
			
		}
		StopHook();
	}

	return 1;
}

