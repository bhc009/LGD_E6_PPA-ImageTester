#pragma once


// CPageAlign 대화 상자입니다.
#include "define.h"

class CPageAlign : public CDialogEx
{
	DECLARE_DYNAMIC(CPageAlign)

public:
	CPageAlign(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPageAlign();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE_ALIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
