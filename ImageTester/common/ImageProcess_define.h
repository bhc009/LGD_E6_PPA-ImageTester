#pragma once

#include <list>
using namespace std;

#ifndef _FLOAT_POINT_
	struct FPoint
	{
		float x;
		float y;
	};
	#define _FLOAT_POINT_
#endif


//
//
//
//
struct OFFSET_FMM
{
	double mask_x;
	double mask_y;
	double glass_x;
	double glass_y;

	OFFSET_FMM()
	{
		mask_x = 0.0;
		mask_y = 0.0;
		glass_x = 0.0;
		glass_y = 0.0;
	}
};

struct DRect
{
	double sx;
	double sy;
	double ex;
	double ey;
};


//////////////////////////////////////////////////////////////////////////
//
// 이물검사기용 parameter
//
//////////////////////////////////////////////////////////////////////////

// 검사 대상체 정보
struct PARAM_FMM_SURFACE_INSPECTIOM
{
	double x_resolution;		// Resolution [mm/pixel]
	double y_resolution;		// Resolution [mm/pixel]

	unsigned int uiPitch;		// pitch [pixel]
	double xRealPitch;			// pitch [mm]
	double yRealPitch;			// pitch [mm]

	unsigned int uiDarkThreshold;	// threshold
	unsigned int uiBrightThreshold;	// threshold
	double dMinSize;	// threshold

	unsigned int uiOverlapLeft;	// 왼쪽 overlap 영역
	unsigned int uiOverlapRight;// 오른쪽 overlap 영역

	double xRealMergeRange;	// merge range [mm]
	double yRealMergeRange;	// merge range [mm]

	PARAM_FMM_SURFACE_INSPECTIOM()
	{
		uiPitch		= 62;
		uiDarkThreshold = 50;
		uiBrightThreshold = 50;

		x_resolution = 0.0015;	// 1.5 um/pixel
		y_resolution = 0.0015;	// 1.5 um/pixel
		xRealPitch = 0.040;		// pitch [mm]
		yRealPitch = 0.040;		// pitch [mm]

		uiOverlapLeft = 0;
		uiOverlapRight = 0;

		xRealMergeRange = 1.0;
		yRealMergeRange = 1.0;

		dMinSize = 1;
	}
};

// 검사 영역 정보
struct ACTIVE_REGIONS
{
	list<DRect> RectList;	// 사각형 active 영역


	// active 영역 clear
	void clear(  )
	{
		RectList.clear();
	}


	// active 영역 추가
	void addRegion( DRect region )
	{
		RectList.push_back(region);
	}


	// active 영역 여부 점검
	BOOL checkIn( double x, double y )
	{
		for( list<DRect>::iterator ite = RectList.begin() ; ite!=RectList.end() ; ite++ )
		{
			if( x>=ite->sx && x<=ite->ex && y>=ite->sy && y<=ite->ey )
			{
				return TRUE;
			}
		}

		return FALSE;
	};
};

// 불량 정보
struct DEFECT_INFO 
{
	int x;		// 영상좌표
	int y;		// 영상좌표
	int iWidth;	// 폭(pixel)
	int iHeight;// 높이(pixel)
	int iArea;	// 높이(pixel)

	double dRealX;		// 도면좌표(mm)
	double dRealY;		// 도면좌표(mm)
	double dRealArea;	// 면적(mm)
	double dRealWidth;	// 면적(mm)
	double dRealHeight;	// 면적(mm)

	int iBrightness;	// 0:dark, 1:bright
	int iStrength;		//
	int iMaxStrength;	//

	char cFilePath[999];

	DEFECT_INFO()
	{
		x	= 0;		// 영상좌표
		y	= 0;		// 영상좌표
		iWidth	= 0;	// 폭(pixel)
		iHeight	= 0;// 높이(pixel)
		iArea	= 0;	// 높이(pixel)

		dRealX	= 0;		// 도면좌표(mm)
		dRealY	= 0;		// 도면좌표(mm)
		dRealArea	= 0;	// 면적(mm)
		dRealWidth	= 0;	// 면적(mm)
		dRealHeight	= 0;	// 면적(mm)

		iBrightness	= 0;	// 0:dark, 1:bright
		iStrength	= 0;		//
		iMaxStrength	= 0;	//

		memset(cFilePath, 0, 999);
	}
};


typedef enum threshold_method 
{
	MANUAL_METHOD,
	OTSU_METHOD
} THRESHOLD_METHOD;




#ifndef _POINT_SHAPE_MODEL_
	struct pointShape
	{
		float x;
		float y;
		int dx;
		int dy;
	};
	#define _POINT_SHAPE_MODEL_
#endif

#ifndef _IP_NCC_MODEL_
	struct NCC_MODEL
	{	
		NCC_MODEL()
		{
			pTemplateImage = NULL;
			pValidImage = NULL;
			width = 0;
			height = 0;
			average = 0;
			sigma = 0;
			cx = 0;
			cy = 0;
		}

		void create( int width1, int height1 )
		{
			width = width1;
			height = height1;

			if( pTemplateImage )
			{
				delete[] pTemplateImage;
			}
			pTemplateImage = new BYTE[width*height];
			memset( pTemplateImage, 0, width*height);
		}

		void make_valid_map( int lowThreshold, int highThreshold )
		{
			if( pTemplateImage==NULL )
			{
				return;
			}

			if( pValidImage )
			{
				delete[] pValidImage;
			}
			pValidImage = new BYTE[width*height];

			for( int i=0 ; i<width*height ; i++ )
			{
				if( pTemplateImage[i]<lowThreshold || pTemplateImage[i]>highThreshold )
				{
					pValidImage[i]=0;	// invalid
				} else {
					pValidImage[i]=1;	// valid
				}
			}
		}

		BYTE *pTemplateImage;
		BYTE *pValidImage;
		int width;
		int height;

		float average;
		float sigma;

		float cx;
		float cy;
	};
	#define _IP_NCC_MODEL_
