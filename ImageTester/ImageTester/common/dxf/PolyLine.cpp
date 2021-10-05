// PolyLine.cpp: implementation of the CPolyLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// #include "DXFNester.h"
#include "PolyLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPolyLine::CPolyLine()
{
	m_VertexCount = 0;
	m_PolyLineFlag = 0;
	m_Layer = "0";
	m_Select = FALSE;
	m_Nest = FALSE;
}

CPolyLine::~CPolyLine()
{

}
