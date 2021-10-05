#pragma once


// CPagePixel ��ȭ �����Դϴ�.
#include "define.h"
#include "afxwin.h"
#include "afxcmn.h"

class CPagePixel : public CDialogEx
{
	DECLARE_DYNAMIC(CPagePixel)

public:
	CPagePixel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPagePixel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PAGE_PIXEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

	int m_paramEdge;


	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	CComboBox m_ctrlSmooth;
	afx_msg void OnBnClickedButtonSmooth();
	CComboBox m_ctrlDerivative;
	afx_msg void OnBnClickedButtonDerivative();
	CComboBox m_ctrlEdge;
	afx_msg void OnBnClickedButtonEdge();
	CSliderCtrl m_sliderEdge;
	CSliderCtrl m_ctrlSliderThreshold;
	afx_msg void OnNMReleasedcaptureSliderThreshold(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderEdge(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL m_bAutoThreshold;
	afx_msg void OnBnClickedCheckAutothreshold();
	UINT m_uiThreshold;
	afx_msg void OnNMCustomdrawSliderThreshold(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditThreshold();
};
