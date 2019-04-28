
// ��TopWndDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��TopWnd.h"
#include "��TopWndDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#define WM_MOUSEMESSAGE WM_USER + 100	// �Զ�����Ϣ

#define STATE0 "״̬����ʼ"
#define STATE1 "״̬���ȴ��û�ѡ�����"
#define STATE2 "״̬��MouseHook.dll����"
#define STATE3   _T("״̬���ö�Success")
#define STATE3_1 _T("״̬���ö�FAIL")
#define STATE4   _T("״̬���ָ�Success")
#define STATE4_1 _T("״̬���ָ�FAIL")

HINSTANCE __hInstanceDll = NULL;
BOOL      __IsDO = FALSE;

typedef BOOL(CALLBACK *StartHookMouse)(HWND hwnd);
typedef VOID(CALLBACK *StopHookMouse)();

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


// C��TopWndDlg �Ի���



C��TopWndDlg::C��TopWndDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TOPWND_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);//ͼ��
}

void C��TopWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C��TopWndDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()


	ON_MESSAGE(WM_MOUSEMESSAGE, &C��TopWndDlg::OnMouseMessage) //��Ϣӳ��

	ON_BN_CLICKED(IDC_BUTTON_DO, &C��TopWndDlg::OnBnClickedButtonDo)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, &C��TopWndDlg::OnBnClickedButtonUndo)
END_MESSAGE_MAP()


// C��TopWndDlg ��Ϣ�������

BOOL C��TopWndDlg::OnInitDialog()
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

	SetDlgItemTextA(this->m_hWnd,IDC_STATIC_STATE, STATE0);

	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void C��TopWndDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void C��TopWndDlg::OnPaint()
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
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR C��TopWndDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




//������깳��
void C��TopWndDlg::StartHook()
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

	//HWND* v1 = this->m_hWnd();   //��ȡ������ָ��

	//��������ľ������ ����DLL,����ͨ��
	if (StartHook(this->m_hWnd))
	{
		SetDlgItemTextA(this->m_hWnd,IDC_STATIC_STATE, STATE1);
	}
	else
	{
		SetDlgItemTextA(this->m_hWnd,IDC_STATIC_STATE, STATE2);
	}
}


//������깳��
void C��TopWndDlg::StopHook()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	StopHookMouse StopHook;
	__hInstanceDll = LoadLibrary(_T("HookDll.dll"));
	if (__hInstanceDll == NULL)
	{
		::MessageBox(NULL, (LPCTSTR)GetLastError(), L"LoadLibrary() Error", 0);
		return;
	}

	StopHook = (StopHookMouse) ::GetProcAddress(
		__hInstanceDll, "StopHookMouse");

	StopHook();//ֱ�ӵ��ã�������꿨��һ�£�ϵͳж��Hook��

	if (__hInstanceDll != NULL)
	{
		::FreeLibrary(__hInstanceDll);
	}

}

//�ö�button
void C��TopWndDlg::OnBnClickedButtonDo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	StartHook();
	
	
	GetDlgItem(IDC_BUTTON_DO)->EnableWindow(FALSE);
	
	
	
	//GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(TRUE);
	__IsDO = TRUE;
}




//�ָ�button
void C��TopWndDlg::OnBnClickedButtonUndo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	StartHook();
	GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(FALSE);

	//GetDlgItem(IDC_BUTTON_DO)->EnableWindow(TRUE);
	__IsDO = FALSE;

}


//ʵ��
LRESULT C��TopWndDlg::OnMouseMessage(
	WPARAM wParam, LPARAM lParam)
{
	CString v1;
	static PMSLLHOOKSTRUCT MouseHookStruct;
	MouseHookStruct = (PMSLLHOOKSTRUCT)lParam;

	if (wParam == WM_LBUTTONDOWN)
	{
		BOOL IsOk = FALSE;
		HWND hWnd = ::WindowFromPoint(MouseHookStruct->pt);


		//��ø�����
		while (::GetParent(hWnd))
		{
			hWnd = ::GetParent(hWnd);
		}

			

		TCHAR title[MAX_PATH];
		::GetWindowText(hWnd, title, MAX_PATH);

		

		if (__IsDO == TRUE)
		{
			//HWND_TOPMOST �����ö�  
			//SWP_NOMOVE | SWP_NOSIZE ���ı䴰�ڴ�С��λ��
			
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

