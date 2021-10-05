// PageAlign.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTester.h"
#include "PageAlign.h"
#include "afxdialogex.h"


// CPageAlign 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPageAlign, CDialogEx)

CPageAlign::CPageAlign(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageAlign::IDD, pParent)
{

}

CPageAlign::~CPageAlign()
{
}

void CPageAlign::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageAlign, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON5, &CPageAlign::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &CPageAlign::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPageAlign::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPageAlign::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPageAlign::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CPageAlign::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CPageAlign::OnBnClickedButton7)
END_MESSAGE_MAP()


// CPageAlign 메시지 처리기입니다.


void CPageAlign::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Create main pattern
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_ALIGN, MSG_ALIGN_CREATE , MSG_MAIN_MARK);	
}


void CPageAlign::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Create SUB pattern
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_ALIGN, MSG_ALIGN_CREATE , MSG_SUB_MARK);	
}


void CPageAlign::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// FIND MAIN PATTERN #1
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_ALIGN, MSG_ALIGN_FIND , MSG_MAIN_MARK_1);	
}


void CPageAlign::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// FIND MAIN PATTERN #2
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_ALIGN, MSG_ALIGN_FIND , MSG_MAIN_MARK_2);	
}


void CPageAlign::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// FIND SUB PATTERN #1
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_ALIGN, MSG_ALIGN_FIND , MSG_SUB_MARK_1);	
}


void CPageAlign::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// FIND SUB PATTERN #2
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_ALIGN, MSG_ALIGN_FIND , MSG_SUB_MARK_2);	
}


void CPageAlign::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_ALIGN, MSG_ALIGN_CALCULATE , 0);	
}
