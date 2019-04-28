// HookDll.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "HookDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MOUSEMESSAGE WM_USER + 100	// 自定义消息



// 共享代码段，所有线程共享
#pragma data_seg("SHARED")
static HHOOK __hhkMouse = NULL;			// 鼠标钩子句柄
static HINSTANCE __hInstance = NULL;	// 本DLL的实例句柄
static HWND __hWnd = NULL;				// 调用DLL的主窗口句柄
#pragma data_seg()
#pragma comment(linker,"/section:SHARED,rws")


//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CHookDllApp

BEGIN_MESSAGE_MAP(CHookDllApp, CWinApp)
END_MESSAGE_MAP()


// CHookDllApp 构造

CHookDllApp::CHookDllApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CHookDllApp 对象

CHookDllApp theApp;


// CHookDllApp 初始化

BOOL CHookDllApp::InitInstance()
{
	CWinApp::InitInstance();


	//
	// 获取自身的dll句柄
	//
	__hInstance = ::AfxGetInstanceHandle();


	return TRUE;
}



// 定义低级鼠标子函数
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// 有鼠标消息时，将其发给主程序

	if (__hWnd != NULL && nCode == HC_ACTION)//HC_ACTION 有鼠标信息
	{
		//通知MFC
		::SendMessage(__hWnd, WM_MOUSEMESSAGE, wParam, lParam);
	}

	return CallNextHookEx(__hhkMouse, nCode, wParam, lParam);

}






// 安装低级鼠标子函数，截获系统所有的鼠标消息
BOOL WINAPI StartHookMouse(HWND hwnd)
{
	//这里自身的窗口句柄传过来

	__hWnd = hwnd;




	/*
	在使用SetWindowsHook截获鼠标事件时，如果使用WH_MOUSE参数，则只能监控钩子所在模块的鼠标事件。
如需要截获整个系统的鼠标事件，那么使用WH_MOUSE_LL参数。
	
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


// 卸载低级鼠标钩子
VOID WINAPI StopHookMouse()
{
	if (__hhkMouse != NULL)
	{
		::UnhookWindowsHookEx(__hhkMouse);
	}
}


