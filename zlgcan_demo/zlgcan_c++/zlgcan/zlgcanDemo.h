
// zlgcan.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cusbcanfdx00udemoApp:
// �йش����ʵ�֣������ zlgcan.cpp
//

class Cpciecanfdx00uApp : public CWinAppEx
{
public:
	Cpciecanfdx00uApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cpciecanfdx00uApp theApp;