#pragma once
#include "afxwin.h"



// CMessageView �� ���Դϴ�.
class CImageTesterDoc;

class CMessageView : public CFormView
{
	DECLARE_DYNCREATE(CMessageView)

protected:
	CMessageView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listBox_message;
};


