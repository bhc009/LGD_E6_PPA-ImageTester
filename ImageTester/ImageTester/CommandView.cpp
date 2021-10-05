// CommandView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTester.h"
#include "CommandView.h"

#include "ImageTesterDoc.h"

#define PAGE_ALIGN	0
#define PAGE_TEMPLATE_MATCHING	1
#define PAGE_PIXEL	2
#define PAGE_MORPHOLOGY	3
#define PAGE_ETC	4


// CCommandView

IMPLEMENT_DYNCREATE(CCommandView, CFormView)

CCommandView::CCommandView()
	: CFormView(CCommandView::IDD)
	, m_chkRoi(FALSE)
	, m_libType(0)
	, m_bDisplayProcessingImage(FALSE)
{
	m_pageTemplate = NULL;
	m_pPageAlign = NULL;
	m_pPagePixel = NULL;
	m_pPageEtc = NULL;
	m_pPageMorphology = NULL;
}

CCommandView::~CCommandView()
{
	if(m_pageTemplate )
	{
		delete[] m_pageTemplate;
	}
	m_pageTemplate = NULL;

	if( m_pPageAlign )
	{
		delete[] m_pPageAlign;
	}
	m_pPageAlign = NULL;

	if( m_pPagePixel )
	{
		delete[] m_pPagePixel;
	}
	m_pPagePixel = NULL;

	if( m_pPageMorphology )
	{
		delete[] m_pPageMorphology;
	}
	m_pPageMorphology = NULL;

	if( m_pPageEtc )
	{
		delete[] m_pPageEtc;
	}
	m_pPageEtc = NULL;
}

void CCommandView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_COMMAND, m_ctrlTab);
	DDX_Check(pDX, IDC_CHECK_ROI, m_chkRoi);
	DDX_Radio(pDX, IDC_RADIO1, m_libType);
	DDX_Check(pDX, IDC_CHECK_DISPAY, m_bDisplayProcessingImage);
}

BEGIN_MESSAGE_MAP(CCommandView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CCommandView::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_PUSH, &CCommandView::OnBnClickedButtonPush)
	ON_BN_CLICKED(IDC_BUTTON_POP_IMAGE, &CCommandView::OnBnClickedButtonPopImage)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_IMAGE, &CCommandView::OnBnClickedButtonDeleteImage)
	ON_BN_CLICKED(IDC_CHECK_ROI, &CCommandView::OnBnClickedCheckRoi)
	ON_BN_CLICKED(IDC_BUTTON_TEST2, &CCommandView::OnBnClickedButtonTest2)
	ON_BN_CLICKED(IDC_BUTTON1, &CCommandView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, &CCommandView::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CCommandView::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CCommandView::OnBnClickedRadio3)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_COMMAND, &CCommandView::OnTcnSelchangeTabCommand)
	ON_MESSAGE( MSG_TEMPLATE_MATCHING, OnTemplateMatching )  // 삽입된 부분 
	ON_MESSAGE( MSG_ALIGN, OnAlign )  // 삽입된 부분 
	ON_MESSAGE( MSG_PPA_INSPECTION, OnInspectPPA )  // 삽입된 부분 
	ON_MESSAGE( MSG_OFFSET_INSPECTION, OnInspect_offset )  // 삽입된 부분 
	ON_MESSAGE( MSG_INSPECT_FOCUS, OnInspect_focus )  // 삽입된 부분 
	ON_MESSAGE( MSG_THRESHOLD, OnThreshold )  // 삽입된 부분 
	ON_MESSAGE( MSG_SMOOTH, OnSmooth )  // 삽입된 부분 
	ON_MESSAGE( MSG_DERIVATIVE, OnDerivative )  // 삽입된 부분 
	ON_MESSAGE( MSG_EDGE, OnEdge )  // 삽입된 부분 
	ON_MESSAGE( MSG_MORPHOLOGY, OnMorphology )  // 삽입된 부분 
	ON_MESSAGE( MSG_PITCH_COMPARE, OnPitchCompare )  // 삽입된 부분 
	ON_MESSAGE( MSG_LABELING, OnLabeling )  // 삽입된 부분 
	ON_MESSAGE( MSG_INSPECT_PARTICLE, OnInspectParticle )  // 삽입된 부분 
	ON_BN_CLICKED(IDC_CHECK_DISPAY, &CCommandView::OnBnClickedCheckDispay)
END_MESSAGE_MAP()


// CCommandView 진단입니다.

