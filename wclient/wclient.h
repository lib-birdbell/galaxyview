
// wclient.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CwclientApp:
// �� Ŭ������ ������ ���ؼ��� wclient.cpp�� �����Ͻʽÿ�.
//

class CwclientApp : public CWinApp
{
public:
	CwclientApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CwclientApp theApp;