
// ��TopWnd.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C��TopWndApp: 
// �йش����ʵ�֣������ ��TopWnd.cpp
//

class C��TopWndApp : public CWinApp
{
public:
	C��TopWndApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern C��TopWndApp theApp;