#ifdef _DEBUG
void CCommandView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCommandView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCommandView 메시지 처리기입니다.


void CCommandView::OnBnClickedButtonTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pDoc->clear_result_data();
}


void CCommandView::OnBnClickedButtonPush()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pDoc->ImageList_Push();
}


void CCommandView::OnBnClickedButtonPopImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pDoc->ImageList_Pop();
}


void CCommandView::OnBnClickedButtonDeleteImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pDoc->ImageList_Delete();
}


void CCommandView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//
	// Tab 생성
	//
	if( m_ctrlTab.GetItemCount()==0 )
	{
		m_ctrlTab.InsertItem( 0, "Align");
		m_ctrlTab.InsertItem( 1, "매칭");
		m_ctrlTab.InsertItem( 2, "Pixel.");
		m_ctrlTab.InsertItem( 3, "Morph");
		m_ctrlTab.InsertItem( 4, "기타");
	}

	// Tap control의 위치
	CRect rtTab;	
	m_ctrlTab.GetWindowRect(&rtTab);
	ScreenToClient(&rtTab);

	// Tab page 생성
	CRect rtPage;	// Page의 위치
	if( m_pageTemplate==NULL )
	{
		m_pageTemplate = new CPageTemplateMatching;
 		m_pageTemplate->Create(IDD_PAGE_TEMPLATE_MATCHING, this);
		m_pageTemplate->GetWindowRect(&rtPage);
		m_pageTemplate->MoveWindow(rtTab.left+5, rtTab.top+25, rtPage.Width(), rtPage.Height());
		m_pageTemplate->ShowWindow(SW_HIDE);
	}

	if( m_pPageAlign==NULL )
	{
		m_pPageAlign = new CPageAlign;
		m_pPageAlign->Create(IDD_PAGE_ALIGN, this);
		m_pPageAlign->GetWindowRect(&rtPage);
		m_pPageAlign->MoveWindow(rtTab.left+5, rtTab.top+25, rtPage.Width(), rtPage.Height());
		m_pPageAlign->ShowWindow(SW_SHOW);
	}

	if( m_pPagePixel==NULL )
	{
		m_pPagePixel = new CPagePixel;
		m_pPagePixel->Create(IDD_PAGE_PIXEL, this);
		m_pPagePixel->GetWindowRect(&rtPage);
		m_pPagePixel->MoveWindow(rtTab.left+5, rtTab.top+25, rtPage.Width(), rtPage.Height());
		m_pPagePixel->ShowWindow(SW_HIDE);
	}

	if( m_pPageMorphology==NULL )
	{
		m_pPageMorphology = new CPageMorphology;
		m_pPageMorphology->Create(IDD_PAGE_MORPHOLOGY, this);
		m_pPageMorphology->GetWindowRect(&rtPage);
		m_pPageMorphology->MoveWindow(rtTab.left+5, rtTab.top+25, rtPage.Width(), rtPage.Height());
		m_pPageMorphology->ShowWindow(SW_HIDE);
	}

	if( m_pPageEtc==NULL )
	{
		m_pPageEtc = new CPageEtc;
		m_pPageEtc->Create(IDD_PAGE_ETC, this);
		m_pPageEtc->GetWindowRect(&rtPage);
		m_pPageEtc->MoveWindow(rtTab.left+5, rtTab.top+25, rtPage.Width(), rtPage.Height());
		m_pPageEtc->ShowWindow(SW_HIDE);
	}
}


LRESULT CCommandView::OnTemplateMatching( WPARAM wParam, LPARAM lParam )
{

	switch( wParam )
	{
	case MSG_CREATE_TEMPLATE:	// template 생성
		if( lParam== ITP_NCC )
		{
			m_pDoc->create_ncc_model();
		}
		else if( lParam== ITP_SHAPE_BASED )
		{
			m_pDoc->create_shape_model();
		}
		break;

	case MSG_FIND_TEMPLATE:	// template 찾기
		if( lParam== ITP_NCC )
		{
			m_pDoc->find_ncc_model();
		}
		else if( lParam== ITP_SHAPE_BASED )
		{
			m_pDoc->find_shape_model();
		}
		break;

	case MSG_SAVE_TEMPLATE:	// 저장
		if( lParam== ITP_NCC )
		{
			m_pDoc->save_ncc_model();
		}
		else if( lParam== ITP_SHAPE_BASED )
		{
			m_pDoc->save_shape_model();
		}
		break;

	case MSG_LOAD_TEMPLATE:	// 로딩
		if( lParam== ITP_NCC )
		{
			m_pDoc->load_ncc_model();
		}
		else if( lParam== ITP_SHAPE_BASED )
		{
			m_pDoc->load_shape_model();
		}
		break;

	default:
		break;
	}

	return 0;
}


