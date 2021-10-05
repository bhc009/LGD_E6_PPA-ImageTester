// Line.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_H__57D9AECB_3DF9_42D3_BC4B_789EA9B64D68__INCLUDED_)
#define AFX_LINE_H__57D9AECB_3DF9_42D3_BC4B_789EA9B64D68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLine  
{
public:
	double x1,y1,x2,y2;
	CString m_Layer;
	BYTE m_Select;
	BYTE m_Nest;

	CLine();
	virtual ~CLine();
};

#endif // !defined(AFX_LINE_H__57D9AECB_3DF9_42D3_BC4B_789EA9B64D68__INCLUDED_)
