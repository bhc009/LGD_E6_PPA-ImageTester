#pragma once
#include "afxwin.h"



// CMessageView 폼 뷰입니다.
class CImageTesterDoc;

class CMessageView : public CFormView
{
	DECLARE_DYNCREATE(CMessageView)

protected:
	CMessageView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMessageView();

public:
	enum { IDD = IDD_MESSAGEVIEW };
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
	CListBox m_listBox_message;
};


