
// ImageTesterView.cpp : CImageTesterView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageTesterView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()

END_MESSAGE_MAP()

// CImageTesterView ����/�Ҹ�

CImageTesterView::CImageTesterView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CImageTesterView::~CImageTesterView()
{
}

BOOL CImageTesterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CImageTesterView �׸���

void CImageTesterView::OnDraw(CDC* /*pDC*/)
{
	CImageTesterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CImageTesterView �μ�


void CImageTesterView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageTesterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CImageTesterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CImageTesterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CImageTesterView ����

#ifdef _DEBUG
void CImageTesterView::AssertValid() const
{
	CView::AssertValid();
}

void CImageTesterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageTesterDoc* CImageTesterView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageTesterDoc)));
	return (CImageTesterDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageTesterView �޽��� ó����
