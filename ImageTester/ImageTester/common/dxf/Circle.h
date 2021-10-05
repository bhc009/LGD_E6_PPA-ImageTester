// Circle.h: interface for the CCircle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIRCLE_H__B468D98B_7FA1_4A99_B55F_2A56932403A7__INCLUDED_)
#define AFX_CIRCLE_H__B468D98B_7FA1_4A99_B55F_2A56932403A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCircle  
{
public:
	double x,y,r;
	CString m_Layer;
	BYTE m_Select;
	BYTE m_Nest;

	CCircle();
	virtual ~CCircle();

};

#endif // !defined(AFX_CIRCLE_H__B468D98B_7FA1_4A99_B55F_2A56932403A7__INCLUDED_)
