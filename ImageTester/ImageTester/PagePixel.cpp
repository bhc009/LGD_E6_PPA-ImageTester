// PagePixel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTester.h"
#include "PagePixel.h"
#include "afxdialogex.h"

#define CONTROL_SMOOTH_BLUR		0
#define CONTROL_SMOOTH_BOX		1
#define CONTROL_SMOOTH_MEDIAN	2
#define CONTROL_SMOOTH_GAUSSIAN	3

#define CONTROL_DERIVATIVE_SOBEL	0
#define CONTROL_DERIVATIVE_SCHARR	1
#define CONTROL_DERIVATIVE_LAPLACE	2
#define CONTROL_DERIVATIVE_PREWITT	3


#define CONTROL_EDGE_CANNY	0

// CPagePixel 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPagePixel, CDialogEx)

CPagePixel::CPagePixel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPagePixel::IDD, pParent)
	, m_bAutoThreshold(FALSE)
	, m_uiThreshold(127)
{
	m_paramEdge = 20;
}

CPagePixel::~CPagePixel()
{
}

void CPagePixel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ctrlSmooth);
	DDX_Control(pDX, IDC_COMBO2, m_ctrlDerivative);
	DDX_Control(pDX, IDC_COMBO_EDGE, m_ctrlEdge);
	DDX_Control(pDX, IDC_SLIDER_EDGE, m_sliderEdge);
	DDX_Control(pDX, IDC_SLIDER_THRESHOLD, m_ctrlSliderThreshold);
	DDX_Check(pDX, IDC_CHECK_AUTOTHRESHOLD, m_bAutoThreshold);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD, m_uiThreshold);
	DDV_MinMaxUInt(pDX, m_uiThreshold, 0, 255);
}


BEGIN_MESSAGE_MAP(CPagePixel, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPagePixel::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_SMOOTH, &CPagePixel::OnBnClickedButtonSmooth)
	ON_BN_CLICKED(IDC_BUTTON_DERIVATIVE, &CPagePixel::OnBnClickedButtonDerivative)
	ON_BN_CLICKED(IDC_BUTTON_EDGE, &CPagePixel::OnBnClickedButtonEdge)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_THRESHOLD, &CPagePixel::OnNMReleasedcaptureSliderThreshold)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_EDGE, &CPagePixel::OnNMReleasedcaptureSliderEdge)
	ON_BN_CLICKED(IDC_CHECK_AUTOTHRESHOLD, &CPagePixel::OnBnClickedCheckAutothreshold)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_THRESHOLD, &CPagePixel::OnNMCustomdrawSliderThreshold)
	ON_EN_CHANGE(IDC_EDIT_THRESHOLD, &CPagePixel::OnEnChangeEditThreshold)
END_MESSAGE_MAP()


// CPagePixel 메시지 처리기입니다.


void CPagePixel::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_bAutoThreshold )
	{
		::SendMessage( GetParent()->GetSafeHwnd(), MSG_THRESHOLD, MSG_THRESHOLD_OTSU , m_uiThreshold );	
	} else {
		::SendMessage( GetParent()->GetSafeHwnd(), MSG_THRESHOLD, MSG_THRESHOLD_MANUAL , m_uiThreshold );	
	}
}


