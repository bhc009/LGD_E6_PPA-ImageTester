// PageAlign.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTester.h"
#include "PageAlign.h"
#include "afxdialogex.h"


// CPageAlign ��ȭ �����Դϴ�.

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


// CPageAlign �޽��� ó�����Դϴ�.


void CPageAlign::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// Create main pattern
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_ALIGN, MSG_ALIGN_CREATE , MSG_MAIN_MARK);	
}


void CPageAlign::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// Create SUB pattern
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_ALIGN, MSG_ALIGN_CREATE , MSG_SUB_MARK);	
}


void CPageAlign::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// FIND MAIN PATTERN #1
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_ALIGN, MSG_ALIGN_FIND , MSG_MAIN_MARK_1);	
}


void CPageAlign::OnBnClickedButton5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// FIND MAIN PATTERN #2
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_ALIGN, MSG_ALIGN_FIND , MSG_MAIN_MARK_2);	
}


void CPageAlign::OnBnClickedButton4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// FIND SUB PATTERN #1
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_ALIGN, MSG_ALIGN_FIND , MSG_SUB_MARK_1);	
}


void CPageAlign::OnBnClickedButton6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// FIND SUB PATTERN #2
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_ALIGN, MSG_ALIGN_FIND , MSG_SUB_MARK_2);	
}


void CPageAlign::OnBnClickedButton7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_ALIGN, MSG_ALIGN_CALCULATE , 0);	
}
