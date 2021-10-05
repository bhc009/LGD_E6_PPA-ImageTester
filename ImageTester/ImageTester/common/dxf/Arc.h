// Arc.h: interface for the CArc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARC_H__F3B30C5E_D3C4_4380_844A_03369FF97719__INCLUDED_)
#define AFX_ARC_H__F3B30C5E_D3C4_4380_844A_03369FF97719__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CArc  
{
public:
	double x,y,r,sa,ea;
	CString m_Layer;
	BYTE m_Select;
	BYTE m_Nest;

	CArc();
	virtual ~CArc();

};

#endif // !defined(AFX_ARC_H__F3B30C5E_D3C4_4380_844A_03369FF97719__INCLUDED_)
