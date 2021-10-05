#pragma once


// CPageMorphology 대화 상자입니다.
#include "define.h"

class CPageMorphology : public CDialogEx
{
	DECLARE_DYNAMIC(CPageMorphology)

public:
	CPageMorphology(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPageMorphology();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE_MORPHOLOGY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nSzKernel;
	afx_msg void OnEnChangeEditKernelSize();
	afx_msg void OnBnClickedButtonErode();
	afx_msg void OnBnClickedButtonDilation();
};
