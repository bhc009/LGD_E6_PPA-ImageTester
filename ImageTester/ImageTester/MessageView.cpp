// MessageView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTester.h"
#include "MessageView.h"


// CMessageView

IMPLEMENT_DYNCREATE(CMessageView, CFormView)

CMessageView::CMessageView()
	: CFormView(CMessageView::IDD)
{

}

CMessageView::~CMessageView()
{
}

void CMessageView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBox_message);
}

BEGIN_MESSAGE_MAP(CMessageView, CFormView)
END_MESSAGE_MAP()


// CMessageView �����Դϴ�.

#ifdef _DEBUG
void CMessageView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMessageView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMessageView �޽��� ó�����Դϴ�.