LRESULT CCommandView::OnAlign( WPARAM wParam, LPARAM lParam )
{

	switch( wParam )
	{
	case MSG_ALIGN_CREATE:
		if( lParam== MSG_MAIN_MARK )
		{
			m_pDoc->create_main_mark();
		}
		if( lParam== MSG_SUB_MARK )
		{
			m_pDoc->create_sub_mark();
		}
		break;

	case MSG_ALIGN_FIND:
		if( lParam== MSG_MAIN_MARK_1 )
		{
			m_pDoc->find_main_mark(0);
		}
		if( lParam== MSG_MAIN_MARK_2 )
		{
			m_pDoc->find_main_mark(1);
		}
		if( lParam== MSG_SUB_MARK_1 )
		{
			m_pDoc->find_sub_mark(0);
		}
		if( lParam== MSG_SUB_MARK_2 )
		{
			m_pDoc->find_sub_mark(1);
		}
		break;

	case MSG_ALIGN_CALCULATE:
		m_pDoc->calc_align();
		break;

	default:
		break;
	}

	return 0;
}


LRESULT CCommandView::OnInspectPPA( WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case PPA_ABSOLUTE:
		m_pDoc->insp_ppa(lParam);
		break;

	case PPA_RELATIVE:
		m_pDoc->insp_offset();
		break;

	case PPA_POSITION:
		m_pDoc->insp_ppa_pos(lParam);
		break;

	default:
		break;
	}
	
	return 0;
}


LRESULT CCommandView::OnInspect_offset( WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case 0:
		m_pDoc->insp_offset();
		break;
	case 1:
		m_pDoc->insp_offset_n();
		break;
	default:
		break;
	}

	return 0;
}

LRESULT CCommandView::OnInspect_focus( WPARAM wParam, LPARAM lParam )
{

	m_pDoc->insp_focus();

	return 0;
}

void CCommandView::OnBnClickedCheckRoi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_pDoc->valid_Roi(m_chkRoi);
}


void CCommandView::OnBnClickedButtonTest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pDoc->check_result_data();
}


//void CCommandView::OnTcnSelchangeTabCommand(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	
//	*pResult = 0;
//}


void CCommandView::OnTcnSelchangeTabCommand(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int i = m_ctrlTab.GetCurSel();

	switch(i)
	{
		case PAGE_ALIGN:
			m_pPageAlign->ShowWindow(SW_SHOW);
			m_pageTemplate->ShowWindow(SW_HIDE);
			m_pPagePixel->ShowWindow(SW_HIDE);
			m_pPageMorphology->ShowWindow(SW_HIDE);
			m_pPageEtc->ShowWindow(SW_HIDE);
			break;

		case PAGE_TEMPLATE_MATCHING:
			m_pPageAlign->ShowWindow(SW_HIDE);
			m_pageTemplate->ShowWindow(SW_SHOW);
			m_pPagePixel->ShowWindow(SW_HIDE);
			m_pPageMorphology->ShowWindow(SW_HIDE);
			m_pPageEtc->ShowWindow(SW_HIDE);
			break;

		case PAGE_PIXEL:
			m_pPageAlign->ShowWindow(SW_HIDE);
			m_pageTemplate->ShowWindow(SW_HIDE);
			m_pPagePixel->ShowWindow(SW_SHOW);
			m_pPageMorphology->ShowWindow(SW_HIDE);
			m_pPageEtc->ShowWindow(SW_HIDE);
			break;

		case PAGE_MORPHOLOGY:
			m_pPageAlign->ShowWindow(SW_HIDE);
			m_pageTemplate->ShowWindow(SW_HIDE);
			m_pPagePixel->ShowWindow(SW_HIDE);
			m_pPageMorphology->ShowWindow(SW_SHOW);
			m_pPageEtc->ShowWindow(SW_HIDE);
			break;

		case PAGE_ETC:
			m_pPageAlign->ShowWindow(SW_HIDE);
			m_pageTemplate->ShowWindow(SW_HIDE);
			m_pPagePixel->ShowWindow(SW_HIDE);
			m_pPageMorphology->ShowWindow(SW_HIDE);
			m_pPageEtc->ShowWindow(SW_SHOW);
			break;

		default:
			break;
	}

	*pResult = 0;
}


