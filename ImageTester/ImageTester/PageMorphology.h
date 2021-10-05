#pragma once


// CPageMorphology ��ȭ �����Դϴ�.
#include "define.h"

class CPageMorphology : public CDialogEx
{
	DECLARE_DYNAMIC(CPageMorphology)

public:
	CPageMorphology(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPageMorphology();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PAGE_MORPHOLOGY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_nSzKernel;
	afx_msg void OnEnChangeEditKernelSize();
	afx_msg void OnBnClickedButtonErode();
	afx_msg void OnBnClickedButtonDilation();
};