BOOL CPagePixel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//////////////////////////////////////////////////////////////////////////
	m_ctrlSmooth.AddString( _T("Blur") );
	m_ctrlSmooth.AddString( _T("Box") );
	m_ctrlSmooth.AddString( _T("Median") );
	m_ctrlSmooth.AddString( _T("Gaussian") );

	m_ctrlSmooth.SetCurSel(CONTROL_SMOOTH_BLUR);


	//////////////////////////////////////////////////////////////////////////
	m_ctrlDerivative.AddString( _T("Sobel") );
	m_ctrlDerivative.AddString( _T("Scharr") );
	m_ctrlDerivative.AddString( _T("Laplace") );
	m_ctrlDerivative.AddString( _T("Prewitt") );

	m_ctrlDerivative.SetCurSel(CONTROL_DERIVATIVE_SOBEL);


	//////////////////////////////////////////////////////////////////////////
	m_ctrlEdge.AddString( _T("Canny") );

	m_ctrlEdge.SetCurSel(CONTROL_EDGE_CANNY);


	//////////////////////////////////////////////////////////////////////////
	//
	m_sliderEdge.SetRange(0, 100);
	m_sliderEdge.SetPos(m_paramEdge);

	//////////////////////////////////////////////////////////////////////////
	//
	m_ctrlSliderThreshold.SetRange(0, 255);
	m_ctrlSliderThreshold.SetPos(m_uiThreshold);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPagePixel::OnBnClickedButtonSmooth()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	switch( m_ctrlSmooth.GetCurSel() )
	{
		case CONTROL_SMOOTH_BLUR:
			::SendMessage( GetParent()->GetSafeHwnd(), MSG_SMOOTH, MSG_SMOOTH_BLUR , 0 );	
			break;

		case CONTROL_SMOOTH_BOX:
			::SendMessage( GetParent()->GetSafeHwnd(), MSG_SMOOTH, MSG_SMOOTH_BOX , 0 );	
			break;

		case CONTROL_SMOOTH_MEDIAN:
			::SendMessage( GetParent()->GetSafeHwnd(), MSG_SMOOTH, MSG_SMOOTH_MEDIAN , 0 );	
			break;

		case CONTROL_SMOOTH_GAUSSIAN:
			::SendMessage( GetParent()->GetSafeHwnd(), MSG_SMOOTH, MSG_SMOOTH_GAUSSIAN , 0 );	
			break;

	}
}


void CPagePixel::OnBnClickedButtonDerivative()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	switch( m_ctrlDerivative.GetCurSel() )
	{
	case CONTROL_DERIVATIVE_SOBEL:
		::SendMessage( GetParent()->GetSafeHwnd(), MSG_DERIVATIVE, MSG_DERIVATIVE_SOBEL , 0 );	
		break;

	case CONTROL_DERIVATIVE_SCHARR:
		::SendMessage( GetParent()->GetSafeHwnd(), MSG_DERIVATIVE, MSG_DERIVATIVE_SCHARR , 0 );	
		break;

	case CONTROL_DERIVATIVE_LAPLACE:
		::SendMessage( GetParent()->GetSafeHwnd(), MSG_DERIVATIVE, MSG_DERIVATIVE_LAPLACE , 0 );	
		break;

	case CONTROL_DERIVATIVE_PREWITT:
		::SendMessage( GetParent()->GetSafeHwnd(), MSG_DERIVATIVE, MSG_DERIVATIVE_PREWITT , 0 );	
		break;
	}
}


void CPagePixel::OnBnClickedButtonEdge()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_EDGE, MSG_EDGE_CANNY , m_paramEdge );	
}


void CPagePixel::OnNMReleasedcaptureSliderThreshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nPos = m_ctrlSliderThreshold.GetPos();

// 	::SendMessage( GetParent()->GetSafeHwnd(), MSG_THRESHOLD, MSG_THRESHOLD_OTSU , nPos );	

	*pResult = 0;
}


void CPagePixel::OnNMReleasedcaptureSliderEdge(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_paramEdge = m_sliderEdge.GetPos();

// 	::SendMessage( GetParent()->GetSafeHwnd(), MSG_EDGE, MSG_EDGE_CANNY , nPos );	


	*pResult = 0;
}


void CPagePixel::OnBnClickedCheckAutothreshold()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CPagePixel::OnNMCustomdrawSliderThreshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_uiThreshold = m_ctrlSliderThreshold.GetPos();

	UpdateData(FALSE);

	*pResult = 0;
}


void CPagePixel::OnEnChangeEditThreshold()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_ctrlSliderThreshold.SetPos(m_uiThreshold);
}
