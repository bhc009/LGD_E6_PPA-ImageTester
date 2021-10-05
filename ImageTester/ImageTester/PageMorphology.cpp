// PageMorphology.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTester.h"
#include "PageMorphology.h"
#include "afxdialogex.h"


// CPageMorphology ��ȭ �����Դϴ�.

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


// CPageMorphology �޽��� ó�����Դϴ�.


void CPageMorphology::OnEnChangeEditKernelSize()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
}


void CPageMorphology::OnBnClickedButtonErode()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_MORPHOLOGY, MSG_MORPHOLOGY_ERODE, m_nSzKernel);	
}


void CPageMorphology::OnBnClickedButtonDilation()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_MORPHOLOGY, MSG_MORPHOLOGY_DILATION, m_nSzKernel);	
}
