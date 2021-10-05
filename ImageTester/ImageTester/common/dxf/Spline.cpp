// Spline.cpp: implementation of the CSpline class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// #include "DXFNester.h"
#include "Spline.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpline::CSpline()
{
	m_FitPointCount = 0;
	m_Layer = "0";
	m_Select = FALSE;
	m_Nest = FALSE;
}

CSpline::~CSpline()
{

}
