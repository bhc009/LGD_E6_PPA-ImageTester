#pragma once


// CPageTemplateMatching 대화 상자입니다.

#include "define.h"

class CPageTemplateMatching : public CDialogEx
{
	DECLARE_DYNAMIC(CPageTemplateMatching)

public:
	CPageTemplateMatching(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPageTemplateMatching();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE_TEMPLATE_MATCHING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	virtual BOOL OnInitDialog();

	int m_method;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCreateTemplate();
	afx_msg void OnBnClickedButtonFindTemplate();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedButtonSaveModel();
	afx_msg void OnBnClickedButtonLoadModel();
};
