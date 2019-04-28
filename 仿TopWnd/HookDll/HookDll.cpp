// HookDll.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "HookDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MOUSEMESSAGE WM_USER + 100	// �Զ�����Ϣ



// �������Σ������̹߳���
#pragma data_seg("SHARED")
static HHOOK __hhkMouse = NULL;			// ��깳�Ӿ��
static HINSTANCE __hInstance = NULL;	// ��DLL��ʵ�����
static HWND __hWnd = NULL;				// ����DLL�������ھ��
#pragma data_seg()
#pragma comment(linker,"/section:SHARED,rws")


//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CHookDllApp

BEGIN_MESSAGE_MAP(CHookDllApp, CWinApp)
END_MESSAGE_MAP()


// CHookDllApp ����

CHookDllApp::CHookDllApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CHookDllApp ����

CHookDllApp theApp;


// CHookDllApp ��ʼ��

BOOL CHookDllApp::InitInstance()
{
	CWinApp::InitInstance();


	//
	// ��ȡ�����dll���
	//
	__hInstance = ::AfxGetInstanceHandle();


	return TRUE;
}



// ����ͼ�����Ӻ���
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// �������Ϣʱ�����䷢��������

	if (__hWnd != NULL && nCode == HC_ACTION)//HC_ACTION �������Ϣ
	{
		//֪ͨMFC
		::SendMessage(__hWnd, WM_MOUSEMESSAGE, wParam, lParam);
	}

	return CallNextHookEx(__hhkMouse, nCode, wParam, lParam);

}






// ��װ�ͼ�����Ӻ������ػ�ϵͳ���е������Ϣ
BOOL WINAPI StartHookMouse(HWND hwnd)
{
	//��������Ĵ��ھ��������

	__hWnd = hwnd;




	/*
	��ʹ��SetWindowsHook�ػ�����¼�ʱ�����ʹ��WH_MOUSE��������ֻ�ܼ�ع�������ģ�������¼���
����Ҫ�ػ�����ϵͳ������¼�����ôʹ��WH_MOUSE_LL������
	
	*/
	__hhkMouse = SetWindowsHookEx(
		WH_MOUSE_LL,
		LowLevelMouseProc,
		__hInstance,
		0
	);

	if (NULL == __hhkMouse)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}


// ж�صͼ���깳��
VOID WINAPI StopHookMouse()
{
	if (__hhkMouse != NULL)
	{
		::UnhookWindowsHookEx(__hhkMouse);
	}
}


