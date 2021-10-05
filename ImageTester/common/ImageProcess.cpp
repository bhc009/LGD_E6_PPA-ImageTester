#include "StdAfx.h"
#include "ImageProcess.h"

#include <stdlib.h>
#include <math.h>


// edge direction
// #define DIR_0	0
// #define DIR_30	1
// #define DIR_60	2
// #define DIR_90	3
// #define DIR_120	4
// #define DIR_150	5
// #define DIR_180	6
// #define DIR_210	7
// #define DIR_240	8
// #define DIR_270	9
// #define DIR_300	10
// #define DIR_330	11


// #define PI 3.14159265359
//
// #define POINT_XY(img,width,x,y)	img[ (x) + (y)*(width) ]
// #define RANGE_LIMIT(v_min, v, v_max)	MAX( MIN( v, v_max), v_min )


CImageProcess::CImageProcess(void)
{
// 	m_bSaveOriginalImage = true;
// 	m_bSaveProcessingImage = false;
// 	m_bDisplayImage = false;

// 	m_fResolution = 3.45f;	// 3.45 um

// 	m_ptMainPattern1_world.x = 0;
// 	m_ptMainPattern1_world.y = 0;
// 	m_ptMainPattern2_world.x = 350000;	// 단위 um
// 	m_ptMainPattern2_world.y = 714000;	// 단위 um
// 
// 	m_ptSubPattern1_world.x = 0;
// 	m_ptSubPattern1_world.y = 0;
// 	m_ptSubPattern2_world.x = 350000;	// 단위 um
// 	m_ptSubPattern2_world.y = 714000;	// 단위 um
// 
// 	m_ptMainPattern1_image.x = 2448/2;	
// 	m_ptMainPattern1_image.y = 2048/2;
// 	m_ptMainPattern2_image.x = 2448/2;	
// 	m_ptMainPattern2_image.y = 2048/2;	

// 	m_nccModel.pTemplateImage = NULL;

	QueryPerformanceFrequency(&m_counter_Freq);

}

CImageProcess::~CImageProcess()
{
	m_nPpaResult = 0;

// 	if( m_nccModel.pTemplateImage )
// 	{
// 		delete[] m_nccModel.pTemplateImage;
// 	}
}

void CImageProcess::start_global_counter()
{
	QueryPerformanceCounter(&m_globalStartCounter);
}

void CImageProcess::end_global_counter(CString strMsg)
{
	QueryPerformanceCounter(&m_globalEndCounter);

	CString str;
	str.Format(" = %f sec\n", (double)(m_globalEndCounter.QuadPart - m_globalStartCounter.QuadPart) / (double)m_counter_Freq.QuadPart );

	OutputDebugString(strMsg+str);
}

//////////////////////////////////////////////////////////////////////////
//
// 두 각도간 사이각을 구한다.
//
// [In]
//	- fAngle1 : 크기는 0 이상
//	- fAngle2 : 크기는 0 이상
//
//////////////////////////////////////////////////////////////////////////
float CImageProcess::calc_angle_distance_degree( float fAngle1, float fAngle2 )
{
	float a = fAngle1;
	float b = fAngle2;

	while( a>=360.0f )
	{
		a -= 360.0f;
	}

	while( a<0.0f )
	{
		a += 360.0f;
	}

	while( b>=360.0f )
	{
		b -= 360.0f;	
	}

	while( b<0.0f )
	{
		b += 360.0f;	
	}

	float fResult = fabs( a - b );

	// 각도가 180 이상이면 작은 각도를 계산
	if( fResult>180.0f )
	{
		fResult = 360.0f - fResult;
	} 

	return fResult;
}

float CImageProcess::calc_angle_distance_radian( float fAngle1, float fAngle2 )
{
	float a = fAngle1;
	float b = fAngle2;

	while( a>=2*_IP_PI_ )
	{
		a -= (float)(2*_IP_PI_);
	}

	while( a<0.0f )
	{
		a += (float)(2*_IP_PI_);
	}

	while( b>=2*_IP_PI_ )
	{
		b -= (float)(2*_IP_PI_);	
	}

	while( b<0.0f )
	{
		b += (float)(2*_IP_PI_);	
	}

	float fResult = fabs( a - b );

	// 각도가 180 이상이면 작은 각도를 계산
	if( fResult>_IP_PI_ )
	{
		fResult = (float)(2*_IP_PI_) - fResult;
	} 

	return fResult;
}




//
// Gaussian param
//
// int Mask_3[3][3] = {{1,2,1},{2,4,2},{1,2,1}};
// int Mask_5[5][5] =
// {{1,1,2,1,1},{1,2,4,2,1},{2,4,8,4,2},{1,2,4,2,1},{1,1,2,1,1}};
// int Mask_9[9][9] =
// {{1,1,1,2,2,2,1,1,1},{1,1,2,2,4,2,2,1,1},{1,2,2,4,8,4,2,2,1},
// {2,2,4,8,16,8,5,2,2},{2,4,8,16,32,16,8,4,2},{2,2,4,8,16,8,4,2,2},
// {1,2,2,4,8,4,2,2,1},{1,1,2,2,4,2,2,1,1},{1,1,1,2,2,2,1,1,1}};
bool CImageProcess::Gauss_image( BYTE *pImage_in, BYTE *pImage_out, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int size )
{
	int x, y, xx, yy, row, col;
	int iSum, iNum;

	int szFilter_half = size/2;
	int Mask[3][3] = {{1,2,1},{2,4,2},{1,2,1}};

	for( y=(INT)sy ; y<=(INT)ey ; y++ )
	{
		for( x=(INT)sx ; x<=(INT)ex ; x++ )
		{
			iSum = 0;
			iNum = 0;

			// convolution
			for( yy=y-szFilter_half, row=0 ; yy<=y+szFilter_half ; yy++, row++ )
			{
				if( yy<0 || yy>(INT)height-1 )
				{
					continue;
				}

				for( xx=x-szFilter_half, col=0 ; xx<=x+szFilter_half ; xx++, col++ )
				{
					if( xx<0 || xx>(INT)width-1 )
					{
						continue;
					}
					iSum += pImage_in[ xx + yy*width ]*Mask[row][col];
					iNum += Mask[row][col];
				}
			}

			pImage_out[ x + y*width ] = iSum/iNum;
		}
	}


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// Mean image
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::mean_image(BYTE *pImage, BYTE *pImage_out, int width, int height, int sx, int sy, int ex, int ey, int szFilterHalf)
{
	int x, y, xx, yy;
	int iSum;
	int iNum;

	for( y=sy ; y<=ey ; y++ )
	{
		for( x=sx ; x<=ex ; x++ )
		{
			iSum = 0;
			iNum = 0;

			// convolution
			for( yy=y-szFilterHalf ; yy<=y+szFilterHalf ; yy++ )
			{
				if( yy<0 || yy>height-1 )
				{
					continue;
				}

				for( xx=x-szFilterHalf ; xx<=x+szFilterHalf ; xx++ )
				{
					if( xx<0 || xx>width-1 )
					{
						continue;
					}
					iSum += pImage[ xx + yy*width ];
					iNum++;
				}
			}

			pImage_out[ x + y*width ] = iSum/iNum;
		}
	}


	return true;
}

bool CImageProcess::blur(BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, CSize szFilter)
{
	AfxMessageBox(_T("No fuction"));

	return false;
}


bool CImageProcess::box_filter(BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, CSize szFilter)
{
	AfxMessageBox(_T("No fuction"));

	return false;
}


bool CImageProcess::median_filter(BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, CSize szFilter)
{
	AfxMessageBox(_T("No fuction"));

	return false;
}


bool CImageProcess::gaussian_filter(BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, CSize szFilter)
{
	AfxMessageBox(_T("No fuction"));

	return false;
}

bool CImageProcess::prewitt( BYTE *pImage, BYTE *pImage_out, int width, int height, int sx, int ex, int sy, int ey )
{
	AfxMessageBox(_T("No fuction"));

	return false;
}


bool CImageProcess::sobel( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey )
{
	AfxMessageBox(_T("No fuction"));

	return false;
}


bool CImageProcess::scharr( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey )
{
	AfxMessageBox(_T("No fuction"));

	return false;
}


bool CImageProcess::laplacian( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey )
{
	AfxMessageBox(_T("No fuction"));

	return false;
}


bool CImageProcess::edge( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int iMethod, int iParam )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return false;
}


