// Circle.cpp: implementation of the CCircle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// #include "DXFNester.h"
#include "Circle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCircle::CCircle()
{
	m_Layer = "0";
	m_Select = FALSE;
	m_Nest = FALSE;
}

CCircle::~CCircle()
{

}
