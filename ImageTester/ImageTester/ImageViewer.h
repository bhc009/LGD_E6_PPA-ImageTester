#pragma once

typedef enum Zoom_Factor 
{
	ZOOM_IN_,	// SHOW ORIGINAL IMAGE
	ZOOM_OUT_	// SHOW ORIGINAL IMAGE
}ZOOM_FACTOR;

// CImageViewer 뷰입니다.
class CImageTesterDoc;

class CImageViewer : public CScrollView
{
	DECLARE_DYNCREATE(CImageViewer)

protected:
	CImageViewer();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CImageViewer();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	CImageTesterDoc *m_pDoc;

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

	void FillBmpInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin );

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void ResizeView();
	void Zoom(ZOOM_FACTOR i);
// 	CPoint ConvertToImagePoint( CPoint pt );
// 	CPoint ConvertToViewPoint( CPoint pt );
	void Convert_image_to_view( float x_in, float y_in, float *x_out, float *y_out );
	void Convert_view_to_image( float x_in, float y_in, float *x_out, float *y_out );
	void Convert_image_to_view( float length_in, float *length_out );
	void Convert_view_to_image( float length_in, float *length_out );

protected:
// 	double m_fZoom;
	int m_iZoom;
	CPoint m_ptCenter;

	CPoint m_ptLButtonDown;
	CPoint m_ptLButtonUp;
	CPoint m_ptMouse;
	CPoint m_ptMouseImage;
	BOOL m_bState_LButtonDown;

public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


