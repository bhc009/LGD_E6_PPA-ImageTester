// Dxf.cpp: implementation of the CDxf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dxf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDxf::CDxf()
{
	m_LoadStatus = FALSE;
	m_InitialDraw = FALSE;
	m_LineCount = 0;
	m_CircleCount = 0;
	m_ArcCount = 0;
	m_PolyLineCount = 0;
	m_SplineCount = 0;
	m_Nesting = FALSE;

	m_Line = NULL;
	m_Arc = NULL;
	m_Circle = NULL;
	m_PolyLine = NULL;
	m_Spline = NULL;

}

CDxf::~CDxf()
{
	if( m_Line )
	{
		delete[] m_Line;
		m_Line = NULL;
	}

	if( m_Arc )
	{
		delete[] m_Arc;
		m_Arc = NULL;
	}

	if( m_Circle )
	{
		delete[] m_Circle;
		m_Circle = NULL;
	}

	if( m_PolyLine )
	{
		delete[] m_PolyLine;
		m_PolyLine = NULL;
	}

	if( m_Spline )
	{
		delete[] m_Spline;
		m_Spline = NULL;
	}
}

void CDxf::LoadFile(CString str)
{
	FILE *fp;

	fp = fopen(str,"r");
	if(fp != NULL)
	{
		char lpCode[256], lpValue[256];
		
		while(!feof(fp))
		{
			fscanf(fp,"%s %s", &lpCode, &lpValue);

			if
			(
				strcmp(lpCode,"0") == 0 &&
				strcmp(lpValue,"LINE") == 0
			)
			{
				m_LineCount++;
			}
			//Lwpolyline command.
			else if
			(
				strcmp(lpCode,"0") == 0 &&
				strcmp(lpValue,"LWPOLYLINE") == 0
			)
			{
				m_PolyLineCount++;
			}
			//Circle command.
			else if
			(
				strcmp(lpCode,"0") == 0 &&
				strcmp(lpValue,"CIRCLE") == 0
			)
			{
				m_CircleCount++;
			}
			//Arc command.
			else if
			(
				strcmp(lpCode,"0") == 0 &&
				strcmp(lpValue,"ARC") == 0
			)
			{
				m_ArcCount++;
			}
			else if
			(
				strcmp(lpCode,"0") == 0 &&
				strcmp(lpValue,"SPLINE") == 0
			)
			{
				m_SplineCount++;
			}
		}

		fclose(fp);
		AllocMemory();
	}

	m_LineCount = 0;
	m_CircleCount = 0;
	m_ArcCount = 0;
	m_PolyLineCount = 0;
	m_SplineCount = 0;

	fp = fopen(str,"r");
	if(fp != NULL)
	{
		m_LoadStatus = TRUE;

		char lpCode[256], lpValue[256];
		
		while(!feof(fp))
		{
			fscanf(fp,"%s %s", &lpCode, &lpValue);

			if
			(
				strcmp(lpCode,"0") == 0 &&
				strcmp(lpValue,"LINE") == 0
			)
			{
				while(strcmp(lpCode,"8") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"8") == 0)
				{
					m_Line[m_LineCount].m_Layer = lpValue;
				}

				while(strcmp(lpCode,"10") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"10") == 0)
				{
					m_Line[m_LineCount].x1 = atof(lpValue);
				}

				while(strcmp(lpCode,"20") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"20") == 0)
				{
					m_Line[m_LineCount].y1 = atof(lpValue);
				}

				while(strcmp(lpCode,"11") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"11") == 0)
				{
					m_Line[m_LineCount].x2 = atof(lpValue);
				}

				while(strcmp(lpCode,"21") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"21") == 0)
				{
					m_Line[m_LineCount].y2 = atof(lpValue);
				}

				m_LineCount++;
			}
			//Lwpolyline command.
			else if
			(
				strcmp(lpCode,"0") == 0 &&
				strcmp(lpValue,"LWPOLYLINE") == 0
			)
			{
				while(strcmp(lpCode,"8") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"8") == 0)
				{
					m_PolyLine[m_PolyLineCount].m_Layer = lpValue;
				}

				while(strcmp(lpCode,"90") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"90") == 0)
				{
					m_PolyLine[m_PolyLineCount].m_VertexCount = atoi(lpValue);
				}

				while(strcmp(lpCode,"70") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"70") == 0)
				{
					m_PolyLine[m_PolyLineCount].m_PolyLineFlag = atoi(lpValue);
				}

				for(UINT i = 0; i < m_PolyLine[m_PolyLineCount].m_VertexCount; i++)
				{
					while(strcmp(lpCode,"10") != 0 && !feof(fp))
					{
						fscanf(fp,"%s %s", &lpCode, &lpValue);
					}

					if(strcmp(lpCode,"10") == 0)
					{
						m_PolyLine[m_PolyLineCount].x[i] = atof(lpValue);
					}

					while(strcmp(lpCode,"20") != 0 && !feof(fp))
					{
						fscanf(fp,"%s %s", &lpCode, &lpValue);
					}

					if(strcmp(lpCode,"20") == 0)
					{
						m_PolyLine[m_PolyLineCount].y[i] = atof(lpValue);
					}
				}

				m_PolyLineCount++;
			}
			//Circle command.
			else if
			(
				strcmp(lpCode,"0") == 0 &&
				strcmp(lpValue,"CIRCLE") == 0
			)
			{
				while(strcmp(lpCode,"8") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"8") == 0)
				{
					m_Circle[m_CircleCount].m_Layer = lpValue;
				}

				while(strcmp(lpCode,"10") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"10") == 0)
				{
					m_Circle[m_CircleCount].x = atof(lpValue);
				}

				while(strcmp(lpCode,"20") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"20") == 0)
				{
					m_Circle[m_CircleCount].y = atof(lpValue);
				}

				while(strcmp(lpCode,"40") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"40") == 0)
				{
					m_Circle[m_CircleCount].r = atof(lpValue);
				}

				m_CircleCount++;
			}
			//Arc command.
			else if
			(
				strcmp(lpCode,"0") == 0 &&
				strcmp(lpValue,"ARC") == 0
			)
			{
				while(strcmp(lpCode,"8") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"8") == 0)
				{
					m_Arc[m_ArcCount].m_Layer = lpValue;
				}

				while(strcmp(lpCode,"10") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"10") == 0)
				{
					m_Arc[m_ArcCount].x = atof(lpValue);
				}

				while(strcmp(lpCode,"20") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"20") == 0)
				{
					m_Arc[m_ArcCount].y = atof(lpValue);
				}

				while(strcmp(lpCode,"40") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"40") == 0)
				{
					m_Arc[m_ArcCount].r = atof(lpValue);
				}

				while(strcmp(lpCode,"50") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"50") == 0)
				{
					m_Arc[m_ArcCount].sa = atof(lpValue);
				}

				while(strcmp(lpCode,"51") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"51") == 0)
				{
					m_Arc[m_ArcCount].ea = atof(lpValue);
				}

				m_ArcCount++;
			}
			else if
			(
				strcmp(lpCode,"0") == 0 &&
				strcmp(lpValue,"SPLINE") == 0
			)
			{
				while(strcmp(lpCode,"8") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"8") == 0)
				{
					m_Spline[m_SplineCount].m_Layer = lpValue;
				}

				while(strcmp(lpCode,"74") != 0 && !feof(fp))
				{
					fscanf(fp,"%s %s", &lpCode, &lpValue);
				}

				if(strcmp(lpCode,"74") == 0)
				{
					m_Spline[m_SplineCount].m_FitPointCount = atoi(lpValue);
				}

				for(UINT i = 0; i < m_Spline[m_SplineCount].m_FitPointCount; i++)
				{
					while(strcmp(lpCode,"11") != 0 && !feof(fp))
					{
						fscanf(fp,"%s %s", &lpCode, &lpValue);
					}

					if(strcmp(lpCode,"11") == 0)
					{
						m_Spline[m_SplineCount].x[i] = atof(lpValue);
					}

					while(strcmp(lpCode,"21") != 0 && !feof(fp))
					{
						fscanf(fp,"%s %s", &lpCode, &lpValue);
					}

					if(strcmp(lpCode,"21") == 0)
					{
						m_Spline[m_SplineCount].y[i] = atof(lpValue);
					}
				}

				m_SplineCount++;
			}
		}
	}
	else
	{
		m_LoadStatus = FALSE;
	}
}

