
// ImageTester.h : ImageTester ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#define IT_IMAGE	CBhImage

// CImageTesterApp:
// �� Ŭ������ ������ ���ؼ��� ImageTester.cpp�� �����Ͻʽÿ�.
//

class CImageTesterApp : public CWinAppEx
{
public:
	CImageTesterApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CImageTesterApp theApp;
