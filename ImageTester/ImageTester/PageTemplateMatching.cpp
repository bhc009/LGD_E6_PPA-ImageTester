// PageTemplateMatching.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTester.h"
#include "PageTemplateMatching.h"
#include "afxdialogex.h"


// CPageTemplateMatching 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPageTemplateMatching, CDialogEx)

CPageTemplateMatching::CPageTemplateMatching(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageTemplateMatching::IDD, pParent)
	, m_method(0)
{

}

CPageTemplateMatching::~CPageTemplateMatching()
{
}

void CPageTemplateMatching::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_method);
}


BEGIN_MESSAGE_MAP(CPageTemplateMatching, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_TEMPLATE, &CPageTemplateMatching::OnBnClickedButtonCreateTemplate)
	ON_BN_CLICKED(IDC_BUTTON_FIND_TEMPLATE, &CPageTemplateMatching::OnBnClickedButtonFindTemplate)
	ON_BN_CLICKED(IDC_RADIO1, &CPageTemplateMatching::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CPageTemplateMatching::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_MODEL, &CPageTemplateMatching::OnBnClickedButtonSaveModel)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_MODEL, &CPageTemplateMatching::OnBnClickedButtonLoadModel)
END_MESSAGE_MAP()


// CPageTemplateMatching 메시지 처리기입니다.


void CPageTemplateMatching::OnBnClickedButtonCreateTemplate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	switch( m_method )
	{
	case ITP_NCC:
		::SendMessage( GetParent()->GetSafeHwnd(), MSG_TEMPLATE_MATCHING, MSG_CREATE_TEMPLATE, ITP_NCC);	
		break;

	case ITP_SHAPE_BASED:
		::SendMessage( GetParent()->GetSafeHwnd(), MSG_TEMPLATE_MATCHING, MSG_CREATE_TEMPLATE, ITP_SHAPE_BASED);	
		break;
	}
}


void CPageTemplateMatching::OnBnClickedButtonFindTemplate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	switch( m_method )
	{
	case ITP_NCC:
		::SendMessage( GetParent()->GetSafeHwnd(), MSG_TEMPLATE_MATCHING, MSG_FIND_TEMPLATE, ITP_NCC);	
		break;

	case ITP_SHAPE_BASED:
		::SendMessage( GetParent()->GetSafeHwnd(), MSG_TEMPLATE_MATCHING, MSG_FIND_TEMPLATE, ITP_SHAPE_BASED);	
		break;
	}
}


BOOL CPageTemplateMatching::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
// 	CRect rtPosition, rt;
// 
// 	(GetDlgItem(IDC_STATIC_HISTO))->GetWindowRect(rtPosition);
// 	ScreenToClient(rtPosition);
// 	rt = CRect( rtPosition.left, rtPosition.top, rtPosition.left+256, rtPosition.top+128);
// 
// 	if( m_pImageView==NULL )
// 	{
// 		m_pImageView = new CImageView;
// 		m_pImageView->Create(NULL,NULL,NULL,rt,this,NULL);
// 		m_pImageView->SetWindowPos( NULL, rt.left, rt.top, rt.Width(), rt.Height(), SWP_SHOWWINDOW );
// 	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPageTemplateMatching::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CPageTemplateMatching::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CPageTemplateMatching::OnBnClickedButtonSaveModel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	switch( m_method )
	{
	case ITP_NCC:
		::SendMessage( GetParent()->GetSafeHwnd(), MSG_TEMPLATE_MATCHING, MSG_SAVE_TEMPLATE, ITP_NCC);	
		break;

	case ITP_SHAPE_BASED:
		::SendMessage( GetParent()->GetSafeHwnd(), MSG_TEMPLATE_MATCHING, MSG_SAVE_TEMPLATE, ITP_SHAPE_BASED);	
		break;
	}
}


void CPageTemplateMatching::OnBnClickedButtonLoadModel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	switch( m_method )
	{
	case ITP_NCC:
		::SendMessage( GetParent()->GetSafeHwnd(), MSG_TEMPLATE_MATCHING, MSG_LOAD_TEMPLATE, ITP_NCC);	
		break;

	case ITP_SHAPE_BASED:
		::SendMessage( GetParent()->GetSafeHwnd(), MSG_TEMPLATE_MATCHING, MSG_LOAD_TEMPLATE, ITP_SHAPE_BASED);	
		break;
	}
}
