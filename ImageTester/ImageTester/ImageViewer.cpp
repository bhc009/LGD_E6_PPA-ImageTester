// ImageViewer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTester.h"
#include "ImageViewer.h"

#include "ImageTesterDoc.h"

// CImageViewer

IMPLEMENT_DYNCREATE(CImageViewer, CScrollView)

CImageViewer::CImageViewer()
{
	m_pDoc = NULL;
// 	m_fZoom = 1.0;
	m_iZoom = 100;
	m_ptCenter = CPoint(0,0);

	m_ptLButtonDown = CPoint(0,0);
	m_ptLButtonUp = CPoint(0,0);
	m_ptMouse = CPoint(0,0);

	m_ptMouseImage = CPoint(0,0);

	m_bState_LButtonDown = FALSE;
}

CImageViewer::~CImageViewer()
{
}


BEGIN_MESSAGE_MAP(CImageViewer, CScrollView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CImageViewer 그리기입니다.

void CImageViewer::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	if( m_pDoc->get_image_buffer() )
	{
		sizeTotal.cx = m_pDoc->get_width();
		sizeTotal.cy = m_pDoc->get_height();
	} else {
		sizeTotal.cx = 100;
		sizeTotal.cy = 100;
	}

	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImageViewer::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
// 	CPoint pos = GetScrollPosition();

	CRect rtView;
	GetClientRect(&rtView);

	CPen *pOldPen=NULL;
	CPen colorPen[6];
	colorPen[0].CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	colorPen[1].CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	colorPen[2].CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	colorPen[3].CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	colorPen[4].CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	colorPen[5].CreatePen(PS_SOLID, 1, RGB(255, 255, 0));


	HBRUSH hOldBrush, hBrush;
	hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, hBrush);

	int i;

	if( m_pDoc->get_image_buffer() )
	{
		// 영상 표시
		int nWidth = m_pDoc->get_width();
		int nHeight = m_pDoc->get_height();
		CRect rect;

		GetClientRect(rect);	// View 영역 크기

		float width_all_view, height_all_view;
		Convert_image_to_view( (float)nWidth, &width_all_view );
		Convert_image_to_view( (float)nHeight, &height_all_view );

		m_pDoc->get_image()->Draw(pDC->m_hDC, 0, 0, (int)width_all_view, (int)height_all_view);


		// Draw line
		float x, y;
		for( i=0 ; i<m_pDoc->Get_line_num() ; i++ )
		{
			pOldPen = pDC->SelectObject(&colorPen[i%6]);

			list<FPoint>::iterator ite;
			if(  (m_pDoc->Get_line()+i)->size() >= 2)
			{
				FPoint pt = *((m_pDoc->Get_line()+i)->begin());
				Convert_image_to_view( pt.x+0.5f, pt.y+0.5f, &x, &y );
				pDC->MoveTo( (int)x , (int)y );

				for( ite = (m_pDoc->Get_line()+i)->begin() ; ite!= (m_pDoc->Get_line()+i)->end() ; ite++ )
				{
					Convert_image_to_view( (ite->x)+0.5f, (ite->y)+0.5f, &x, &y );
					pDC->LineTo((int)x , (int)y );
				}
			}
		}

		// Draw circle
		i=0;
		for( list<CIRCLED>::iterator ite = m_pDoc->get_display_circle()->begin() ; ite!=m_pDoc->get_display_circle()->end() ; ite++ )
		{
			pOldPen = pDC->SelectObject(&colorPen[(i++)%6]);
			

			float left, top, right, bottom;
			Convert_image_to_view( (float)(ite->cx - ite->radius), (float)(ite->cy - ite->radius), &left, &top );
			Convert_image_to_view( (float)(ite->cx + ite->radius), (float)(ite->cy + ite->radius), &right, &bottom );

			pDC->Ellipse( (int)left, (int)top, (int)right, (int)bottom );
		}


		//
		// Draw ROI
		//
		pOldPen = pDC->SelectObject(&colorPen[0]);
		CRect rtRoi;
		m_pDoc->GetROI(&rtRoi);
		float roi_left, roi_right, roi_top, roi_bottom;
		Convert_image_to_view( rtRoi.left + 0.5f, rtRoi.top + 0.5f, &roi_left, &roi_top );
		Convert_image_to_view( rtRoi.right + 0.5f, rtRoi.bottom + 0.5f, &roi_right, &roi_bottom );
		pDC->Rectangle( (int)roi_left, (int)roi_top, (int)roi_right, (int)roi_bottom );


		//
		// Draw object
		//
		int dist = 25;
		for( list<DEFECT_INFO>::iterator ite = m_pDoc->GetDefect()->begin() ; ite!=m_pDoc->GetDefect()->end() ; ite++ )
		{
			pDC->Rectangle( ite->x - ite->iWidth/2 - dist, 
							ite->y - ite->iHeight/2 - dist, 
							ite->x + ite->iWidth/2 + dist, 
							ite->y + ite->iHeight/2 + dist );
		}
// 		CBrush tmpBrush;
// 		tmpBrush.CreateSolidBrush(RGB(255,0,0));
// 		CRgn tmpRgn;
// 		tmpRgn.CreateRectRgn( 100, 100, 300, 300 );
// 		pDC->FillRgn( &tmpRgn, &tmpBrush );
	}

	// Draw rectangle
	if( m_bState_LButtonDown )
	{
		pOldPen = pDC->SelectObject(&colorPen[1]);

		pDC->Rectangle( m_ptLButtonDown.x, m_ptLButtonDown.y, m_ptMouse.x, m_ptMouse.y );
	}


	if( pOldPen )
	{
		pDC->SelectObject(pOldPen);
	}
	for( i=0 ; i<6 ; i++)
	{
		colorPen[i].DeleteObject();
	}

	SelectObject(pDC->m_hDC, hOldBrush);
	DeleteObject(hBrush);
	
}


