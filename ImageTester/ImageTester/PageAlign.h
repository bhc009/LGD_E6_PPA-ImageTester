#pragma once


// CPageAlign ��ȭ �����Դϴ�.
#include "define.h"

class CPageAlign : public CDialogEx
{
	DECLARE_DYNAMIC(CPageAlign)

public:
	CPageAlign(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPageAlign();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PAGE_ALIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
};
