#pragma once


// CPageEtc 대화 상자입니다.
#include "define.h"

class CPageEtc : public CDialogEx
{
	DECLARE_DYNAMIC(CPageEtc)

public:
	CPageEtc(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPageEtc();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE_ETC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnPpa();
// 	afx_msg void OnBnClickedBtnPpaMulti();
	afx_msg void OnBnClickedBtnOffset();
	afx_msg void OnBnClickedBtnOffsetMulti();
	afx_msg void OnBnClickedBtnFocus();
// 	unsigned int m_uiPitch_Param;
	afx_msg void OnBnClickedBtnPitchCompare();
	afx_msg void OnEnChangePitchParam();
protected:
// 	unsigned int m_uiPitch;
public:
	UINT m_uiPitch;
	afx_msg void OnBnClickedButtonLabeling();
	afx_msg void OnBnClickedButtonParticle();
	afx_msg void OnBnClickedButtonParticleSimulatio();
	afx_msg void OnBnClickedButtonParticleSimulationN();
	afx_msg void OnBnClickedBtnPos();
	virtual BOOL OnInitDialog();
};
