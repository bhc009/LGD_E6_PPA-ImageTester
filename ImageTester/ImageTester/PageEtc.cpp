// PageEtc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTester.h"
#include "PageEtc.h"
#include "afxdialogex.h"


// CPageEtc 대화 상자입니다.

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


// CPageEtc 메시지 처리기입니다.
void CPageEtc::OnBnClickedBtnPpa()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nByn = GetDlgItemInt(IDC_EDIT_N_BY_N);

	::SendMessage( GetParent()->GetSafeHwnd(), MSG_PPA_INSPECTION, PPA_ABSOLUTE, nByn);	
}


void CPageEtc::OnBnClickedBtnPos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nByn = GetDlgItemInt(IDC_EDIT_N_BY_N);

	::SendMessage( GetParent()->GetSafeHwnd(), MSG_PPA_INSPECTION, PPA_RELATIVE, nByn);	
}


void CPageEtc::OnBnClickedBtnOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nByn = GetDlgItemInt(IDC_EDIT_N_BY_N);

	::SendMessage( GetParent()->GetSafeHwnd(), MSG_OFFSET_INSPECTION, PPA_POSITION, nByn);	
}


void CPageEtc::OnBnClickedBtnOffsetMulti()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nByn = GetDlgItemInt(IDC_EDIT_N_BY_N);

	::SendMessage( GetParent()->GetSafeHwnd(), MSG_OFFSET_INSPECTION, 1, nByn);	
}


void CPageEtc::OnBnClickedBtnFocus()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_INSPECT_FOCUS, 0, 0);	
}


void CPageEtc::OnBnClickedBtnPitchCompare()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_PITCH_COMPARE, m_uiPitch, 0);	
}


void CPageEtc::OnEnChangePitchParam()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CPageEtc::OnBnClickedButtonLabeling()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_LABELING, 0, 0);	
}


void CPageEtc::OnBnClickedButtonParticle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_INSPECT_PARTICLE, INSPECT_PARTICLE_GENERAL, 0);	
}


//////////////////////////////////////////////////////////////////////////
//
// FMM 표면검사 - 1회검사
//
//////////////////////////////////////////////////////////////////////////
void CPageEtc::OnBnClickedButtonParticleSimulatio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_INSPECT_PARTICLE, INSPECT_PARTICLE_SIMULATION_1, 0);	
}


//////////////////////////////////////////////////////////////////////////
//
// FMM 표면검사 - 연속 검사
//
//////////////////////////////////////////////////////////////////////////
void CPageEtc::OnBnClickedButtonParticleSimulationN()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_INSPECT_PARTICLE, INSPECT_PARTICLE_SIMULATION_N, 0);	
}


BOOL CPageEtc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetDlgItemInt(IDC_EDIT_N_BY_N, 3);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