void CCommandView::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pDoc->Test();

}


LRESULT CCommandView::OnThreshold( WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case MSG_THRESHOLD_MANUAL:
		m_pDoc->threshold(MSG_THRESHOLD_MANUAL, (int)lParam, 255 );
		break;

	case MSG_THRESHOLD_OTSU:
		m_pDoc->threshold(MSG_THRESHOLD_OTSU, 0, 0 );
		break;

	default:
		break;
	}

	return 0;
}

LRESULT CCommandView::OnSmooth( WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case MSG_SMOOTH_BLUR:
		m_pDoc->smooth(MSG_SMOOTH_BLUR);
		break;

	case MSG_SMOOTH_BOX:
		m_pDoc->smooth(MSG_SMOOTH_BOX);
		break;

	case MSG_SMOOTH_MEDIAN:
		m_pDoc->smooth(MSG_SMOOTH_MEDIAN);
		break;

	case MSG_SMOOTH_GAUSSIAN:
		m_pDoc->smooth(MSG_SMOOTH_GAUSSIAN);
		break;

	default:
		break;
	}

	return 0;
}


LRESULT CCommandView::OnDerivative( WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case MSG_DERIVATIVE_SOBEL:
		m_pDoc->derivative_filter(MSG_DERIVATIVE_SOBEL);
		break;

	case MSG_DERIVATIVE_SCHARR:
		m_pDoc->derivative_filter(MSG_DERIVATIVE_SCHARR);
		break;

	case MSG_DERIVATIVE_LAPLACE:
		m_pDoc->derivative_filter(MSG_DERIVATIVE_LAPLACE);
		break;

	case MSG_DERIVATIVE_PREWITT:
		m_pDoc->derivative_filter(MSG_DERIVATIVE_PREWITT);
		break;

	}

	return 0;
}


LRESULT CCommandView::OnEdge( WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case MSG_EDGE_CANNY:
		m_pDoc->edge(MSG_EDGE_CANNY, (int)lParam);
		break;

	}

	return 0;
}


LRESULT CCommandView::OnMorphology( WPARAM wParam, LPARAM lParam )
{
	m_pDoc->PerformMorphology( (int)wParam, (int)lParam );

	return 0;
}


//
// Pitch 비교 알고리즘 실행
//
LRESULT CCommandView::OnPitchCompare( WPARAM wParam, LPARAM lParam )
{
	m_pDoc->PerformPitchCompare( (int)wParam );

	return 0;
}


//
// Labeling 실행
//
LRESULT CCommandView::OnLabeling( WPARAM wParam, LPARAM lParam )
{
	m_pDoc->PerformLabeling();

	return 0;
}


//
// Labeling 실행
//
LRESULT CCommandView::OnInspectParticle( WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case 0:
		m_pDoc->InspectParticle(INSPECT_PARTICLE_GENERAL);
		break;

	case 1:
		m_pDoc->InspectParticle(INSPECT_PARTICLE_SIMULATION_1);
		break;

	case 2:
		m_pDoc->InspectParticle(INSPECT_PARTICLE_SIMULATION_N);
		break;

	}

	return 0;
}


void CCommandView::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	switch( m_libType )
	{
	case 0:
		m_pDoc->set_libray( LIB_C );
		return;

	case 1:
		m_pDoc->set_libray( LIB_HALCON );
		return;

	case 2:
		m_pDoc->set_libray( LIB_OPENCV );
		return;

	default:
		break;
	}
}


void CCommandView::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	switch( m_libType )
	{
	case 0:
		m_pDoc->set_libray( LIB_C );
		return;

	case 1:
		m_pDoc->set_libray( LIB_HALCON );
		return;

	case 2:
		m_pDoc->set_libray( LIB_OPENCV );
		return;

	default:
		break;
	}
}


void CCommandView::OnBnClickedRadio3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	switch( m_libType )
	{
	case 0:
		m_pDoc->set_libray( LIB_C );
		return;

	case 1:
		m_pDoc->set_libray( LIB_HALCON );
		return;

	case 2:
		m_pDoc->set_libray( LIB_OPENCV );
		return;

	default:
		break;
	}
}


void CCommandView::OnBnClickedCheckDispay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_pDoc->displayProcessingImage(m_bDisplayProcessingImage);
}
