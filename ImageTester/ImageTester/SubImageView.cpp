// SubImageView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTester.h"
#include "SubImageView.h"
#include "ImageTesterDoc.h"


// CSubImageView

IMPLEMENT_DYNCREATE(CSubImageView, CScrollView)

CSubImageView::CSubImageView()
{

}

CSubImageView::~CSubImageView()
{
}


BEGIN_MESSAGE_MAP(CSubImageView, CScrollView)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CSubImageView �׸����Դϴ�.

void CSubImageView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CSubImageView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	int nImage = m_pDoc->ImageList_GetNum();

// 	CBhImage img;
// 	img.Open("d:\\a.bmp");
// 	img.Draw( pDC->m_hDC, 10 , 10, 100, 100);

	if( nImage>0 )
	{
		for( int i=0 ; i<nImage ; i++ )
		{
			IT_IMAGE *pImage = m_pDoc->ImageList_GetImage(i);

			pImage->Draw( pDC->m_hDC, 10 + 110*i, 10, 100, 100);
		}
	}


	// ���õ� 
	int nSelected;
	if( m_pDoc->ImageList_GetSelectedIndex(&nSelected) )
	{
		CPen *pOldPen, redPen;
		redPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

		pOldPen = pDC->SelectObject(&redPen);

		HBRUSH hOldBrush, hBrush;
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, hBrush);

		pDC->Rectangle( SUBIMAGE_GAP + nSelected*(SUBIMAGE_GAP+SUBIMAGE_WIDTH), SUBIMAGE_GAP,
						SUBIMAGE_GAP + nSelected*(SUBIMAGE_GAP+SUBIMAGE_WIDTH) + SUBIMAGE_WIDTH, SUBIMAGE_GAP+SUBIMAGE_HEIGHT );

		pDC->SelectObject(pOldPen);
		redPen.DeleteObject();
		SelectObject(pDC->m_hDC, hOldBrush);
		DeleteObject(hBrush);
	}
}


// CSubImageView �����Դϴ�.

#ifdef _DEBUG
void CSubImageView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CSubImageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSubImageView �޽��� ó�����Դϴ�.
void CSubImageView::FillBmpInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin )
{
//	assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	memset( bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;

	if( bpp == 8 )
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for( i = 0; i < 256; i++ )
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}

int CSubImageView::CheckPosition(int x, int y)
{
	int iPitch = SUBIMAGE_GAP + SUBIMAGE_WIDTH;

	int iPos1 = x / iPitch;
	int iPos2 = x % iPitch;

	if( iPos2>SUBIMAGE_GAP )
	{
		return iPos1;
	}

	return SUBIMAGE_NONE;
}


void CSubImageView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	int iSelect = CheckPosition(point.x, point.y);

	if( iSelect!=SUBIMAGE_NONE )
	{
		m_pDoc->ImageList_Select(iSelect);
	}

	CScrollView::OnLButtonDown(nFlags, point);
}


BOOL CSubImageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes( MM_TEXT, sizeTotal );

	return CScrollView::PreCreateWindow(cs);
}
