#pragma once

#define SUBIMAGE_GAP	10
#define SUBIMAGE_WIDTH	100
#define SUBIMAGE_HEIGHT	100
#define SUBIMAGE_NONE	-1

// CSubImageView ���Դϴ�.
class CImageTesterDoc;
class CSubImageView : public CScrollView
{
	DECLARE_DYNCREATE(CSubImageView)

protected:
	CSubImageView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual void OnInitialUpdate();     // ������ �� ó���Դϴ�.

	int CheckPosition(int x, int y);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


