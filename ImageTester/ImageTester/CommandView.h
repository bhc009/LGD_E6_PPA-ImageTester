#pragma once
#include "afxcmn.h"

#include "define.h"
#include "PageTemplateMatching.h"
#include "PageAlign.h"
#include "PagePixel.h"
#include "PageMorphology.h"
#include "PageEtc.h"


// CCommandView 폼 뷰입니다.
class CImageTesterDoc;
class CCommandView : public CFormView
{
	DECLARE_DYNCREATE(CCommandView)

protected:
	CCommandView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CCommandView();

public:
	enum { IDD = IDD_COMMANDVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	CImageTesterDoc *m_pDoc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	CTabCtrl m_ctrlTab;
	CPageTemplateMatching *m_pageTemplate;
	CPageAlign *m_pPageAlign;
	CPagePixel *m_pPagePixel;
	CPageMorphology *m_pPageMorphology;
	CPageEtc *m_pPageEtc;

	BOOL m_chkRoi;
	int m_libType;

	virtual void OnInitialUpdate();

	afx_msg void OnBnClickedButtonTest2();
	afx_msg void OnTcnSelchangeTabCommand(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonPush();
	afx_msg void OnBnClickedButtonPopImage();
	afx_msg void OnBnClickedButtonDeleteImage();
	afx_msg void OnBnClickedCheckRoi();
	afx_msg LRESULT OnTemplateMatching( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnAlign( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnInspectPPA( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnInspect_offset( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnInspect_focus( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThreshold( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSmooth( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnDerivative( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnEdge( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnMorphology( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnPitchCompare( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnLabeling( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnInspectParticle( WPARAM wParam, LPARAM lParam );

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	BOOL m_bDisplayProcessingImage;
	afx_msg void OnBnClickedCheckDispay();
};
