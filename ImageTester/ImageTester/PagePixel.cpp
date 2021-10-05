// PagePixel.cpp : ���� �����Դϴ�.
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

// CPagePixel ��ȭ �����Դϴ�.

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


// CPagePixel �޽��� ó�����Դϴ�.


void CPagePixel::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CPagePixel::OnBnClickedButtonSmooth()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SendMessage( GetParent()->GetSafeHwnd(), MSG_EDGE, MSG_EDGE_CANNY , m_paramEdge );	
}


void CPagePixel::OnNMReleasedcaptureSliderThreshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nPos = m_ctrlSliderThreshold.GetPos();

// 	::SendMessage( GetParent()->GetSafeHwnd(), MSG_THRESHOLD, MSG_THRESHOLD_OTSU , nPos );	

	*pResult = 0;
}


void CPagePixel::OnNMReleasedcaptureSliderEdge(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_paramEdge = m_sliderEdge.GetPos();

// 	::SendMessage( GetParent()->GetSafeHwnd(), MSG_EDGE, MSG_EDGE_CANNY , nPos );	


	*pResult = 0;
}


void CPagePixel::OnBnClickedCheckAutothreshold()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
}


void CPagePixel::OnNMCustomdrawSliderThreshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_uiThreshold = m_ctrlSliderThreshold.GetPos();

	UpdateData(FALSE);

	*pResult = 0;
}


void CPagePixel::OnEnChangeEditThreshold()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	m_ctrlSliderThreshold.SetPos(m_uiThreshold);
}