//
// Masking
//
//	- boundary 영역도 mask 적용
//
bool CImageProcess::Mask_image_include_boundary(BYTE *pImage_in, BYTE *pImage_out, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int size, int *pMask, bool bProcessBoundary, bool bBias, UINT bias )
{
	int x, y, xx, yy, row, col;
	int iSum, iNum;

	int szFilter_half = size/2;

	for( y=(INT)sy ; y<=(INT)ey ; y++ )
	{
		for( x=(INT)sx ; x<=(INT)ex ; x++ )
		{
			iSum = 0;
			iNum = 0;

			// Masking
			for( yy=y-szFilter_half, row=0 ; yy<=y+szFilter_half ; yy++, row++ )
			{
				if( yy<0 || yy>(INT)height-1 )
				{
					continue;
				}

				for( xx=x-szFilter_half, col=0 ; xx<=x+szFilter_half ; xx++, col++ )
				{
					if( xx<0 || xx>(INT)width-1 )
					{
						continue;
					}
					iSum += pImage_in[ xx + yy*width ]*pMask[col + row*size];
					iNum ++;
				}
			}

			pImage_out[ x + y*width ] = max( 0, min( 255, bias + abs(iSum/iNum)) );
		}
	}


	return true;
}


//
// Masking
//
//	- Boundary 영역은 제외
//	- bias를 기준으로 값 적용
//
bool CImageProcess::Mask_image_except_boundary(BYTE *pImage_in, BYTE *pImage_out, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int size, int *pMask, UINT bias )
{
	int x, y, xx, yy, row, col;
	int iSum, iNum;

	int szFilter_half = size/2;

	for( y=(INT)sy ; y<=(INT)ey ; y++ )
	{
		for( x=(INT)sx ; x<=(INT)ex ; x++ )
		{
			iSum = 0;
			iNum = 0;

			pImage_out[ x + y*width ] = bias;

			if( x<szFilter_half || x>(INT)width-szFilter_half-1 || y<szFilter_half || y>(INT)height-szFilter_half-1 )
			{
				continue;
			}

			// Masking
			for( yy=y-szFilter_half, row=0 ; yy<=y+szFilter_half ; yy++, row++ )
			{
				if( yy<0 || yy>(INT)height-1 )
				{
					continue;
				}

				for( xx=x-szFilter_half, col=0 ; xx<=x+szFilter_half ; xx++, col++ )
				{
					if( xx<0 || xx>(INT)width-1 )
					{
						continue;
					}
					iSum += pImage_in[ xx + yy*width ]*pMask[col + row*size];
					iNum ++;
				}
			}

			pImage_out[ x + y*width ] = max( 0, min( 255, bias + abs(iSum/iNum)) );
		}
	}


	return true;
}

//
// edge 검
//
bool CImageProcess::Mask_derivative(BYTE *pImage, BYTE *pImage_out, unsigned int width, unsigned int height, unsigned int sx, unsigned int ex, unsigned int sy, unsigned int ey, int size)
{
	//
	int mask_dx[3*3] = {	
		-1, 0, 1, 
		-2, 0, 2,  
		-1, 0, 1 };
	
	BYTE *pImage_dx = new BYTE[width*height];

	Mask_image_except_boundary(pImage, pImage_dx, width, height, sx, ex, sy, ey, size, mask_dx, 0);


	//
	int mask_dy[3*3] = {
		-1, -2, -1, 
		0, 0, 0,  
		1, 2, 1 };

	BYTE *pImage_dy = new BYTE[width*height];

	Mask_image_except_boundary(pImage, pImage_dy, width, height, sx, ex, sy, ey, size, mask_dy, 0);


	//
	sqrt_image( pImage_dx, pImage_dy, pImage_out, width, height, sx, sy, ex, ey );


	delete[] pImage_dx;
	delete[] pImage_dy;

	return true;
}


BYTE* CImageProcess::load_image( char *pFileName, int *pWidth, int *pHeight )
{
	return NULL;
}


//
// pImageResult = sqrt( pImage1^2 + pImage2^2 )
//
bool CImageProcess::sqrt_image( BYTE *pImage1, BYTE *pImage2, BYTE *pImageResult, unsigned int width, unsigned int height, unsigned int sx, unsigned int sy, unsigned int ex, unsigned int ey )
{
	int x, y, iPos;
	float g1, g2;

	for( y=(INT)sy ; y<=(INT)ey ; y++ )
	{
		for( x=(INT)sx ; x<=(INT)ex ; x++ )
		{
			iPos = x + y*width;

			g1 = pImage1[iPos];
			g2 = pImage2[iPos];

			pImageResult[ iPos ] = (BYTE)sqrt( g1*g1 + g2*g2 );
		}
	}

	return true;
}

