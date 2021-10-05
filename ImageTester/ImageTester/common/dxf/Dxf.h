// Dxf.h: interface for the CDxf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DXF_H__148A5C6F_8B80_49A1_A007_2259EB5655F5__INCLUDED_)
#define AFX_DXF_H__148A5C6F_8B80_49A1_A007_2259EB5655F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "line.h"
#include "arc.h"
#include "Circle.h"	// Added by ClassView
#include "PolyLine.h"	// Added by ClassView
#include "Spline.h"	// Added by ClassView

class CDxf  
{
public:
	void Init();
	BYTE m_InitialDraw;
	BYTE m_LoadStatus;
	UINT m_LineCount;
	UINT m_ArcCount;
	UINT m_CircleCount;
	UINT m_PolyLineCount;
	UINT m_SplineCount;
	BYTE m_Nesting;

	CLine *m_Line;
	CArc *m_Arc;
	CCircle *m_Circle;
	CPolyLine *m_PolyLine;
	CSpline *m_Spline;

	CDxf();
	virtual ~CDxf();
	void LoadFile(CString);
	UINT GetLineCount();
	UINT GetCircleCount();
	UINT GetArcCount();
	UINT GetLwPolyLineCount();
	UINT GetSplineCount();
	void AllocMemory();
};

#endif // !defined(AFX_DXF_H__148A5C6F_8B80_49A1_A007_2259EB5655F5__INCLUDED_)