// CImageViewer 진단입니다.

#ifdef _DEBUG
void CImageViewer::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CImageViewer::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CImageViewer 메시지 처리기입니다.
void CImageViewer::FillBmpInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin )
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

void CImageViewer::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pDoc->SetDisplayModeFlip();

	CScrollView::OnLButtonDblClk(nFlags, point);
}


void CImageViewer::ResizeView()
{
	CSize sizeTotal;
	int xPos, yPos;

	if( m_pDoc->get_image_buffer() )
	{
		// 전체 크기 = 이미지크기 x 줌배율;
		float width_total_View;
		float height_total_View;
		float x_center_view;
		float y_center_view;

		Convert_image_to_view( (float)m_pDoc->get_width(), (float)m_pDoc->get_height(), &width_total_View, &height_total_View );

		// 영상을 표시할 전체 영역의 크기
		sizeTotal.cx = (LONG)width_total_View;
		sizeTotal.cy = (LONG)height_total_View;


		// View 영역의 크기
		CRect rect_roi_view;
		GetClientRect(rect_roi_view);

// 		Convert_image_to_view( (float)m_ptCenter.x, (float)m_ptCenter.y, &x_center_view, &y_center_view );
		Convert_image_to_view( (float)m_ptMouseImage.x, (float)m_ptMouseImage.y, &x_center_view, &y_center_view );



		// 
		if( width_total_View<rect_roi_view.Width() )
		{
			xPos = 0;
		} else {
			if( x_center_view + rect_roi_view.Width()/2 >= width_total_View )
			{
				xPos = max( 0, (int)(width_total_View - rect_roi_view.Width() - 1) );

			} else {
				xPos = max( 0, (int)(x_center_view - rect_roi_view.Width()/2) );
			}
		}

		if( height_total_View<rect_roi_view.Height() )
		{
			yPos = 0;
		} else {

			if( y_center_view + rect_roi_view.Height()/2 >= height_total_View )
			{
				yPos = max( 0, (int)(height_total_View - rect_roi_view.Height() - 1) );

			} else {
				yPos = max( 0, (int)(y_center_view - rect_roi_view.Height()/2) );
			}
		}
	} else {
		sizeTotal.cx = 100;
		sizeTotal.cy = 100;

		xPos = 0;
		yPos = 0;
	}
	
	SetScrollSizes(MM_TEXT, sizeTotal);

 	SetScrollPos(SB_HORZ, xPos, TRUE);
 	SetScrollPos(SB_VERT, yPos, TRUE);

	Invalidate(TRUE);
}