//
// threshold
//
bool CImageProcess::threshold( BYTE *pImage, BYTE *pImageResult, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int lowThreshold, int highThreshold, int objectGray, int backgroundGray )
{
	UINT x, y;

	for( y=sy ; y<=ey ; y++ )
	{
		for( x=sx ; x<=ex ; x++ )
		{
			if( pImage[x + y*width]<lowThreshold || pImage[x + y*width]>highThreshold )
			{
				pImageResult[ x + y*width ] = backgroundGray;
			} else {
				pImageResult[ x + y*width ] = objectGray;
			}
		}
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// threshold
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::threshold( float *pImage, BYTE *pImageResult, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, float lowThreshold, float highThreshold, int objectGray, int backgroundGray )
{
	UINT x, y;

	for( y=sy ; y<=ey ; y++ )
	{
		for( x=sx ; x<=ex ; x++ )
		{
			if( pImage[x + y*width]<lowThreshold || pImage[x + y*width]>highThreshold )
			{
				pImageResult[ x + y*width ] = backgroundGray;
			} else {
				pImageResult[ x + y*width ] = objectGray;
			}
		}
	}

	return true;
}


bool CImageProcess::threshold( BYTE *pImage, BYTE *pImageResult, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, THRESHOLD_METHOD eMethod )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return false;
}

//////////////////////////////////////////////////////////////////////////
//
// Thinning algorithm
//
//

// Zhang Suen의pixel 삭제조건
// 
//  P4   P3   P2
//  P5   P0   P1
//  P6   P7   P8
// 
// 1) S(P0) = 1
// 2) 2 <= count(P0) <= 6
// 3) P1*P3*P5 = 0
// 4) P3*P5*P7 = 0
// 3') P7*P1*P3 = 0
// 4') P5*P7*P1 = 0
// 
// count(Pn) :Pn 이웃 중 0이 아닌 값을 가진 이웃수
// S(Pn) :Pn 이웃의 0-1 천이 회수


#define PIXEL_YX(img,y,x)	img->imageData[(y)*img->widthStep + (x)]
/*
void CImageProcess::DeletePixel(IplImage *valImg, IplImage *tmpImg)
{
	int width = cvGetSize(valImg).width;
	int height = cvGetSize(valImg).height;

	for(int y = 1; y < height; y++) {
		for(int x = 1; x < width; x++) {
			if(PIXEL_YX(tmpImg, y, x)) {
				PIXEL_YX(valImg, y, x) = 0;
				PIXEL_YX(tmpImg, y, x) = 0;
			}
		}
	}
}

int CImageProcess::CountPixel(IplImage *img, int cx, int cy)
{
	int count = 0;

	for(int y = cy-1; y <= cy+1; y++) {
		for(int x = cx-1; x <= cx+1; x++) {
			if(y!=cy || x!=cx) {
				if(PIXEL_YX(img, y, x)) {
					count++;
				}
			}
		}
	}
	return count;
}

int CImageProcess::IsConnected(IplImage* img, int x, int y)
{
	int count = 0;

	if (PIXEL_YX(img, y+1, x  ) && !PIXEL_YX(img, y+1, x-1)) count++;
	if (PIXEL_YX(img, y+1, x-1) && !PIXEL_YX(img, y  , x-1)) count++;
	if (PIXEL_YX(img, y  , x-1) && !PIXEL_YX(img, y-1, x-1)) count++;
	if (PIXEL_YX(img, y-1, x-1) && !PIXEL_YX(img, y-1, x  )) count++;
	if (PIXEL_YX(img, y-1, x  ) && !PIXEL_YX(img, y-1, x+1)) count++;
	if (PIXEL_YX(img, y-1, x+1) && !PIXEL_YX(img, y  , x+1)) count++;
	if (PIXEL_YX(img, y  , x+1) && !PIXEL_YX(img, y+1, x+1)) count++;
	if (PIXEL_YX(img, y+1, x+1) && !PIXEL_YX(img, y+1, x  )) count++;

	return count;
}


void CImageProcess::ThinningZhang(IplImage *inImg, IplImage *outImg)
{
	// inImg 원 영상
	// outImg 세선화 영상

	int width = cvGetSize(inImg).width;
	int height = cvGetSize(inImg).height;

	IplImage* valImg = cvCreateImage (cvSize (width+2, height+2), 8, 1);
	IplImage* tmpImg = cvCreateImage (cvSize (width+2, height+2), 8, 1);

	cvSetZero (valImg);
	cvSetZero (tmpImg);

	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			PIXEL_YX(valImg, y+1, x+1) = PIXEL_YX(inImg, y, x) ? 1 : 0;
		}
	}

	while (1) {
		bool remain = false;
		for(int y = 1; y < valImg->height-1; y++) {
			for(int x = 1; x < valImg->widthStep-1; x++) {
				if(PIXEL_YX(valImg, y, x)) {
					int count = CountPixel(valImg, x, y);
					if (count >= 2 && count <= 6 && IsConnected(valImg, x, y) == 1)	{
						if (PIXEL_YX(valImg, y+1, x  )*
							PIXEL_YX(valImg, y  , x-1)*
							PIXEL_YX(valImg, y-1, x  ) == 0 &&
							PIXEL_YX(valImg, y  , x-1)*
							PIXEL_YX(valImg, y  , x+1)*
							PIXEL_YX(valImg, y-1, x  ) == 0 ){
							PIXEL_YX(tmpImg, y, x) = !0;
							remain = true;    
						}
					}
				}
			}
		}
		DeletePixel(valImg, tmpImg);
		
		if (!remain) break;

		remain = false;

		for(int y = 1; y < valImg->height-1; y++) {
			for(int x = 1; x < valImg->widthStep-1; x++) {
				if(PIXEL_YX(valImg, y, x)) {
					int count = CountPixel(valImg, x, y);
					if((count >= 2 && count <=6) && IsConnected(valImg, x, y) == 1) {
						if (PIXEL_YX(valImg, y  , x-1)*
							PIXEL_YX(valImg, y+1, x  )*
							PIXEL_YX(valImg, y  , x+1) == 0 &&
							PIXEL_YX(valImg, y+1, x  )*
							PIXEL_YX(valImg, y  , x+1)*
							PIXEL_YX(valImg, y-1, x  ) == 0 ){
							PIXEL_YX(tmpImg, y, x) = !0;
							remain = true;
						}
					}
				}
			}
		}
		DeletePixel(valImg, tmpImg);

		if (!remain) break;
	}

	// 골격선을 255로 표시
	for(int y = 0 ; y < height ; y++) {
		for(int x = 0 ; x < width ; x++) {
			PIXEL_YX(outImg, y, x) = PIXEL_YX(valImg, y+1, x+1) ? 255 : 0;
		}
	}

	cvReleaseImage(&valImg);
	cvReleaseImage(&tmpImg);
}
*/
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// Derectional edge detection
//
//	[In]
//		- image & info
//		- ROI
//	[Out]
//		- Magnitude buffer
//		- direction buffer
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::directional_edge_detection(BYTE *pImage_in, float *pMagnitude, float *pDirection, unsigned int width, unsigned int height, unsigned int sx, unsigned int ex, unsigned int sy, unsigned int ey )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// float형 데이터를 byte형 이미지로 생성
//
//	- Just display
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::make_image_float_to_byte(float *pIn, BYTE *pOut, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey )
{
	UINT x, y; 
	float fMin=FLT_MAX, fMax=FLT_MIN;

	// Get range
	for( y=sy ; y<=ey ; y++ )
	{
		for( x=sx ; x<=ex ; x++ )
		{
			fMin = min( fMin, POINT_XY(pIn, width, x, y) );
			fMax = max( fMax, POINT_XY(pIn, width, x, y) );
		}
	}

	//
	for( y=sy ; y<=ey ; y++ )
	{
		for( x=sx ; x<=ex ; x++ )
		{
			float a = 255 *  (POINT_XY(pIn, width, x, y) - fMin)/(fMax-fMin) ;
			POINT_XY(pOut, width, x, y) = (BYTE)RANGE_LIMIT( 0, a, 255 );
		}
	}

	return true;
}


bool CImageProcess::make_image_float_to_byte(float *pIn, BYTE *pOut, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, float fRangeMin, float fRangeMax )
{
	UINT x, y; 
	float fMin=fRangeMin, fMax=fRangeMax;


	//
	for( y=sy ; y<=ey ; y++ )
	{
		for( x=sx ; x<=ex ; x++ )
		{
			float a = 255 *  (POINT_XY(pIn, width, x, y) - fMin)/(fMax-fMin) ;
			POINT_XY(pOut, width, x, y) = (BYTE)RANGE_LIMIT( 0, a, 255 );
		}
	}

	return true;
}


void CImageProcess::paint_boundary_region( BYTE *pImage, UINT width, UINT height, int range, int paint_value )
{
	int x, y;

	for( y=0 ; y<(INT)height ; y++ )
	{
		for( x=0 ; x<range ; x++ )
		{
			POINT_XY( pImage,width,x,y) = paint_value;
		}
	}

	for( y=0 ; y<(INT)height ; y++ )
	{
		for( x=(INT)width-range-1 ; x<(INT)width ; x++ )
		{
			POINT_XY( pImage,width,x,y) = paint_value;
		}
	}

	for( x=0 ; x<(INT)width ; x++ )
	{
		for( y=0 ; y<range ; y++ )
		{
			POINT_XY( pImage,width,x,y) = paint_value;
		}
	}

	for( x=0 ; x<(INT)width ; x++ )
	{
		for( y=(INT)height-range-1 ; y<(INT)height ; y++ )
		{
			POINT_XY( pImage,width,x,y) = paint_value;
		}
	}
}


void CImageProcess::paint_boundary_region( BYTE *pImage, UINT width, UINT height, int sx, int ex, int sy, int ey, int range, int paint_value )
{
	int x, y;

	for( y=sy ; y<=ey ; y++ )
	{
		for( x=sx ; x<sx+range ; x++ )
		{
			POINT_XY( pImage,width,x,y) = paint_value;
		}
	}

	for( y=sy ; y<=ey ; y++ )
	{
		for( x=ex-range+1 ; x<=ex ; x++ )
		{
			POINT_XY( pImage,width,x,y) = paint_value;
		}
	}

	for( x=sx ; x<=ex ; x++ )
	{
		for( y=sy ; y<sy+range ; y++ )
		{
			POINT_XY( pImage,width,x,y) = paint_value;
		}
	}

	for( x=sx ; x<=ex ; x++ )
	{
		for( y=ey-range+1 ; y<=ey ; y++ )
		{
			POINT_XY( pImage,width,x,y) = paint_value;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//
// Thinning edge magnitude images
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::thin_edge( BYTE *pEdgeImage, float *pMag, float *pDir, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey)
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}

//////////////////////////////////////////////////////////////////////////
//
// Sup-pixel 정도로 egde 추출
//
//	[In]
//		원본영상 : pOrgImage
//		에지영상 : pEdgeImage
//		에지성분 : pMag
//
//	[Out]
//		pPointX
//		pPointY
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::edge_subpixel( BYTE *pOrgImage, BYTE *pEdgeImage, float *pMag, float *pPointX, float *pPointY, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey)
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	Calc taylor param(2d)
//
//	- No boundary check 
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::calc_taylor_param( float *pIn, int width, int x, int y, float *pDx, float *pDy, float *pDxx, float *pDxy, float *pDyy )
{
	float *pDxTemp = new float[9];
	float *pDyTemp = new float[9];

	int xx, yy;
	int pos=0;
	for( yy=y-1; yy<=y+1 ; yy++ )
	{
		for( xx=x-1 ; xx<=x+1 ; xx++, pos++ )
		{
			// Dx
			pDxTemp[pos] = 0.5f*( pIn[ xx+1 + yy*width ] - pIn[ xx-1 + yy*width ] );

			// Dy
			pDyTemp[pos] = 0.5f*( pIn[ xx + (yy+1)*width ] - pIn[ xx + (yy-1)*width ] );

		}
	}

	*pDx = pDxTemp[4];
	*pDy = pDyTemp[4];

	// Dxx
	*pDxx = 0.5f*( pDxTemp[5] - pDxTemp[3] );

	// Dyy
	*pDyy = 0.5f*( pDyTemp[7] - pDyTemp[1] );

	// Dxy
	*pDxy = 0.5f * ( 0.5f*( pDxTemp[7] - pDxTemp[1] ) + 0.5f*( pDyTemp[5] - pDyTemp[3] ) );


	delete[] pDxTemp;
	delete[] pDyTemp;


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	Calc taylor param(2d)
//
//	- No boundary check 
//	- point를 중심으로 5x5 공간 필요
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::calc_taylor_param( BYTE *pIn, int width, int x, int y, float *pDx, float *pDy, float *pDxx, float *pDxy, float *pDyy )
{
	float a, b;
	float *pDxTemp = new float[9];
	float *pDyTemp = new float[9];

	int xx, yy;
	int pos=0;
	for( yy=y-1; yy<=y+1 ; yy++ )
	{
		for( xx=x-1 ; xx<=x+1 ; xx++, pos++ )
		{
			a = pIn[ xx+1 + yy*width ];
			b = pIn[ xx-1 + yy*width ];
			// Dx
			pDxTemp[pos] = 0.5f*( a - b );

			a = pIn[ xx + (yy+1)*width ];
			b = pIn[ xx + (yy-1)*width ];
			// Dy
			pDyTemp[pos] = 0.5f*( a - b );

		}
	}

	*pDx = pDxTemp[4];
	*pDy = pDyTemp[4];

	// Dxx
	*pDxx = 0.5f*( pDxTemp[5] - pDxTemp[3] );

	// Dyy
	*pDyy = 0.5f*( pDyTemp[7] - pDyTemp[1] );

	// Dxy
	*pDxy = 0.5f * ( 0.5f*( pDxTemp[7] - pDxTemp[1] ) + 0.5f*( pDyTemp[5] - pDyTemp[3] ) );


	delete[] pDxTemp;
	delete[] pDyTemp;


	return true;
}

//////////////////////////////////////////////////////////////////////////
//
// 각도를 0 ~ 2pi 범위로 변환
//	
//	단위 : radian
//
//////////////////////////////////////////////////////////////////////////
float CImageProcess::convert_angle_range( float fAngle )
{
	float fResult = fAngle;

	while(fResult<0.0f )
	{
		fResult += (float)(2*_IP_PI_);
	}

	while( fResult>=2*_IP_PI_ )
	{
		fResult -= (float)(2*_IP_PI_);
	}

	return fResult;
}


//////////////////////////////////////////////////////////////////////////
//
//	다음 edge point를 검사
//
//	[In]
//		fAngle_edge : 현재 edge 방향(단위:degree)
//
//	[Out]
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::search_neighbor_edge_point( BYTE *pImage, BYTE *pEdgeImage, float *pMag, UINT width, UINT height, int x, int y, float fAngle_edge, int *x_out, int *y_out )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}

bool CImageProcess::erode( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int nSzKernel )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}


bool CImageProcess::dilation( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int nSzKernel )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
BYTE* CImageProcess::pyramid_image( BYTE *pImageIn, int widthIn, int heightIn, int level, int *widthOut, int* heightOut )
{
	int iSum, iNum;
	int x, y, i, j;

	int factor = (int)pow( 2.0, level );

	int width = widthIn/factor;
	int height = heightIn/factor;

	BYTE *pOut = new BYTE[width*height];

	for( y=0 ; y<height ; y++ )
	{
		for( x=0 ; x<width ; x++ )
		{
			iSum = 0;
			iNum = 0;
			for( j=0 ; j<factor ; j++ )
			{
				for( i=0 ; i<factor ; i++ )
				{
					iSum += POINT_XY( pImageIn, widthIn, x*factor + i, y*factor + j );
					iNum++;
				}
			}
			POINT_XY( pOut, width, x, y ) = iSum/iNum;
		}
	}

	*widthOut = width;
	*heightOut = height;

	return pOut;
}

//////////////////////////////////////////////////////////////////////////
//
// Create NCC model
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::create_ncc_model( BYTE *pImage, int width, int height, int sx, int ex, int sy, int ey, int lowThreshold, int highThreshold )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// Create NCC model
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::create_ncc_model( BYTE *pImage, int width, int height, int sx, int ex, int sy, int ey, int iColor )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// Find NCC model
//
//	.
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::find_ncc_model( BYTE *pImage, int width, int height, int sx, int ex, int sy, int ey, float *resultX, float *resultY  )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// Find NCC model
//
//	- Pyramid 적용
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::find_ncc_model( BYTE *pImage, int width, int height, int sx, int ex, int sy, int ey, float *resultX, float *resultY, int pyramid )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// Sub-pixel 단위의 max point를 찾는다.
//
//	- Taylor 함수 활용
//	- f( x + dx, y + dy ) = f(x,y) + fx*dx + fy*dy + ( fxx*dx^2 + 2*fxy*dx*dy + fyy*dy^2 )/2 + ...
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::calc_max_point_taylor( float *pScoreMap, int width, int height, int sx, int ex, int sy, int ey, float *xResult, float *yResult)
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// Sub-pixel 단위의 max point를 찾는다.
//
//	- facet model
//		f(x,y) = p1 + p2*x + p3*y + p4*x^2 + p5*y^2 + p6*x*y
//		dx = P2 + 2 * P4 * x + P6 * y = 0
//		dy = P3 + 2 * P5 * y + P6 * x = 0
//
//		=>	dy = (P2*P6 - 2*P3*P4)/(4*P4*P5 - P6^2 )
//			dx = -( P2 + P6*dy )/( 2*P4 )
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::calc_max_point_facet( float *pScoreMap, int width, int height, int sx, int ex, int sy, int ey, float *xResult, float *yResult)
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// Interpolation
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::cale_bilinear_interpolation( float *pMap, int width, int height, float x, float y, float *resultValue )
{
	int sx, sy, lx, ly;  //small u, small v, large u, large v를 사용하여 추정좌표 근처의 4개 pixel 표현
	
	sx = (int)x;
	sy = (int)y;
	lx = (int)(x + 1);
	ly = (int)(y + 1);

	//
	if( sx<0 || lx>=width || sy<0 || ly>=height )
	{
		return false;
	}

	//bilinear
	double inc1, inc2, ttt;
	ttt = pMap[ lx + sy*width ];
	inc1 = pMap[lx + sy*width] - pMap[sx+sy*width];	// 분모는 1이므로 생략
	inc2 = pMap[lx + ly*width] - pMap[sx+ly*width];	// 분모는 1이므로 생략

	double temp1, temp2;  // 위쪽 변의 점의 값과 아래쪽 변의 점의 값
	temp1 = pMap[sx+sy*width] + (inc1 * (x-sx));	// 윗변 값
	temp2 = pMap[sx+ly*width] + (inc2 * (x-sx));	// 아랫변 값

	double inc;
	inc = temp2-temp1;  // 위쪽 변의 점과 아래쪽 변의 점 사이의 값증가폭
	*resultValue = (float)( temp1 + (inc * (y-sy)) ); 

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// Interpolation
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::cale_bilinear_interpolation( BYTE *pMap, int width, int height, float x, float y, float *resultValue )
{
	int sx, sy, lx, ly;  //small u, small v, large u, large v를 사용하여 추정좌표 근처의 4개 pixel 표현

	sx = (int)x;
	sy = (int)y;
	lx = (int)(x + 1);
	ly = (int)(y + 1);

	//
	if( sx<0 || lx>=width || sy<0 || ly>=height )
	{
		return false;
	}

	//bilinear
	double inc1, inc2, ttt;
	ttt = pMap[ lx + sy*width ];
	inc1 = pMap[lx + sy*width] - pMap[sx+sy*width];	// 분모는 1이므로 생략
	inc2 = pMap[lx + ly*width] - pMap[sx+ly*width];	// 분모는 1이므로 생략

	double temp1, temp2;  // 위쪽 변의 점의 값과 아래쪽 변의 점의 값
	temp1 = pMap[sx+sy*width] + (inc1 * (x-sx));	// 윗변 값
	temp2 = pMap[sx+ly*width] + (inc2 * (x-sx));	// 아랫변 값

	double inc;
	inc = temp2-temp1;  // 위쪽 변의 점과 아래쪽 변의 점 사이의 값증가폭
	*resultValue = (float)( temp1 + (inc * (y-sy)) );

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// Interpolation
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::cale_bicubic_interpolation( float *pMap, int width, int height, float x, float y, float *resultValue )
{
// 	int sx, sy, lx, ly;  //small u, small v, large u, large v를 사용하여 추정좌표 근처의 4개 pixel 표현
// 	
// 	sx = x;
// 	sy = y;
// 	lx = x + 1;
// 	ly = y + 1;
// 
// 	//
// 	if( sx<0 || lx>=width || sy<0 || ly>=height )
// 	{
// 		return false;
// 	}

	//bilinear

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// Interpolation
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::cale_bicubic_interpolation( BYTE *pMap, int width, int height, float x, float y, float *resultValue )
{
	// 	int sx, sy, lx, ly;  //small u, small v, large u, large v를 사용하여 추정좌표 근처의 4개 pixel 표현
	// 	
	// 	sx = x;
	// 	sy = y;
	// 	lx = x + 1;
	// 	ly = y + 1;
	// 
	// 	//
	// 	if( sx<0 || lx>=width || sy<0 || ly>=height )
	// 	{
	// 		return false;
	// 	}

	//bilinear

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::set_align_parameter( float fRes, float x1_world, float y1_world, float x2_world, float y2_world )
{
// 	m_fResolution = fRes;
// 
// 	m_ptMainPattern1_world.x = x1_world;
// 	m_ptMainPattern1_world.y = y1_world;
// 
// 	m_ptMainPattern2_world.x = x2_world;
// 	m_ptMainPattern2_world.y = y2_world;

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// Main pattern 등록
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::create_main_pattern( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey, int lowThreshold, int highThreshold )
{
	/*
	//
	// shape model
	//
	create_shape_model( pImage, width, height, sx, sy, ex, ey, 3 );

	m_contourVector_shapeModel_main_pattern.clear();

	for(int i=0 ; i<m_contourVector_shapeModel.size() ; i++ )
	{
		list<pointShape>::iterator ite;
		list<pointShape> tmpShapeContour;

		for( ite = m_contourVector_shapeModel[i].begin() ; ite != m_contourVector_shapeModel[i].end() ; ite++ )
		{
			tmpShapeContour.push_back(*ite);
		}
		m_contourVector_shapeModel_main_pattern.push_back(tmpShapeContour);
	}
	*/


	//
	// NCC model
	//
	
	//
// 	BYTE *pGauss = new BYTE[width*height];
// 	Gauss_image(pImage, pGauss, width, height, 0, width-1, 0, height-1, 3);
// 
// 	if( m_bSaveImage )
// 	{
// // 		SaveImage(pImage, width, height, "d:\\processing\\[create_main_pattern]input image.bmp");
// 	}
// 
// 
// 	//
// 	create_ncc_model( pGauss, width, height, sx, ex, sy, ey, lowThreshold, highThreshold );
// 
// 	delete[] pGauss;


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// sub pattern 등록
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::create_sub_pattern( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey )
{
// 	create_shape_model( pImage, width, height, sx, sy, ex, ey, 3 );
// 
// 	m_contourVector_shapeModel_sub_pattern.clear();
// 
// 	for(UINT i=0 ; i<m_contourVector_shapeModel.size() ; i++ )
// 	{
// 		list<pointShape>::iterator ite;
// 		list<pointShape> tmpShapeContour;
// 		for( ite = m_contourVector_shapeModel[i].begin() ; ite != m_contourVector_shapeModel[i].end() ; ite++ )
// 		{
// 			tmpShapeContour.push_back(*ite);
// 		}
// 		m_contourVector_shapeModel_sub_pattern.push_back(tmpShapeContour);
// 	}

	return true;
}


bool CImageProcess::copy_shape_model( vector<list<pointShape>> *scrModel, vector<list<pointShape>> *resultModel )
{
// 	resultModel->clear();
// 	
// 	for(UINT i=0 ; i<scrModel->size() ; i++ )
// 	{
// 		list<pointShape>::iterator ite;
// 		list<pointShape> tmpShapeContour;
// 
// 		for( ite = (*scrModel)[i].begin() ; ite != (*scrModel)[i].end() ; ite++ )
// 		{
// 			tmpShapeContour.push_back(*ite);
// 		}
// 
// 		resultModel->push_back(tmpShapeContour);
// 	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::find_main_pattern( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey, int index, float *xResult, float *yResult )
{
// 	float cx=0, cy=0;
// 
// 	//
// 	BYTE * pSmoothImage = new BYTE[ width*height ];
// 
// 	Gauss_image(	pImage, 
// 					pSmoothImage, //(BYTE*)Get_NextImage()->imageData,
// 					width, height, 
// 					0, width - 1, 0, height - 1, 
// 					3 );
// 
// 	if( m_bSaveImage )
// 	{
// // 		SaveImage(pImage, width, height, "d:\\processing\\[find_main_pattern]input image.bmp");
// 	}
// 
// 
// 
// 	//
// // 	copy_shape_model( &m_contourVector_shapeModel_main_pattern, &m_contourVector_shapeModel );
// // 
// // 	bool bSucess = find_shape_model( pSmoothImage, width, height, sx, ex, sy, ey, &cx, &cy, 2, _IP_TAYLOR_ );
// // 
// // 	if( bSucess )
// // 	{
// // 		*xResult = cx;
// // 		*yResult = cy;
// // 
// // 
// // 		if( index == 0 )
// // 		{
// // 			m_ptMainPattern1_image.x = cx;
// // 			m_ptMainPattern1_image.y = cy;
// // 		} else {
// // 			m_ptMainPattern2_image.x = cx;
// // 			m_ptMainPattern2_image.y = cy;
// // 		}
// // 	}
// 
// 	bool bSucess = find_ncc_model( pSmoothImage, width, height, sx, ex, sy, ey, &cx, &cy, 3 );
// 
// 	if( bSucess )
// 	{
// 		*xResult = cx;
// 		*yResult = cy;
// 	} else {
// 		*xResult = 0;
// 		*yResult = 0;
// 	}
// 
// 	delete[] pSmoothImage;
// 
// 	return bSucess;

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::find_sub_pattern( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey, int index, float *xResult, float *yResult )
{
// 	float cx=0, cy=0;
// 
// 	copy_shape_model( &m_contourVector_shapeModel_sub_pattern, &m_contourVector_shapeModel );
// 
// 	bool bSucess = find_shape_model( pImage, width, height, sx, ex, sy, ey, &cx, &cy, 3, _IP_TAYLOR_ );
// 
// 	if( bSucess )
// 	{
// 		*xResult = cx;
// 		*yResult = cy;
// 
// 		if( index == 0 )
// 		{
// 			m_ptSubPattern1_image.x = cx;
// 			m_ptSubPattern1_image.y = cy;
// 		} else {
// 			m_ptSubPattern2_image.x = cx;
// 			m_ptSubPattern2_image.y = cy;
// 		}
// 	}
// 
// 	return bSucess;

	return false;
}


//
//
//
bool CImageProcess::set_frame_pattern( float x_image, float y_image, float x_world, float y_world, int index )
{
// 	bool bOk;
// 	CString str;
// 
// 	switch( index )
// 	{
// 	case 0:
// 		m_ptMainPattern1_image.x = x_image;
// 		m_ptMainPattern1_image.y = y_image;
// 		m_ptMainPattern1_world.x = x_world;
// 		m_ptMainPattern1_world.y = y_world;
// 
// 		str.Format( _T("[alIgn]Set mask mark 1 = (%f, %f)\n"), m_ptMainPattern1_world.x, m_ptMainPattern1_world.y );
// 		OutputDebugString(str);
// 
// 
// 		bOk = true;
// 		break;
// 
// 	case 1:
// 		m_ptMainPattern2_image.x = x_image;
// 		m_ptMainPattern2_image.y = y_image;
// 		m_ptMainPattern2_world.x = x_world;
// 		m_ptMainPattern2_world.y = y_world;
// 
// 		str.Format( _T("[alIgn]Set mask mark 2 = (%f, %f)\n"), m_ptMainPattern2_world.x, m_ptMainPattern2_world.y );
// 		OutputDebugString(str);
// 
// 		bOk = true;
// 		break;
// 
// 	default:
// 		bOk = false;
// 		break;
// 	}
// 
// 	return bOk;

	return false;
}


//
//
//
bool CImageProcess::set_glass_pattern( float x_image, float y_image, float x_world, float y_world, int index )
{
// 	bool bOk;
// 
// 	switch( index )
// 	{
// 	case 0:
// 		m_ptSubPattern1_image.x = x_image;
// 		m_ptSubPattern1_image.y = y_image;
// 		m_ptSubPattern1_world.x = x_world;
// 		m_ptSubPattern1_world.y = y_world;
// 
// 		bOk = true;
// 		break;
// 
// 	case 1:
// 		m_ptSubPattern2_image.x = x_image;
// 		m_ptSubPattern2_image.y = y_image;
// 		m_ptSubPattern2_world.x = x_world;
// 		m_ptSubPattern2_world.y = y_world;
// 
// 		bOk = true;
// 		break;
// 
// 	default:
// 		bOk = false;
// 		break;
// 	}
// 
// 	return bOk;


	return false;

}


//////////////////////////////////////////////////////////////////////////
//
//	calc_align
//
//	dx, dy : sub pattern의 필요 이동랑
//	theta : 필요이동랑(시계방향)
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::calc_align( float *dx, float *dy, float *theta )
{
// 	// Glass mark의 목표 위치
// 	FPoint ptTarget1_world, ptTarget2_world;
// 	ptTarget1_world.x = m_ptSubPattern1_world.x;	// glass mark #1의 world 좌표
// 	ptTarget1_world.y = m_ptSubPattern1_world.y;
// 	ptTarget2_world.x = m_ptSubPattern2_world.x;	// glass mark #2의 world 좌표
// 	ptTarget2_world.y = m_ptSubPattern2_world.y;
// 
// 
// 	// Glass mark의 현재 world 좌표
// 	// align mark(left top)의 위치
// 	m_ptSubPattern1_world.x = m_ptMainPattern1_world.x + m_fResolution * ( m_ptSubPattern1_image.x - m_ptMainPattern1_image.x);
// 	m_ptSubPattern1_world.y = m_ptMainPattern1_world.y + m_fResolution * ( m_ptSubPattern1_image.y - m_ptMainPattern1_image.y);
// 
// 	// align mark(right bottom)의 위치
// 	m_ptSubPattern2_world.x = m_ptMainPattern2_world.x + m_fResolution * ( m_ptSubPattern2_image.x - m_ptMainPattern2_image.x);
// 	m_ptSubPattern2_world.y = m_ptMainPattern2_world.y + m_fResolution * ( m_ptSubPattern2_image.y - m_ptMainPattern2_image.y);
// 
// 
// 	// 목표 위치의 중심
// // 	float cxMain = (m_ptMainPattern1_world.x + m_ptMainPattern2_world.x)/2;
// // 	float cyMain = (m_ptMainPattern1_world.y + m_ptMainPattern2_world.y)/2;
// 	float cxMain = (ptTarget1_world.x + ptTarget2_world.x)/2;
// 	float cyMain = (ptTarget1_world.y + ptTarget2_world.y)/2;
// 
// 
// 	// 현재 위치의 중심
// 	float cxSub = (m_ptSubPattern1_world.x + m_ptSubPattern2_world.x)/2;
// 	float cySub = (m_ptSubPattern1_world.y + m_ptSubPattern2_world.y)/2;
// 
// 
// 	// 중심 차이
// 	*dx = cxMain - cxSub;
// 	*dy = cyMain - cySub;
// 
// 
// 	// 각각의 회전량
// // 	float fThetaMain = atan2( m_ptMainPattern2_world.y - m_ptMainPattern1_world.y, m_ptMainPattern2_world.x - m_ptMainPattern1_world.x );
// // 	float fThetaSub = atan2( m_ptSubPattern2_world.y - m_ptSubPattern1_world.y, m_ptSubPattern2_world.x - m_ptSubPattern1_world.x );
// 	float fThetaMain = atan2( ptTarget2_world.y - ptTarget1_world.y, ptTarget2_world.x - ptTarget1_world.x );
// 	float fThetaSub = atan2( m_ptSubPattern2_world.y - m_ptSubPattern1_world.y, m_ptSubPattern2_world.x - m_ptSubPattern1_world.x );
// 
// 	*theta = (float)( 180.f*(fThetaMain - fThetaSub)/_IP_PI_ );
// 
// 
// 	//
// 	CString str;
// 	str.Format( _T("[alIgn]mask mark(um,um) = (%f, %f) & (%f, %f)\n"), m_ptMainPattern1_world.x, m_ptMainPattern1_world.y, m_ptMainPattern2_world.x, m_ptMainPattern2_world.y );
// 	OutputDebugString(str);
// 
// 	str.Format( _T("[alIgn]glass mark(um,um) = (%f, %f) & (%f, %f)\n"), m_ptSubPattern1_world.x, m_ptSubPattern1_world.y, m_ptSubPattern2_world.x, m_ptSubPattern2_world.y );
// 	OutputDebugString(str);
// 
// 	str.Format( _T("[alIgn]Target position(um,um,degree) : cx=%f, cy=%f, theta=%f\n"), cxMain, cyMain, fThetaMain*180.f/_IP_PI_ );
// 	OutputDebugString(str);
// 
// 	str.Format( _T("[alIgn]Glass position(um,um,degree) : cx=%f, cy=%f, theta=%f\n"), cxSub, cySub, fThetaSub*180.f/_IP_PI_ );
// 	OutputDebugString(str);

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// PPA 검사용 pattern
//
//	lowThreshold, highThreshold : center를 구하기 위한 threshold
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::create_mask_pattern_ppa( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, int lowThreshold, int highThreshold )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::find_mask_pattern_ppa( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, float *cx, float *cy )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::pitch_compare( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, UINT uiPitch )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//	- sub-pattern 검사
//	- mask pattern 검사
//		.영역 설정
//		.threshold
//		.labeling & find max
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::insp_offset( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, int szMask )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return true;
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::insp_offset( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, int lowThrashold_glass, int highThreshold_glass, int minArea_glass, int maxArea_glass, int lowThreshold_mask, int highThreshold_mask, int minArea_mask, int maxArea_mask, int szMask, float *cx_glass, float *cy_glass, float *szGalss, float *cx_mask, float *cy_mask, float *width_mask )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return false;
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::insp_ppa( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, int low_threshold, int high_threshold, UINT low_area, UINT high_area, double *cx_result, double *cy_result, int *width_result, int *height_result )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return false;
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess::edit_shape_model( int szMin , int szMax )
{
// 	int i;
// 	int size = (int)( m_contourVector_shapeModel.size() );
// 
// 
// 	// find max
// 	int maxLength=0;
// 	for( i=0 ; i<size ; i++ )
// 	{
// 		if( m_contourVector_shapeModel[i].size()>(UINT)maxLength )
// 		{
// 			maxLength = (int)(m_contourVector_shapeModel[i].size());
// 		}
// 	}
// 
// 
// 	// delete else
// 	for( i=0 ; i<size ; i++ )
// 	{
// // 		if( ite->size()<szMin || ite->size()>szMax )
// 		if( m_contourVector_shapeModel[i].size() < (UINT)maxLength )
// 		{
// 			m_contourVector_shapeModel[i].clear();
// 		}
// 	}
// 
// 
// 	//
// 	size = (int)m_contourVector_shapeModel.size();
// 	float cx = 0;
// 	float cy = 0;
// 	int nData = 0;
// 
// 	for( i=0 ; i<size ; i++ )
// 	{
// 		list<pointShape>::iterator ite;
// 		for( ite = m_contourVector_shapeModel.at(i).begin() ; ite != m_contourVector_shapeModel.at(i).end() ; ite++ )
// 		{
// 			cx += ite->x;
// 			cy += ite->y;
// 			nData++;
// 		}
// 	}
// 
// 	if( nData==0 )
// 	{
// 		return false;
// 	} 
// 
// 
// 	cx /= nData;
// 	cy /= nData;
// 
// 	for( i=0 ; i<size ; i++ )
// 	{
// 		list<pointShape>::iterator ite;
// 		for( ite = m_contourVector_shapeModel.at(i).begin() ; ite != m_contourVector_shapeModel.at(i).end() ; ite++ )
// 		{
// 			ite->x -= cx;
// 			ite->y -= cy;
// 		}
// 	}


	return true;
}

bool CImageProcess::create_shape_model( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey, int szSmooth )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return false;
}

bool CImageProcess::find_shape_model( BYTE *pImage, int width, int height, UINT sx, UINT ex, UINT sy, UINT ey, float *x_pos, float *y_pos )
{
	AfxMessageBox(_T("No fuction"));

	return false;
}


bool CImageProcess::find_shape_model( BYTE *pImage, int width, int height, UINT sx, UINT ex, UINT sy, UINT ey, float *x_pos, float *y_pos, int pyramidLevel, int method )
{
	AfxMessageBox(_T("No fuction"));

	return false;
}


bool CImageProcess::save_shape_model(char *pFileName)
{
	AfxMessageBox(_T("No fuction"));

	return false;
}


bool CImageProcess::load_shape_model(char *pFileName)
{
	AfxMessageBox(_T("No fuction"));

	return false;
}


bool CImageProcess::AddActiveRegion( DRect rect )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return false;
}


bool CImageProcess::SetParam_coordinate( double dTx, double dTy, double dTheta, double xScale, double yScale )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return false;
}

bool CImageProcess::SetParam_coordinate( double *xSrc, double *ySrc, int nSrc, double *xDst, double *yDst, int nDst )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return false;
}

bool CImageProcess::Calculate_coordinate( double xIn, double yIn, double *xOut, double *yOut )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return false;
}

bool CImageProcess::setParam_surfaceInspection( PARAM_FMM_SURFACE_INSPECTIOM *pParam )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return false;
}


bool CImageProcess::inspectParticleAtFMM( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return false;
}


bool CImageProcess::inspectParticleDefect( BYTE* pImage, int width, int height, double sxCoord, double syCoord, int iDirection, DEFECT_INFO *pDefects, int *nDefect )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return false;
}


bool CImageProcess::mergeParticleDefect( list<DEFECT_INFO> *pDefectList, list<DEFECT_INFO> *pListMergedDefect )
{
	AfxMessageBox("요딴 기능 없음~ㅋㅋ");

	return false;
}

