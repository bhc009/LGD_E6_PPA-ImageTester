#pragma once

#define SUBIMAGE_GAP	10
#define SUBIMAGE_WIDTH	100
#define SUBIMAGE_HEIGHT	100
#define SUBIMAGE_NONE	-1

// CSubImageView 뷰입니다.
class CImageTesterDoc;
class CSubImageView : public CScrollView
{
	DECLARE_DYNCREATE(CSubImageView)

protected:
	CSubImageView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CSubImageView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	CImageTesterDoc *m_pDoc;
	void FillBmpInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin );

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

	int CheckPosition(int x, int y);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