UINT CDxf::GetLineCount()
{
	return m_LineCount;
}

UINT CDxf::GetCircleCount()
{
	return m_CircleCount;
}

UINT CDxf::GetArcCount()
{
	return m_ArcCount;
}

UINT CDxf::GetLwPolyLineCount()
{
	return m_PolyLineCount;
}

UINT CDxf::GetSplineCount()
{
	return m_SplineCount;
}

void CDxf::AllocMemory()
{
	if( m_Line )
	{
		delete[] m_Line;
	}
	m_Line = new CLine[m_LineCount];

	if( m_Arc )
	{
		delete[] m_Arc;
	}
	m_Arc = new CArc[m_ArcCount];

	if( m_Circle )
	{
		delete[] m_Circle;
	}
	m_Circle = new CCircle[m_CircleCount];

	if( m_PolyLine )
	{
		delete[] m_PolyLine;
	}
	m_PolyLine = new CPolyLine[m_PolyLineCount];

	if( m_Spline )
	{
		delete[] m_Spline;
	}
	m_Spline = new CSpline[m_SplineCount];
}

void CDxf::Init()
{
	m_LoadStatus = FALSE;
	m_InitialDraw = FALSE;
	m_LineCount = 0;
	m_CircleCount = 0;
	m_ArcCount = 0;
	m_PolyLineCount = 0;
	m_SplineCount = 0;
	m_Nesting = FALSE;
}
