// PolyLine.h: interface for the CPolyLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYLINE_H__2845A691_54B0_40CE_9000_80D63B888975__INCLUDED_)
#define AFX_POLYLINE_H__2845A691_54B0_40CE_9000_80D63B888975__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPolyLine  
{
public:
	UINT m_VertexCount,m_PolyLineFlag;
	double x[1024],y[1024];
	CString m_Layer;
	BYTE m_Select;
	BYTE m_Nest;

	CPolyLine();
	virtual ~CPolyLine();

};

#endif // !defined(AFX_POLYLINE_H__2845A691_54B0_40CE_9000_80D63B888975__INCLUDED_)
