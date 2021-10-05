// PageMorphology.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTester.h"
#include "PageMorphology.h"
#include "afxdialogex.h"


// CPageMorphology 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPageMorphology, CDialogEx)

CPageMorphology::CPageMorphology(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageMorphology::IDD, pParent)
	, m_nSzKernel(3)
{

}

CPageMorphology::~CPageMorphology()
{
}

void CPageMorphology::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KERNEL_SIZE, m_nSzKernel);
	DDV_MinMaxInt(pDX, m_nSzKernel, 1, 99);
}


BEGIN_MESSAGE_MAP(CPageMorphology, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_KERNEL_SIZE, &CPageMorphology::OnEnChangeEditKernelSize)
	ON_BN_CLICKED(IDC_BUTTON_ERODE, &CPageMorphology::OnBnClickedButtonErode)
	ON_BN_CLICKED(IDC_BUTTON_DILATION, &CPageMorphology::OnBnClickedButtonDilation)
END_MESSAGE_MAP()


// CPageMorphology 메시지 처리기입니다.


void CPageMorphology::OnEnChangeEditKernelSize()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CPageMorphology::OnBnClickedButtonErode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_MORPHOLOGY, MSG_MORPHOLOGY_ERODE, m_nSzKernel);	
}


void CPageMorphology::OnBnClickedButtonDilation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_MORPHOLOGY, MSG_MORPHOLOGY_DILATION, m_nSzKernel);	
}