#endif

#define _IP_TAYLOR_	0
#define _IP_FACET_	1
#define _IP_PPA_	999

#define _IP_DARK_	0
#define _IP_BRIGHT_	1


// edge direction
#define DIR_0	0
#define DIR_30	1
#define DIR_60	2
#define DIR_90	3
#define DIR_120	4
#define DIR_150	5
#define DIR_180	6
#define DIR_210	7
#define DIR_240	8
#define DIR_270	9
#define DIR_300	10
#define DIR_330	11


#define _IP_PI_ 3.14159265359

//
#ifndef MIN
#define MIN(X,Y) (((X)>(Y))?(Y):(X))
#endif

#ifndef MAX
#define MAX(X,Y) (((X)>(Y))?(X):(Y))
#endif

#ifndef POINT_XY
#define POINT_XY(img,iWidth,x,y)	img[ (x) + (y)*(iWidth) ]
#endif

#ifndef RANGE_LIMIT
#define RANGE_LIMIT(v_min, v, v_max)	MAX( MIN( v, v_max), v_min )
#endif


//////////////////////////////////////////////////////////////////////////
//
// PPA
//
//////////////////////////////////////////////////////////////////////////
// 불량 정보

// Mask내 구멍 형상
typedef enum hole_shape 
{
	HOLE_SHAPE_RECTANGLE,
	HOLE_SHAPE_RECTANGLE_45,
	HOLE_SHAPE_DIAMOND,
	HOLE_SHAPE_CIRCLE,
	HOLE_SHAPE_ELLIPSE,
	HOLE_SHAPE_ELLIPSE_45,
} HOLE_SHAPE;

// Mask내 구멍 형상
typedef enum hole_arrange 
{
	HOLE_ARRANGE_RECTANGLE,
	HOLE_ARRANGE_CROSS,
} HOLE_ARRANGE;

// Mask내 구멍 형상
typedef enum hole_measure_method
{
	MEASURE_METHOD_EDGE,
	MEASURE_METHOD_PAIR,
} HOLE_MEASURE_METHOD;

struct MASK_INFO 
{
	HOLE_SHAPE eHoleShape;	// 구멍 형상
	HOLE_ARRANGE eHoleArrange;
	HOLE_MEASURE_METHOD eHoleMeasureMethod;

	double dRealPitchX;		// Pitch(um)
	double dRealPitchY;		// Pitch(um) 
	double dRealHoleWidth;	// 구멍 크기(um)
	double dRealHoleHeight;	// 구멍 크기(um)

	double dResolution;	// um/pixel

	MASK_INFO()
	{
		eHoleShape = HOLE_SHAPE_RECTANGLE;
		eHoleArrange = HOLE_ARRANGE_RECTANGLE;

		dRealPitchX	= 70;
		dRealPitchY	= 70;
		dRealHoleWidth	= 40;
		dRealHoleHeight	= 40;

		dResolution = 0.345;	
	}
};

struct PPA_INFO 
{
	double dPpaXAvg;		// Pitch(um)
	double dPpaXMin;		// Pitch(um)
	double dPpaXMax;		// Pitch(um)
	double dPpaXStd;		// Pitch(um)

	double dPpaYAvg;		// Pitch(um) 
	double dPpaYMin;		// Pitch(um) 
	double dPpaYMax;		// Pitch(um) 
	double dPpaYStd;		// Pitch(um) 

	double dCdWidthAvg;	// 구멍 크기(um)
	double dCdWidthMin;	// 구멍 크기(um)
	double dCdWidthMax;	// 구멍 크기(um)
	double dCdWidthStd;	// 구멍 크기(um)

	double dCdHeightAvg;	// 구멍 크기(um)
	double dCdHeightMin;	// 구멍 크기(um)
	double dCdHeightMax;	// 구멍 크기(um)
	double dCdHeightStd;	// 구멍 크기(um)

	int nPoint;
	double dPpaX[25];
	double dPpaY[25];
	double dCdWidth[25];
	double dCdHeight[25];

	PPA_INFO()
	{
		dPpaXAvg = 0;		// Pitch(um)
		dPpaXMin = 0;		// Pitch(um)
		dPpaXMax = 0;		// Pitch(um)
		dPpaXStd = 0;		// Pitch(um)

		dPpaYAvg = 0;		// Pitch(um) 
		dPpaYMin = 0;		// Pitch(um) 
		dPpaYMax = 0;		// Pitch(um) 
		dPpaYStd = 0;		// Pitch(um) 

		dCdWidthAvg = 0;	// 구멍 크기(um)
		dCdWidthMin = 0;	// 구멍 크기(um)
		dCdWidthMax = 0;	// 구멍 크기(um)
		dCdWidthStd = 0;	// 구멍 크기(um)

		dCdHeightAvg = 0;	// 구멍 크기(um)
		dCdHeightMin = 0;	// 구멍 크기(um)
		dCdHeightMax = 0;	// 구멍 크기(um)
		dCdHeightStd = 0;	// 구멍 크기(um)

		nPoint = 0;
		for( int i=0 ; i<25 ; i++ )
		{
			dPpaX[i]	= 0;
			dPpaY[i]	= 0;
			dCdWidth[i]	= 0;
			dCdHeight[i]= 0;
		}
	}
};


// 위치
#define BHIP_POS_LT	0
#define BHIP_POS_RT	1
#define BHIP_POS_LB	2
#define BHIP_POS_RB	3

//
#define BHIP_LEFT	0
#define BHIP_RIGHT	1