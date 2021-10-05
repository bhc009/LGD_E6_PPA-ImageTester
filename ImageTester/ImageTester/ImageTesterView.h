
// ImageTesterView.h : CImageTesterView Ŭ������ �������̽�
//

#pragma once


class CImageTesterView : public CView
{
protected: // serialization������ ��������ϴ�.
	CImageTesterView();
	DECLARE_DYNCREATE(CImageTesterView)

// Ư���Դϴ�.
public:
	CImageTesterDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CImageTesterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ImageTesterView.cpp�� ����� ����
inline CImageTesterDoc* CImageTesterView::GetDocument() const
   { return reinterpret_cast<CImageTesterDoc*>(m_pDocument); }
#endif

