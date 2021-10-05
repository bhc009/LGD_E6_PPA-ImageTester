// PageEtc.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTester.h"
#include "PageEtc.h"
#include "afxdialogex.h"


// CPageEtc ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPageEtc, CDialogEx)

CPageEtc::CPageEtc(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageEtc::IDD, pParent)
, m_uiPitch(62)
{

}

CPageEtc::~CPageEtc()
{
}

void CPageEtc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PITCH_PARAM, m_uiPitch);
	DDV_MinMaxUInt(pDX, m_uiPitch, 1, 999);
}


BEGIN_MESSAGE_MAP(CPageEtc, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_PPA, &CPageEtc::OnBnClickedBtnPpa)
	ON_BN_CLICKED(IDC_BTN_POS, &CPageEtc::OnBnClickedBtnPos)
	ON_BN_CLICKED(IDC_BTN_OFFSET, &CPageEtc::OnBnClickedBtnOffset)
	ON_BN_CLICKED(IDC_BTN_OFFSET_MULTI, &CPageEtc::OnBnClickedBtnOffsetMulti)
	ON_BN_CLICKED(IDC_BTN_FOCUS, &CPageEtc::OnBnClickedBtnFocus)
	ON_BN_CLICKED(IDC_BTN_PITCH_COMPARE, &CPageEtc::OnBnClickedBtnPitchCompare)
	ON_EN_CHANGE(IDC_PITCH_PARAM, &CPageEtc::OnEnChangePitchParam)
	ON_BN_CLICKED(IDC_BUTTON_LABELING, &CPageEtc::OnBnClickedButtonLabeling)
	ON_BN_CLICKED(IDC_BUTTON_PARTICLE, &CPageEtc::OnBnClickedButtonParticle)
	ON_BN_CLICKED(IDC_BUTTON_PARTICLE_SIMULATIO, &CPageEtc::OnBnClickedButtonParticleSimulatio)
	ON_BN_CLICKED(IDC_BUTTON_PARTICLE_SIMULATION_N, &CPageEtc::OnBnClickedButtonParticleSimulationN)
END_MESSAGE_MAP()


// CPageEtc �޽��� ó�����Դϴ�.
void CPageEtc::OnBnClickedBtnPpa()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nByn = GetDlgItemInt(IDC_EDIT_N_BY_N);

	::SendMessage( GetParent()->GetSafeHwnd(), MSG_PPA_INSPECTION, PPA_ABSOLUTE, nByn);	
}


void CPageEtc::OnBnClickedBtnPos()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nByn = GetDlgItemInt(IDC_EDIT_N_BY_N);

	::SendMessage( GetParent()->GetSafeHwnd(), MSG_PPA_INSPECTION, PPA_RELATIVE, nByn);	
}


void CPageEtc::OnBnClickedBtnOffset()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nByn = GetDlgItemInt(IDC_EDIT_N_BY_N);

	::SendMessage( GetParent()->GetSafeHwnd(), MSG_OFFSET_INSPECTION, PPA_POSITION, nByn);	
}


void CPageEtc::OnBnClickedBtnOffsetMulti()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nByn = GetDlgItemInt(IDC_EDIT_N_BY_N);

	::SendMessage( GetParent()->GetSafeHwnd(), MSG_OFFSET_INSPECTION, 1, nByn);	
}


void CPageEtc::OnBnClickedBtnFocus()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_INSPECT_FOCUS, 0, 0);	
}


void CPageEtc::OnBnClickedBtnPitchCompare()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_PITCH_COMPARE, m_uiPitch, 0);	
}


void CPageEtc::OnEnChangePitchParam()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
}


void CPageEtc::OnBnClickedButtonLabeling()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_LABELING, 0, 0);	
}


void CPageEtc::OnBnClickedButtonParticle()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_INSPECT_PARTICLE, INSPECT_PARTICLE_GENERAL, 0);	
}


//////////////////////////////////////////////////////////////////////////
//
// FMM ǥ��˻� - 1ȸ�˻�
//
//////////////////////////////////////////////////////////////////////////
void CPageEtc::OnBnClickedButtonParticleSimulatio()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_INSPECT_PARTICLE, INSPECT_PARTICLE_SIMULATION_1, 0);	
}


//////////////////////////////////////////////////////////////////////////
//
// FMM ǥ��˻� - ���� �˻�
//
//////////////////////////////////////////////////////////////////////////
void CPageEtc::OnBnClickedButtonParticleSimulationN()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_INSPECT_PARTICLE, INSPECT_PARTICLE_SIMULATION_N, 0);	
}


BOOL CPageEtc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetDlgItemInt(IDC_EDIT_N_BY_N, 3);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