void CImageViewer::Zoom(ZOOM_FACTOR i)
{
	switch( i )
	{
	case ZOOM_IN_:
		if( m_iZoom>=1000 )	// 1000%까지로 제한
		{
			// Nothing
		}
		else if( m_iZoom>=100 )
		{
			m_iZoom += 100;
		} else if( m_iZoom>=25 ) 
		{
			m_iZoom += 25;
		}

		ResizeView();

		break;

	case ZOOM_OUT_:
		if( m_iZoom>100 )
		{
			m_iZoom -= 100;
		} else if( m_iZoom>25 ) 
		{
			m_iZoom -= 25;
		}

		ResizeView();

		break;
	}


	Invalidate(FALSE);
}


BOOL CImageViewer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if( zDelta>0 )
	{
		Zoom(ZOOM_IN_);
	} else {
		Zoom(ZOOM_OUT_);
	}


	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CImageViewer::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 다이얼로그 최대화 시 OnInitialUpdate() 보다 먼저 호출되어 문제가 발생함
	// 일단, 문제가 없도록 수정

	if( m_pDoc )
	{
		ResizeView();
	} else {
		CSize sizeTotal;

		sizeTotal.cx = 100;
		sizeTotal.cy = 100;

		SetScrollSizes(MM_TEXT, sizeTotal);
	}

}


void CImageViewer::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint scroll_pos = GetScrollPosition();

	m_ptLButtonDown = CPoint( scroll_pos.x + point.x, scroll_pos.y+point.y );

	m_bState_LButtonDown = TRUE;

	CScrollView::OnLButtonDown(nFlags, point);
}


void CImageViewer::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint scroll_pos = GetScrollPosition();

	m_ptLButtonUp = CPoint( scroll_pos.x + point.x, scroll_pos.y+point.y );

	m_bState_LButtonDown = FALSE;

	float roi_left, roi_right, roi_top, roi_bottom;
	Convert_view_to_image( (float)m_ptLButtonDown.x, (float)m_ptLButtonDown.y, &roi_left, &roi_top );
	Convert_view_to_image( (float)m_ptLButtonUp.x, (float)m_ptLButtonUp.y, &roi_right, &roi_bottom );

	m_pDoc->SetROI( CRect( (int)min(roi_left, roi_right), (int)min(roi_top, roi_bottom), (int)max(roi_left, roi_right), (int)max(roi_top, roi_bottom) ) );

	CScrollView::OnLButtonUp(nFlags, point);
}


void CImageViewer::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint scroll_pos = GetScrollPosition();

	m_ptMouse = CPoint( scroll_pos.x + point.x, scroll_pos.y+point.y );


	// 화면 중심부의 영상 좌표 계산
	CRect rect_roi_view;
	GetClientRect(rect_roi_view);

	float xImage=0, yImage=0;
	Convert_view_to_image( scroll_pos.x + rect_roi_view.Width()/2, scroll_pos.y+rect_roi_view.Height()/2, &xImage, &yImage );
	m_ptMouseImage = CPoint( xImage, yImage );


	//
	if( m_pDoc->get_image_buffer() )
	{
		float x_image, y_image;
		Convert_view_to_image( (float)m_ptMouse.x, (float)m_ptMouse.y, &x_image, &y_image );

		int ix_image = (int)x_image;
		int iy_image = (int)y_image;

		if( ix_image>=0 && ix_image<m_pDoc->get_width() && iy_image>=0 && iy_image<m_pDoc->get_height())
		{
			BYTE gray = m_pDoc->get_image_buffer()[ (int)x_image + (int)y_image*m_pDoc->get_width_step() ];

			CDC *pDC = GetDC();
			CString str;
			str.Format("(%04d, %04d, %03d)", ix_image, iy_image, gray );
			pDC->TextOut(0, 0, str );
		}
	}

	if( m_bState_LButtonDown )
	{
		Invalidate(FALSE);
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CImageViewer::Convert_image_to_view( float x_in, float y_in, float *x_out, float *y_out )
{
	*x_out = x_in * m_iZoom *0.01f;
	*y_out = y_in * m_iZoom *0.01f;
}

void CImageViewer::Convert_view_to_image( float x_in, float y_in, float *x_out, float *y_out )
{
	*x_out = x_in * 100 / m_iZoom;
	*y_out = y_in * 100 / m_iZoom;
}

void CImageViewer::Convert_image_to_view( float length_in, float *length_out )
{
	*length_out = length_in * m_iZoom *0.01f;
}

void CImageViewer::Convert_view_to_image( float length_in, float *length_out )
{
	*length_out = length_in * 100 / m_iZoom;
}


BOOL CImageViewer::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes( MM_TEXT, sizeTotal );

	return CScrollView::PreCreateWindow(cs);
}
