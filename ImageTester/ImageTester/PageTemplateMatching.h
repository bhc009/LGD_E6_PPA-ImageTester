#pragma once


// CPageTemplateMatching ��ȭ �����Դϴ�.

#include "define.h"

class CPageTemplateMatching : public CDialogEx
{
	DECLARE_DYNAMIC(CPageTemplateMatching)

public:
	CPageTemplateMatching(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPageTemplateMatching();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PAGE_TEMPLATE_MATCHING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
