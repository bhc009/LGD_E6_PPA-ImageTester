
// ImageTesterView.cpp : CImageTesterView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageTester.h"
#endif

#include "ImageTesterDoc.h"
#include "ImageTesterView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageTesterView

IMPLEMENT_DYNCREATE(CImageTesterView, CView)

BEGIN_MESSAGE_MAP(CImageTesterView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageTesterView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()

END_MESSAGE_MAP()

// CImageTesterView 생성/소멸

CImageTesterView::CImageTesterView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CImageTesterView::~CImageTesterView()
{
}

BOOL CImageTesterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CImageTesterView 그리기

void CImageTesterView::OnDraw(CDC* /*pDC*/)
{
	CImageTesterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CImageTesterView 인쇄


void CImageTesterView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageTesterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CImageTesterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CImageTesterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CImageTesterView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageTesterView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageTesterView 진단

#ifdef _DEBUG
void CImageTesterView::AssertValid() const
{
	CView::AssertValid();
}

void CImageTesterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageTesterDoc* CImageTesterView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageTesterDoc)));
	return (CImageTesterDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageTesterView 메시지 처리기
