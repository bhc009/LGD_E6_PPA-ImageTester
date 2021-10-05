// Spline.h: interface for the CSpline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPLINE_H__E4B987FD_0798_424F_9D92_B5CA8CA37F17__INCLUDED_)
#define AFX_SPLINE_H__E4B987FD_0798_424F_9D92_B5CA8CA37F17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSpline  
{
public:
	CString m_Layer;
	UINT m_FitPointCount;
	double x[128],y[128];
	BYTE m_Select;
	BYTE m_Nest;

	CSpline();
	virtual ~CSpline();
};

#endif // !defined(AFX_SPLINE_H__E4B987FD_0798_424F_9D92_B5CA8CA37F17__INCLUDED_)
