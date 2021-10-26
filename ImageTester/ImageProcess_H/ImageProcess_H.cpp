// ImageProcess_H.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//

#include "stdafx.h"

#define DLLEXPORT_H

#include "ImageProcess_H.h"
#include "HalconCpp.h"

// #include <iostream>
// using namespace std;

using namespace HalconCpp;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HShapeModel g_shapeModel;
HNCCModel m_NCCModel;
CSize m_szNCCModel;


CImageProcess_H::CImageProcess_H(void)
{
	m_bSaveOriginalImage = true;
	m_bSaveProcessingImage = false;
	m_bDisplayImage = false;


	//////////////////////////////////////////////////////////////////////////
	// 이물검사 관련
	//////////////////////////////////////////////////////////////////////////

	m_dScanOffsetForward = 0.445;//1.5924;//0.9445;

	// align 영상 저장
	m_bSaveAlignImage = true;
	m_szSaveAlignImage = CSize( 1024, 1024 );

	//
	m_bProcessing = true;

	// align mark 위치값
	for( int i=0 ; i<4 ; i++ )
	{
		m_dAlignMarkRealX[i]	= 0.0;
		m_dAlignMarkRealY[i]	= 0.0;
		m_dAlignMarkMotorX[i]	= 0.0;
		m_dAlignMarkMotorY[i]	= 0.0;
	}

	// PPA 결과 저장
	m_strFilePath.Format("d:\\processing\\");	// 폴더
	m_strFileName.Format("PPA_result");			// 이름

	QueryPerformanceFrequency(&m_counter_Freq);


	m_tansMatrix[0] = 0;
	m_tansMatrix[1] = 0;
	m_tansMatrix[2] = 0;
	m_tansMatrix[3] = 0;
	m_tansMatrix[4] = 0;
	m_tansMatrix[5] = 0;

// 	ResetObjDb(12000,1000,1);
}


CImageProcess_H::~CImageProcess_H()
{
}


void CImageProcess_H::Test()
{
	HWindow w(300, 300, 300, 300);

	HString str("Hello halcon~\nclick");

	Hlong row = 10;
	Hlong col = 10;
	HString CoordSystem = "window";
	HString Color = "red";

	w.SetColor("green");
	w.SetTposition(10,10);
	w.WriteString("zzz");

	w.Click();
}


// void CImageProcess_H::DISPLAY( const HImage & image, CString strMsg )
void DISPLAY( const HImage & image, CString strMsg )
{
	HTuple imageWidth, imageHeight;
	image.GetImageSize( &imageWidth, & imageHeight );

// 	HWindow w(0,0,imageWidth,imageHeight);
	HWindow w(0,0,imageWidth/2,imageHeight/2);
	w.SetColored(6);
	w.DispImage(image);

	w.SetColor("green");
	w.SetTposition(10,10);
	w.WriteString(strMsg);

	w.Click();
}


// void CImageProcess_H::DISPLAY( const HRegion & region, CString strMsg )
void DISPLAY( const HRegion & region, CString strMsg )
{
	HWindow w(0,0,1024,768);
	w.SetColored(6);
	w.DispRegion(region);
	w.Click();
}


// void CImageProcess_H::DISPLAY( const HObject & object, CString strMsg )
void DISPLAY( const HObject & object, CString strMsg )
{
	HTuple imageWidth, imageHeight;
	GetImageSize( object, &imageWidth, & imageHeight );

	// 	HWindow w(0,0,imageWidth,imageHeight);
	HWindow w(0,0,imageWidth/2,imageHeight/2);
	w.SetColored(6);
	w.DispObj(object);

	w.SetColor("green");
	w.SetTposition(10,10);
	w.WriteString(strMsg);

	w.Click();
}


// void CImageProcess_H::DISPLAY( const HImage & image, HRegion & region, CString strMsg )
void DISPLAY( const HImage & image, HRegion & region, CString strMsg )
{
	HTuple imageWidth, imageHeight;
	image.GetImageSize( &imageWidth, & imageHeight );

	// 	HWindow w(0,0,imageWidth,imageHeight);
	HWindow w(0,0,imageWidth/2,imageHeight/2);

	w.SetColored(6);
	w.DispImage(image);
	w.DispRegion(region);

	w.SetColor("green");
	w.SetTposition(10,10);
	w.WriteString(strMsg);

	w.Click();
}


// void CImageProcess_H::DISPLAY( const HImage & image, HXLDCont & xld, CString strMsg )
void DISPLAY( const HImage & image, HXLDCont & xld, CString strMsg )
{
	HTuple imageWidth, imageHeight;
	image.GetImageSize( &imageWidth, & imageHeight );

	// 	HWindow w(0,0,imageWidth,imageHeight);
	HWindow w(0,0,imageWidth/2,imageHeight/2);

	w.SetColored(6);
	w.DispImage(image);
	w.DispXld(xld);

	w.SetColor("green");
	w.SetTposition(10,10);
	w.WriteString(strMsg);

	w.Click();
}


// bool CImageProcess_H::findNearRegionIndex( HRegion regions, HTuple rowTarget, HTuple colTarget, HTuple *index )
bool findNearRegionIndex( HRegion regions, HTuple rowTarget, HTuple colTarget, HTuple *index )
{
	// Region들의 중심좌표 획득
	HTuple rowsRegions, colsRegions;
	regions.AreaCenter( &rowsRegions, &colsRegions );	


	double rowHole=0, colHole=0;
	double minDistance = DBL_MAX;

	bool bValid = false;

	for( int i=0 ; i<regions.CountObj() ; i++ )
	{
		// Target 위치와 거리 계산
		HTuple distance;
		DistancePp(rowsRegions[i], colsRegions[i], rowTarget, colTarget, &distance );


		// 최소 거리여부 확인
		if( distance<minDistance )
		{
			// 최소거리 갱신
			minDistance = distance;	

			// index 갱신
			*index = i;
			bValid = true;
		}
	}


	return bValid;
}


bool findNearIndex( HTuple tRows, HTuple tCols, HTuple tRowTarget, HTuple tColTarget, HTuple *index )
{
	bool bValid = false;
	double minDistance = DBL_MAX;

	for( int i=0 ; i<tRows.Length() ; i++ )
	{
		// Target 위치와 거리 계산
		HTuple distance;
		DistancePp(tRows[i], tCols[i], tRowTarget, tColTarget, &distance );


		// 최소 거리여부 확인
		if( distance<minDistance )
		{
			// 최소거리 갱신
			minDistance = distance;	

			// index 갱신
			*index = i;
			bValid = true;
		}
	}


	return bValid;
}

//////////////////////////////////////////////////////////////////////////
//
// PPA 측정 결과 영상을 저장한다.
//	- 측정이 잘되고 있는지 확인하기 위한 용도
//
//////////////////////////////////////////////////////////////////////////
BOOL savePpaResultImage( HImage orgImage, HRegion rgResult, double xOld, double yOld, CString strFilePath, CString strFileName )
{
	Hlong imgWidth, imgHeight;
	orgImage.GetImageSize( &imgWidth, &imgHeight );

	// Color 영상을 생성한다.
	HImage imageCh1 = orgImage.CopyImage();
	HImage imageCh2 = orgImage.CopyImage();
	HImage imageCh3 = orgImage.CopyImage();

	HImage imgResult = imageCh1.Compose3(imageCh2, imageCh3);


	// 영상 중심을 표시하기 위한 십자 region 생성
	HRegion rgCenter, rgCenter1, rgCenter2;
	double cx = (double)imgWidth/2;
	double cy = (double)imgHeight/2;
	rgCenter1.GenRectangle1(cy, 0, cy, (double)imgWidth-1 );
	rgCenter2.GenRectangle1(0, cx, (double)imgWidth, cx );
	rgCenter = rgCenter1.Union2(rgCenter2);


	// 이전 PPA값의 중심을 표시하기 위한 십자 region 생성
	HRegion rgOld, rgOld1, rgOld2;
	cx = xOld;
	cy = yOld;
	rgOld1.GenRectangle1(cy, 0, cy, imgWidth-1 );
	rgOld2.GenRectangle1(0, cx, imgWidth, cx );
	rgOld = rgOld1.Union2(rgOld2);


	// 검출 구멍 region 생성
	HRegion rgHole = rgResult.Difference(rgResult.ErosionCircle(1.5));


	// 영상에 위의 3가지 영역을 표시한다.
	HImage imgMergeHole = imgResult.PaintRegion( rgResult, ((HTuple(0).Append(0)).Append(255)), HString("fill") );

	HImage imgMergeCenter = imgMergeHole.PaintRegion( rgCenter, ((HTuple(0).Append(255)).Append(0)), HString("fill") );

	HImage imgMergeOld = imgMergeCenter.PaintRegion( rgOld, ((HTuple(255).Append(0)).Append(0)), HString("fill") );


	// d:\\processing 폴더에 저장
	CString strFile;
	strFile.Format("d:\\processing\\%d_insp_ppa_result", GetTickCount());
	OutputDebugString("Save image 1 > " + strFile);

	CString tmp = strFile;
	CString tok;
	while(0 <= tmp.Find("\\"))
	{
		tok += tmp.Left(tmp.Find("\\")+1);
		tmp = tmp.Right(tmp.GetLength() - (tmp.Find("\\")+1));

		CreateDirectory(tok, NULL);
	}

	imgMergeOld.WriteImage("bmp", 0, strFile);



	// 운영 SW에서 지정한 폴더에 저장
	strFile.Format(strFilePath + strFileName);
	OutputDebugString("Save image 2 > " + strFile);

	tmp = strFilePath;
	CString tok2;
	while(0 <= tmp.Find("\\"))
	{
		tok2 += tmp.Left(tmp.Find("\\")+1);
		tmp = tmp.Right(tmp.GetLength() - (tmp.Find("\\")+1));

		CreateDirectory(tok2, NULL);
	}

	imgMergeOld.WriteImage("bmp", 0, strFile);

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
// PPA 측정 결과 영상을 저장한다.
//	- 측정이 잘되고 있는지 확인하기 위한 용도
//
//////////////////////////////////////////////////////////////////////////
BOOL findRegionOnLine( HRegion rgInput, HRegion &rgOutput, int iStartY, double iRange, int iOrgin )
{
	//
	int nRegion = (int)(rgInput.CountObj());

	if( nRegion < 1 )
	{
		return FALSE;
	}


	//
	// 동일 선상의 구멍들의 평균 y좌표 추출
	//
	double dX=0;
	double dY=0;
	int nValid=0;
	for( int i=0 ; i<rgInput.CountObj() ; i++ )
	{
		HRegion rgSelect = rgInput.SelectObj(i+1);

		double x = rgSelect.Column();
		double y = rgSelect.Row();
		
		if( y < iStartY-iRange || y > iStartY+iRange )
		{
			continue;
		}

		dX += x;
		dY += y;
		nValid++;
	}

	if( nValid==0 )
	{
		return FALSE;
	}

	double yTarget = dY/nValid;


	//
	// 추출된 Y 좌표에 해당하는 구멍 선택
	//
	HRegion rgTemp;
 	rgTemp.GenEmptyRegion();
	for( int i=0 ; i<rgInput.CountObj() ; i++ )
	{
		HRegion rgSelect = rgInput.SelectObj(i+1);

		double x = rgSelect.Column();
		double y = rgSelect.Row();

		if( y < yTarget-iRange || y > yTarget+iRange )
		{
			continue;
		}

		rgTemp = rgTemp.Union2( rgSelect );
	}

	HRegion rgHoles = rgTemp.Connection();

// 	if( m_bDisplayImage )
// 	{
// 		DISPLAY(rgHoles, "동일 선상 구멍");
// 	}


	//
	// 방향에 따라 가장 좌(우)측 구멍 선택
	//
	HTuple rows, cols;
	rgHoles.AreaCenter( &rows, &cols );

	HTuple indexSorting = cols.TupleSortIndex();

	HRegion rgHoleRoi;
	int index;
	switch( iOrgin )
	{
	case BHIP_RIGHT:
		index = indexSorting[indexSorting.TupleLength()-1];
		rgHoleRoi = rgHoles.SelectObj(index+1);
		break;

	case BHIP_LEFT:
		index = indexSorting[0];
		rgHoleRoi = rgHoles.SelectObj(index+1);
		break;

	default:
		return FALSE;

	}

// 	if( m_bDisplayImage )
// 	{
// 		DISPLAY(rgHoleRoi, "최종 선택 구멍");
// 	}


	rgOutput = rgHoleRoi;


	return TRUE;
}


void CImageProcess_H::initialize() 
{
	ResetObjDb(12000,1000,1);

	return;
}



void CImageProcess_H::IsProcessing( bool bProcessing ) 
{
	m_bProcessing = bProcessing;

	return;
}


void CImageProcess_H::saveOrigialImage( bool bSave ) 
{
	m_bSaveOriginalImage = bSave;

	return;
}


void CImageProcess_H::saveProcessingImage( bool bSave ) 
{
	m_bSaveProcessingImage = bSave;

	return;
}


void CImageProcess_H::saveAlignImage( bool bSave ) 
{
	m_bSaveAlignImage = bSave;

	return;
}


void CImageProcess_H::displayImage( bool bDisplay )
{
	m_bDisplayImage = bDisplay;

	return;
}


void CImageProcess_H::setScanOffset( double offset ) 
{
	m_dScanOffsetForward = offset;

	return;
}


void CImageProcess_H::setFilePath( char *path, int len ) 
{
	m_strFilePath = path;

	return;
}


void CImageProcess_H::setFileName( char *path, int len ) 
{
	m_strFileName = path;

	return;
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::threshold( BYTE *pImage, BYTE *pImageResult, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int lowThreshold, int highThreshold, int objectGray, int backgroundGray )
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


bool CImageProcess_H::threshold( float *pImage, BYTE *pImageResult, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, float lowThreshold, float highThreshold, int objectGray, int backgroundGray )
{
	return true;
}


bool CImageProcess_H::threshold( BYTE *pImage, BYTE *pImageResult, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, THRESHOLD_METHOD eMethod )
{
	return true;
}


bool CImageProcess_H::mean_image(BYTE *pImage, BYTE *pImage_out, int width, int height, int sx, int sy, int ex, int ey, int szFilterHalf)
{
	//
// 	HByteImage hImage( pImage, width, height );
// 
// 
// 	//
// 	HByteImage hImageOut = hImage.MeanImage( 2*szFilterHalf +1, 2*szFilterHalf +1 );
// 
// 
// 	//
// 	memcpy( pImage_out, &hImageOut[0], width*height );


	return true;
}


bool CImageProcess_H::blur(BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, CSize szFilter)
{
	//
// 	HByteImage hImage( pSrcImage, width, height );
// 
// 
// 	// ROI 설정
// 	HRegion roiRegion = HRegion::GenRectangle1( sy, sx, ey, ex );
// 	HByteImage roiImage = hImage.ReduceDomain(roiRegion);
// 
// 
// 	// Processing
// // 	HByteImage hImageOut = hImage.MeanImage( szFilter.cx, szFilter.cy );
// 	HByteImage hImageOut = roiImage.MeanImage( szFilter.cx, szFilter.cy );
// 
// 
// 	// Copy result
// 	memcpy( pRstImage, &hImageOut[0], width*height );


	return true;
}


bool CImageProcess_H::median_filter(BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, CSize szFilter)
{
	//
// 	HByteImage hImage( pSrcImage, width, height );
// 
// 
// 	// ROI 설정
// 	HRegion roiRegion = HRegion::GenRectangle1( sy, sx, ey, ex );
// 	HByteImage roiImage = hImage.ReduceDomain(roiRegion);
// 
// 
// 	//
// 	HTuple hType = "rectangle";
// 	HTuple HMargin = "mirrored";
// 	HTuple radius = 5;
// 
// 	HByteImage hImageOut = roiImage.MedianImage( hType, radius, HMargin);
// 
// 
// 	//
// 	memcpy( pRstImage, &hImageOut[0], width*height );


	return true;
}


bool CImageProcess_H::gaussian_filter(BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, CSize szFilter)
{
	//
// 	HByteImage hImage( pSrcImage, width, height );
// 
// 
// 	// ROI 설정
// 	HRegion roiRegion = HRegion::GenRectangle1( sy, sx, ey, ex );
// 	HByteImage roiImage = hImage.ReduceDomain(roiRegion);
// 
// 	//
// 	long szKenel = 5;
// 	HByteImage hImageOut = roiImage.GaussImage(szKenel);
// 
// 
// 	//
// 	memcpy( pRstImage, &hImageOut[0], width*height );


	return true;
}


bool CImageProcess_H::sobel( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey )
{
	//
// 	HByteImage hImage( pSrcImage, width, height );
// 
// 
// 	// ROI 설정
// 	HRegion roiRegion = HRegion::GenRectangle1( sy, sx, ey, ex );
// 	HByteImage roiImage = hImage.ReduceDomain(roiRegion);
// 
// 
// 	// Processing
// 	// 	HByteImage hImageOut = hImage.MeanImage( szFilter.cx, szFilter.cy );
// 	HByteImage hImageOut = roiImage.SobelAmp("sum_abs", 3);
// 
// 
// 	// Copy result
// 	memcpy( pRstImage, &hImageOut[0], width*height );


	return true;
}

bool CImageProcess_H::laplacian( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey )
{
	// Set image
// 	HByteImage hImage( pSrcImage, width, height );
// 
// 
// 	// ROI 설정
// 	HRegion roiRegion = HRegion::GenRectangle1( sy, sx, ey, ex );
// 	HByteImage roiImage = hImage.ReduceDomain(roiRegion);
// 
// 
// 	// Processing
// 	// 	HByteImage hImageOut = hImage.MeanImage( szFilter.cx, szFilter.cy );
// 	HByteImage hImageOut = roiImage.Laplace( "abs", 3,"n_4");
// 
// 
// 	// Copy result
// 	memcpy( pRstImage, &hImageOut[0], width*height );
// 

	return true;
}


bool CImageProcess_H::edge( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int iMethod, int iParam )
{
	// Set image
// 	HByteImage hImage( pSrcImage, width, height );
// 
// 
// 	// ROI 설정
// 	HRegion roiRegion = HRegion::GenRectangle1( sy, sx, ey, ex );
// 	HByteImage roiImage = hImage.ReduceDomain(roiRegion);
// 
// 
// 	// Processing
// 	HTuple Filter = "canny";
// 	HTuple Alpha = 0.5;
// 	HTuple Low = 10;
// 	HTuple High = 20;
// 
// 	HXLDContArray Edges = roiImage.EdgesSubPix( Filter, Alpha, Low, High);
// 
// 
// 	// Copy result
// 	int nEdges = Edges.Num();
// 	for( int i=0 ; i<nEdges ; i++ )
// 	{
// 		HTuple rowCont, colCont;
// 		rowCont = Edges[i].GetContourXld(&colCont);
// 
// 		// point 갯수
// 		double dData;
// 		tuple_length( rowCont, &dData );
// 
// 		for( int j=0 ; j<dData ; j++ )
// 		{
// 			long x = colCont[j];
// 			long y = rowCont[j];
// 			pRstImage[ x + y*width ] = 255;
// 		}
// 
// 	}


	return true;
}


bool CImageProcess_H::create_shape_model( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey, int szSmooth )
{
	//
// 	HByteImage hImage( pImage, width, height );
// 
// 
// 	// ROI 설정
// 	HRegion roiRegion = HRegion::GenRectangle1( sy, sx, ey, ex );
// 	HImage roiImage = hImage.ReduceDomain(roiRegion);
// 
// 
// 	//
// 	HTuple NumLevels = 3;
// 	HTuple AngleStart = -0.39;
// 	HTuple AngleExtent = 0.79;
// 	HTuple AngleStep= 0.0175;
// 	HTuple Optimization = "none";
// 	HTuple Metric = "use_polarity";
// 	HTuple Contrast = 10;
// 	HTuple MinContrast = 5;
// 
// 	g_shapeModel.CreateShapeModel(	roiImage, 
// 									NumLevels, 
// 									AngleStart, AngleExtent, AngleStep,
// 									Optimization,
// 									Metric,
// 									Contrast, MinContrast );
// 
// 
// 	// display result
// // 	HXLDContArray contours = g_shapeModel.GetShapeModelContours(1);
// // 
// // 	HWindow w;
// // 	contours.Display(w);
// // 	w.Click();
// 
// 
// 	//
// 	//
// 	//
// 	// clear model
// 	m_contourVector_shapeModel.clear();
// 
// 	// get contour
// 	HXLDContArray xldConts = g_shapeModel.GetShapeModelContours(1);
// 	long nConts=xldConts.Num();	// size
// 
// 	for( int i=0 ; i<nConts ; i++ )
// 	{
// 		//
// 		// 개별 contour로 부터 point 정보를 추출하여 저장
// 		//
// 
// 		// 임시 저장소 생성
// 		list<pointShape> tmpShapeContour;
// 		
// 		// point 정보 추출
// 		HTuple rowCont, colCont;
// // 		Hobject objCont = xldConts[i].Id();
// // 		get_contour_xld(objCont, &rowCont, &colCont );
// 		rowCont = xldConts[i].GetContourXld(&colCont);
// 
// 		// point 갯수
// 		double dData;
// 		tuple_length( rowCont, &dData );
// 
// 		// point 정보 입력
// 		for( int j=0 ; j<dData ; j++ )
// 		{
// 			pointShape tmpPt;
// 			tmpPt.x = (float)(double)colCont[j];
// 			tmpPt.y = (float)(double)rowCont[j];
// 
// 			tmpShapeContour.push_back(tmpPt);
// 		}
// 
// 		// 결과 저장
// 		m_contourVector_shapeModel.push_back(tmpShapeContour);
// 	}

	return true;
}


bool CImageProcess_H::find_shape_model( BYTE *pImage, int width, int height, UINT sx, UINT ex, UINT sy, UINT ey, float *x_pos, float *y_pos, int pyramidLevel, int method )
{
	//
// 	HByteImage hImage( pImage, width, height );
// 
// 
// 	// ROI 설정
// 	HRegion roiRegion = HRegion::GenRectangle1( sy, sx, ey, ex );
// 	HImage roiImage = hImage.ReduceDomain(roiRegion);
// 
// 
// 	//
// 	HTuple AngleStart = -0.39;
// 	HTuple AngleExtent = 0.78;
// 	HTuple MinScore = 0.5;
// 	HTuple NumMatches = 1;
// 	HTuple MaxOverlap = 0.5;
// 	HTuple SubPixel = "least_squares_high";
// 	HTuple NumLevels = 3;
// 	HTuple Greediness = 0.9;
// 	HTuple Column;
// 	HTuple Angle;
// 	HTuple Score;
// 
// 	HTuple row = roiImage.FindShapeModel(	g_shapeModel, 
// 											AngleStart, AngleExtent,
// 											MinScore,
// 											NumMatches,
// 											MaxOverlap,
// 											SubPixel,
// 											NumLevels,
// 											Greediness,
// 											&Column,
// 											&Angle,
// 											&Score );
// 
// 
// 	//
// 	*x_pos = (float)(double)Column[0];
// 	*y_pos = (float)(double)row[0];

	// display 
// 	HXLDContArray contours = g_shapeModel.GetShapeModelContours(1);
// 
// 	HTuple HomMat2D;
// 	vector_angle_to_rigid ( 0, 0, 0, row[0], Column[0], Angle[0], &HomMat2D );
// 	HXLDContArray contours2 = contours.AffineTransContourXld(HomMat2D);
// 
// 	HWindow w;
// 	hImage.Display(w);
// 	contours2.Display(w);
//  	w.Click();

	return true;
}


bool CImageProcess_H::save_shape_model(char *pFileName)
{
// 	g_shapeModel.WriteShapeModel(pFileName);

	return true;
}


bool CImageProcess_H::load_shape_model(char *pFileName)
{
// 	g_shapeModel.ReadShapeModel(pFileName);


	//
	//
	//
	// clear model
// 	m_contourVector_shapeModel.clear();
// 
// 	// get contour
// 	HXLDContArray xldConts = g_shapeModel.GetShapeModelContours(1);
// 	long nConts=xldConts.Num();	// size
// 
// 	for( int i=0 ; i<nConts ; i++ )
// 	{
// 		// 개별 contour 저장소
// 		list<pointShape> tmpShapeContour;
// 
// 
// 		// 개별 contour에 대한 데이터 추출
// 		HTuple rowCont, colCont;
// 		rowCont = xldConts[i].GetContourXld(&colCont);
// 
// 		// 추출된 데이터의 갯수
// 		double dData;
// 		tuple_length( rowCont, &dData );
// 
// 		// 정보 입력
// 		for( int j=0 ; j<dData ; j++ )
// 		{
// 			pointShape tmpPt;
// 			tmpPt.x = (float)(double)colCont[j];
// 			tmpPt.y = (float)(double)rowCont[j];
// 
// 			tmpShapeContour.push_back(tmpPt);
// 		}
// 
// 		m_contourVector_shapeModel.push_back(tmpShapeContour);
// 	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// Template image를 등록한다.
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::load_ncc_model(char *pFileName)
{
	HImage templateImage(pFileName);

	if( m_bDisplayImage )
	{
		DISPLAY( templateImage, "Template" );
	}


	m_NCCModel.CreateNccModel(	templateImage,
								0,	// Level - List of values: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, "auto"
								0,	// AngleStart - Suggested values: -3.14, -1.57, -0.79, -0.39, -0.20, 0.0
								0,	// AngleExtent -Suggested values: 6.29, 3.14, 1.57, 0.79, 0.39
								0,	// AngleStep - Suggested values: "auto", 0.0, 0.0175, 0.0349, 0.0524, 0.0698, 0.0873
								"use_polarity" // Metric - List of values: "ignore_global_polarity", "use_polarity"
								);

	m_szNCCModel = CSize( templateImage.Width(), templateImage.Height());

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::find_ncc_model( BYTE *pImage, int width, int height, int sx, int ex, int sy, int ey, float *resultX, float *resultY )
{
	// 이미지 type 변경 : c++ -> halcon
	HImage inputImage("byte", width, height, pImage);


	HTuple tRows, tCols, tAngles, tScores;

	m_NCCModel.FindNccModel(	inputImage,
								0,		// AngleStart
								0,		//  AngleExtent
								0.8,	// min score
								0,		// NumMatches
								1.0,	// MaxOverlap
								"true",	// SubPixel
								0,		// NumLevels
								&tRows,
								&tCols,
								&tAngles,
								&tScores );


	// *
	if( m_bDisplayImage )
	{
		HTuple tPhis, tLen1, tLen2;
		for( int i=0 ; i<tRows.TupleLength() ; i++ )
		{
			tPhis.Append(0);
			tLen1.Append(m_szNCCModel.cx);
			tLen2.Append(m_szNCCModel.cy);
		}

		HXLDCont xldFound;
		xldFound.GenRectangle2ContourXld(tRows, tCols, tPhis, tLen1, tLen2 );

		DISPLAY( inputImage, xldFound, "Found hole" );
	}

	// Matching 결과가 없을 경우
	int nFound = tRows.TupleLength();
	if( tRows.TupleLength() ==0 )
	{
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	PPA 측정
//
//	1. 중심 구멍 찾기
//	2. 주변 구멍 찾기
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::insp_ppa( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, MASK_INFO maskInfo, int nByn, PPA_INFO *pPpaInfo, int method )
{

	// 이미지 type 변경 : c++ -> halcon
	HImage orgImage("byte", width, height, pImage);

	if( m_bSaveOriginalImage )
	{
		CString strFile;
		strFile.Format("d:\\processing\\%d_insp_ppa", GetTickCount());
		orgImage.WriteImage("bmp", 0, strFile);
	}

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, "입력 영상");
	}


	// threshold
	HTuple thresholdGray;
	HRegion thrRegion = orgImage.BinaryThreshold( "max_separability", "light", &thresholdGray );

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, thrRegion, "threshold");
	}
	
	if( thrRegion.CountObj()==0 )
	{
		return false;
	}


	// connection
	HRegion holeRegions = thrRegion.Connection();

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, holeRegions, "connection");
	}

	if( holeRegions.CountObj()==0 )
	{
		return false;
	}


	// remove false region
	HTuple paramShapeHole, paramMinHole, paramMaxHole;
	double rotWidth, rotHeight;
	switch( maskInfo.eHoleShape )
	{
	case HOLE_SHAPE_RECTANGLE:
	case HOLE_SHAPE_DIAMOND:
	case HOLE_SHAPE_CIRCLE:
	case HOLE_SHAPE_ELLIPSE:
		paramShapeHole[0]	= "width";
		paramMinHole[0]		= maskInfo.dRealHoleWidth/maskInfo.dResolution*0.8;
		paramMaxHole[0]		= maskInfo.dRealHoleWidth/maskInfo.dResolution*1.2;

		paramShapeHole[1]	= "height";
		paramMinHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*0.8;
		paramMaxHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*1.2;
		break;

	case HOLE_SHAPE_ELLIPSE_45:
		paramShapeHole[0]	= "area";
		paramMinHole[0]		= (maskInfo.dRealHoleWidth/maskInfo.dResolution)*(maskInfo.dRealHoleHeight/maskInfo.dResolution)*0.5;
		paramMaxHole[0]		= (maskInfo.dRealHoleWidth/maskInfo.dResolution)*(maskInfo.dRealHoleHeight/maskInfo.dResolution)*1.2;

		break;

	case HOLE_SHAPE_RECTANGLE_45:
		paramShapeHole[0]	= "width";
		rotWidth = max( maskInfo.dRealHoleWidth*cos(PI*45/180), maskInfo.dRealHoleHeight*sin(PI*45/180) );
		paramMinHole[0]		= rotWidth/maskInfo.dResolution*0.8;
		paramMaxHole[0]		= rotWidth/maskInfo.dResolution*1.2;

		paramShapeHole[1]	= "height";
		rotHeight = max( maskInfo.dRealHoleWidth*sin(PI*45/180), maskInfo.dRealHoleHeight*cos(PI*45/180) );
		paramMinHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*0.8;
		paramMaxHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*1.2;
		break;

	default:
		paramShapeHole[0]	= "width";
		paramMinHole[0]		= maskInfo.dRealHoleWidth/maskInfo.dResolution*0.8;
		paramMaxHole[0]		= maskInfo.dRealHoleWidth/maskInfo.dResolution*1.2;

		paramShapeHole[1]	= "height";
		paramMinHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*0.8;
		paramMaxHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*1.2;
	}

	HRegion validHoleRegions = holeRegions.SelectShape( paramShapeHole, "and", paramMinHole, paramMaxHole );

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, validHoleRegions, "유효 구멍들");
	}

	if( validHoleRegions.CountObj()==0 )
	{
		return false;
	}


	// select hole at center
	HTuple roiHoleIndex;
	findNearRegionIndex(validHoleRegions, height/2, width/2, &roiHoleIndex);
	HRegion roiHoleRegion = validHoleRegions.SelectObj(roiHoleIndex+1);

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, roiHoleRegion, "가운데 구멍");
	}

	if( roiHoleRegion.CountObj()==0 )
	{
		return false;
	}

	//
	HTuple offsetX = roiHoleRegion.Column() - width/2.0;	// pixel
	HTuple offsetY = roiHoleRegion.Row() - height/2.0;		// pixel
	int pitchPixel = (int)(maskInfo.dRealPitchX/maskInfo.dResolution);


	//
	//
	// PPA 정보 추출
	//
	//
	int iRange = max( nByn/2, 0 );
	HTuple xPpa, yPpa, widthCd, heightCd;
	int nData = 0;
	for( int y=-iRange ; y<=iRange ; y++ )
	{
		for( int x=-iRange ; x<=iRange ; x++ )
		{
			// Hole의 예상위치
			double targetX = width/2.0	+ x*maskInfo.dRealPitchX/maskInfo.dResolution;
			double targetY = height/2.0 + y*maskInfo.dRealPitchY/maskInfo.dResolution;

			//
			bool bValid = false;
			for( int i=0 ; i<validHoleRegions.CountObj() ; i++ )
			{
				// Target 위치에 있는 hole region을 찾는다.
				roiHoleRegion = validHoleRegions.SelectObj(i+1);

				if( !roiHoleRegion.TestRegionPoint( targetY + offsetY, targetX + offsetX ) )
				{
					continue;
				}


				// PPA 값 계산
				double ppaX = (roiHoleRegion.Column() - targetX)*maskInfo.dResolution;
				double ppaY = (roiHoleRegion.Row() - targetY)*maskInfo.dResolution;
				xPpa.Append(ppaX);
				yPpa.Append(ppaY);
				pPpaInfo->dPpaX[nData] = ppaX;
				pPpaInfo->dPpaY[nData] = ppaY;


				//////////////////////////////////////////////////////////////////////////
				// CD값 계산
				if( maskInfo.eHoleMeasureMethod==MEASURE_METHOD_EDGE )
				{
					// Edge 검색 영역 생성 : Hole 영역의 edge
					HRegion searchRegion = roiHoleRegion.DilationCircle(5.5).Difference( roiHoleRegion.ErosionCircle(5.5) );

					// ROI 영역 설정
					HImage roiImage = orgImage.ReduceDomain(searchRegion);

					// Edge 검출
					HXLDCont edgeXld = roiImage.EdgesSubPix("lanser1", 1, 50, 80);

					// 화면 표시
					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, edgeXld, "Edge 영상");
					}

					if( edgeXld.CountObj()<1 )
					{
						return false;
					}

					// Edge 선택 : 가장 긴 edge
					HTuple tempLength = 0;
					HTuple rowMBR, colMBR, phiMBR, len1MBR, len2MBR;
					for( int i=0 ; i<edgeXld.CountObj() ; i++ )
					{
						if( edgeXld.SelectObj(i+1).LengthXld()>tempLength )
						{
							tempLength = edgeXld.SelectObj(i+1).LengthXld();

// 							edgeXld.SelectObj(i+1).SmallestRectangle1Xld( &row1, &col1, &row2, &col2 );
							edgeXld.SelectObj(i+1).SmallestRectangle2Xld(&rowMBR, &colMBR, &phiMBR, &len1MBR, &len2MBR);

							if( m_bDisplayImage )
							{
								HXLDCont temp;
								temp.GenRectangle2ContourXld(rowMBR, colMBR, phiMBR, len1MBR, len2MBR);
								DISPLAY(orgImage, temp, "MBR 영상");
							}
						}
					}


					double cdWidth = 2*len1MBR*maskInfo.dResolution;
					double cdHeight = 2*len2MBR*maskInfo.dResolution;
					widthCd.Append( cdWidth );
					heightCd.Append( cdHeight );
					pPpaInfo->dCdWidth[nData] = cdWidth;
					pPpaInfo->dCdHeight[nData] = cdHeight;
				} 
				else if( maskInfo.eHoleMeasureMethod==MEASURE_METHOD_PAIR )
				{
					// MBR 생성(회전)
					HTuple rowMBR, colMBR, phiMBR, len1MBR, len2MBR;
					roiHoleRegion.SmallestRectangle2( &rowMBR, &colMBR, &phiMBR, &len1MBR, &len2MBR);

					// measure 생성
					HMeasure measureX( rowMBR, colMBR, phiMBR+(HTuple(90).TupleRad()), len2MBR + HTuple(20), HTuple(30), HTuple(width), HTuple(height), HString("nearest_neighbor") ); 

					//
					HXLDCont xldRectX;
					xldRectX.GenRectangle2ContourXld( rowMBR, colMBR, phiMBR+(HTuple(90).TupleRad()), len2MBR + HTuple(20), HTuple(30) );

					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectX, "X 측정자 생성");
					}


					//
					HTuple RowEdgeFirst, ColumnEdgeFirst, AmplitudeFirst, RowEdgeSecond, ColumnEdgeSecond, AmplitudeSecond, IntraDistance, InterDistance;
					measureX.MeasurePairs(orgImage, 1.0, 30, "positive", "all", &RowEdgeFirst, &ColumnEdgeFirst, &AmplitudeFirst, &RowEdgeSecond, &ColumnEdgeSecond, &AmplitudeSecond, &IntraDistance, &InterDistance);

					if( IntraDistance.Length()==0 )
					{
						return false;
					}

					xldRectX.GenRectangle2ContourXld( RowEdgeFirst[0], ColumnEdgeFirst[0], phiMBR, HTuple(20), HTuple(0) );
					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectX, "edge 1");
					}

					xldRectX.GenRectangle2ContourXld( RowEdgeSecond[0], ColumnEdgeSecond[0], phiMBR, HTuple(20), HTuple(0) );
					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectX, "edge 2");
					}

					double cdWidth = IntraDistance[0]*maskInfo.dResolution;
					widthCd.Append( cdWidth );
					pPpaInfo->dCdWidth[nData] = cdWidth;


					// measure 생성
					HMeasure measureY( rowMBR, colMBR, phiMBR, len2MBR + HTuple(20), HTuple(30), HTuple(width), HTuple(height), HString("nearest_neighbor") ); 

					HXLDCont xldRectY;
					xldRectY.GenRectangle2ContourXld( rowMBR, colMBR, phiMBR, len2MBR + HTuple(20), HTuple(30) );

					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectY, "Y 측정자 생성");
					}


					//
					measureY.MeasurePairs(orgImage, 1.0, 30, "positive", "all", &RowEdgeFirst, &ColumnEdgeFirst, &AmplitudeFirst, &RowEdgeSecond, &ColumnEdgeSecond, &AmplitudeSecond, &IntraDistance, &InterDistance);

					if( IntraDistance.Length()==0 )
					{
						return false;
					}

					xldRectY.GenRectangle2ContourXld( RowEdgeFirst[0], ColumnEdgeFirst[0], phiMBR+(HTuple(90).TupleRad()), HTuple(20), HTuple(0) );
					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectY, "edge 1");
					}

					xldRectY.GenRectangle2ContourXld( RowEdgeSecond[0], ColumnEdgeSecond[0], phiMBR+(HTuple(90).TupleRad()), HTuple(20), HTuple(0) );
					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectY, "edge 2");
					}

					double cdHeight = IntraDistance[0]*maskInfo.dResolution;
					heightCd.Append( cdHeight );
					pPpaInfo->dCdHeight[nData] = cdHeight;
				}
				//
				//////////////////////////////////////////////////////////////////////////



// 				//////////////////////////////////////////////////////////////////////////
// 				// CD값 계산
// 				if( maskInfo.eHoleShape!=HOLE_SHAPE_ELLIPSE_45 )
// 				{	// edge를 이용하여 CD를 측정한다.
// 					HRegion searchRegion = roiHoleRegion.DilationCircle(5.5).Difference( roiHoleRegion.ErosionCircle(5.5) );
// 
// 					HImage roiImage = orgImage.ReduceDomain(searchRegion);
// 
// 					HXLDCont edgeXld = roiImage.EdgesSubPix("lanser1", 1, 50, 80);
// 
// 					if( m_bProcessingImage )
// 					{
// 						DISPLAY(orgImage, edgeXld, "Edge 영상");
// 					}
// 
// 					if( edgeXld.CountObj()<1 )
// 					{
// 						return false;
// 					}
// 
// 					HTuple tempLength = 0;
// 					HTuple row1, row2, col1, col2;
// 					for( int i=0 ; i<edgeXld.CountObj() ; i++ )
// 					{
// 						if( edgeXld.SelectObj(i+1).LengthXld()>tempLength )
// 						{
// 							tempLength = edgeXld.SelectObj(i+1).LengthXld();
// 
// 							edgeXld.SelectObj(i+1).SmallestRectangle1Xld( &row1, &col1, &row2, &col2 );
// 						}
// 					}
// 
// 					
// 					double cdWidth = (col2 - col1)*maskInfo.dResolution;
// 					double cdHeight = (row2 - row1)*maskInfo.dResolution;
// 					widthCd.Append( cdWidth );
// 					heightCd.Append( cdHeight );
// 					pPpaInfo->dCdWidth[nData] = cdWidth;
// 					pPpaInfo->dCdHeight[nData] = cdHeight;
// 				}
// 				else
// 				{	// Edge를 이용하여 CD를 측정한다.
// 					HRegion searchRegion = roiHoleRegion.DilationCircle(5.5).Difference( roiHoleRegion.ErosionCircle(5.5) );
// 
// 					HImage roiImage = orgImage.ReduceDomain(searchRegion);
// 
// 					HXLDCont edgeXld = roiImage.EdgesSubPix("lanser2_junctions", 1, 50, 100);
// 
// 					if( m_bProcessingImage )
// 					{
// 						DISPLAY(orgImage, edgeXld, "Edge 영상");
// 					}
// 
// 					if( edgeXld.CountObj()<1 )
// 					{
// 						return false;
// 					}
// 
// 					HTuple tempLength = 0;
// 					HTuple rowMBR, colMBR, phiMBR, len1MBR, len2MBR;
// 					for( int i=0 ; i<edgeXld.CountObj() ; i++ )
// 					{
// 						if( edgeXld.SelectObj(i+1).LengthXld()>tempLength )
// 						{
// 							tempLength = edgeXld.SelectObj(i+1).LengthXld();
// 
// // 							edgeXld.SelectObj(i+1).SmallestRectangle1Xld( &row1, &col1, &row2, &col2 );
// 							edgeXld.SelectObj(i+1).SmallestRectangle2Xld(&rowMBR, &colMBR, &phiMBR, &len1MBR, &len2MBR);
// 
// 							if( m_bProcessingImage )
// 							{
// 								HXLDCont temp;
// 								temp.GenRectangle2ContourXld(rowMBR, colMBR, phiMBR, len1MBR, len2MBR);
// 								DISPLAY(orgImage, temp, "MBR 영상");
// 							}
// 						}
// 					}
// 
// 
// 					double cdWidth = 2*len1MBR*maskInfo.dResolution;
// 					double cdHeight = 2*len2MBR*maskInfo.dResolution;
// 					widthCd.Append( cdWidth );
// 					heightCd.Append( cdHeight );
// 					pPpaInfo->dCdWidth[nData] = cdWidth;
// 					pPpaInfo->dCdHeight[nData] = cdHeight;
// 
// /*					
// 					// 측정자 이용방법
// 					// MBR
// 					HTuple rowMBR, colMBR, phiMBR, len1MBR, len2MBR;
// 					roiHoleRegion.SmallestRectangle2( &rowMBR, &colMBR, &phiMBR, &len1MBR, &len2MBR);
// 
// 					// measure 생성
// 					HMeasure measureX( rowMBR, colMBR, phiMBR+(HTuple(90).TupleRad()), len2MBR + HTuple(20), HTuple(30), HTuple(width), HTuple(height), HString("nearest_neighbor") ); 
// 
// 					HXLDCont xldRectX;
// 					xldRectX.GenRectangle2ContourXld( rowMBR, colMBR, phiMBR+(HTuple(90).TupleRad()), len2MBR + HTuple(20), HTuple(30) );
// 
// 					if( m_bProcessingImage )
// 					{
// 						DISPLAY(orgImage, xldRectX, "X 측정자 생성");
// 					}
// 
// 
// 					//
// 					HTuple RowEdgeFirst, ColumnEdgeFirst, AmplitudeFirst, RowEdgeSecond, ColumnEdgeSecond, AmplitudeSecond, IntraDistance, InterDistance;
// 					measureX.MeasurePairs(orgImage, 1.0, 30, "positive", "all", &RowEdgeFirst, &ColumnEdgeFirst, &AmplitudeFirst, &RowEdgeSecond, &ColumnEdgeSecond, &AmplitudeSecond, &IntraDistance, &InterDistance);
// 
// 					if( IntraDistance.Length()==0 )
// 					{
// 						return false;
// 					}
// 
// 					xldRectX.GenRectangle2ContourXld( RowEdgeFirst[0], ColumnEdgeFirst[0], phiMBR, HTuple(20), HTuple(0) );
// 					if( m_bProcessingImage )
// 					{
// 						DISPLAY(orgImage, xldRectX, "edge 1");
// 					}
// 
// 					xldRectX.GenRectangle2ContourXld( RowEdgeSecond[0], ColumnEdgeSecond[0], phiMBR, HTuple(20), HTuple(0) );
// 					if( m_bProcessingImage )
// 					{
// 						DISPLAY(orgImage, xldRectX, "edge 2");
// 					}
// 
// 					double cdWidth = IntraDistance[0]*maskInfo.dResolution;
// 					widthCd.Append( cdWidth );
// 					pPpaInfo->dCdWidth[nData] = cdWidth;
// 
// 
// 					// measure 생성
// 					HMeasure measureY( rowMBR, colMBR, phiMBR, len2MBR + HTuple(20), HTuple(30), HTuple(width), HTuple(height), HString("nearest_neighbor") ); 
// 
// 					HXLDCont xldRectY;
// 					xldRectY.GenRectangle2ContourXld( rowMBR, colMBR, phiMBR, len2MBR + HTuple(20), HTuple(30) );
// 
// 					if( m_bProcessingImage )
// 					{
// 						DISPLAY(orgImage, xldRectY, "Y 측정자 생성");
// 					}
// 
// 
// 					//
// 					measureY.MeasurePairs(orgImage, 1.0, 30, "positive", "all", &RowEdgeFirst, &ColumnEdgeFirst, &AmplitudeFirst, &RowEdgeSecond, &ColumnEdgeSecond, &AmplitudeSecond, &IntraDistance, &InterDistance);
// 
// 					if( IntraDistance.Length()==0 )
// 					{
// 						return false;
// 					}
// 
// 					xldRectY.GenRectangle2ContourXld( RowEdgeFirst[0], ColumnEdgeFirst[0], phiMBR+(HTuple(90).TupleRad()), HTuple(20), HTuple(0) );
// 					if( m_bProcessingImage )
// 					{
// 						DISPLAY(orgImage, xldRectY, "edge 1");
// 					}
// 
// 					xldRectY.GenRectangle2ContourXld( RowEdgeSecond[0], ColumnEdgeSecond[0], phiMBR+(HTuple(90).TupleRad()), HTuple(20), HTuple(0) );
// 					if( m_bProcessingImage )
// 					{
// 						DISPLAY(orgImage, xldRectY, "edge 2");
// 					}
// 
// 					double cdHeight = IntraDistance[0]*maskInfo.dResolution;
// 					heightCd.Append( cdHeight );
// 					pPpaInfo->dCdHeight[nData] = cdHeight;*/
// 				}
// 				//
// 				//////////////////////////////////////////////////////////////////////////

				nData++;

				bValid = true;

				if( nData>25 )
				{
					return false;
				}
			}

			// 측정이 않된 경우 0값으로 설정
			if( !bValid )
			{
				pPpaInfo->dPpaX[nData] = 0;
				pPpaInfo->dPpaY[nData] = 0;
				pPpaInfo->dCdWidth[nData] = 0;
				pPpaInfo->dCdHeight[nData] = 0;
				nData++;
			}
		}
	}


	//
	pPpaInfo->dPpaXAvg = xPpa.TupleMean();
	pPpaInfo->dPpaXMin = xPpa.TupleMin();
	pPpaInfo->dPpaXMax = xPpa.TupleMax();
	pPpaInfo->dPpaXStd = xPpa.TupleDeviation();

	pPpaInfo->dPpaYAvg = yPpa.TupleMean();
	pPpaInfo->dPpaYMin = yPpa.TupleMin();
	pPpaInfo->dPpaYMax = yPpa.TupleMax();
	pPpaInfo->dPpaYStd = yPpa.TupleDeviation();

	pPpaInfo->dCdWidthAvg = widthCd.TupleMean();
	pPpaInfo->dCdWidthMin = widthCd.TupleMin();
	pPpaInfo->dCdWidthMax = widthCd.TupleMax();
	pPpaInfo->dCdWidthStd = widthCd.TupleDeviation();

	pPpaInfo->dCdHeightAvg = heightCd.TupleMean();
	pPpaInfo->dCdHeightMin = heightCd.TupleMin();
	pPpaInfo->dCdHeightMax = heightCd.TupleMax();
	pPpaInfo->dCdHeightStd = heightCd.TupleDeviation();

	pPpaInfo->nPoint = nData;

	CString strMessage;
	strMessage.Format("PPA 측정 > X > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dPpaXAvg, pPpaInfo->dPpaXMin, pPpaInfo->dPpaXMax, pPpaInfo->dPpaXStd );
	OutputDebugString(strMessage);

	strMessage.Format("PPA 측정 > Y > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dPpaYAvg, pPpaInfo->dPpaYMin, pPpaInfo->dPpaYMax, pPpaInfo->dPpaYStd );
	OutputDebugString(strMessage);

	strMessage.Format("PPA 측정 > width > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dCdWidthAvg, pPpaInfo->dCdWidthMin, pPpaInfo->dCdWidthMax, pPpaInfo->dCdWidthStd );
	OutputDebugString(strMessage);

	strMessage.Format("PPA 측정 > height > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dCdHeightAvg, pPpaInfo->dCdHeightMin, pPpaInfo->dCdHeightMax, pPpaInfo->dCdHeightStd );
	OutputDebugString(strMessage);


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	PPA 측정
//
//	1. 중심 구멍 찾기
//	2. 주변 구멍 찾기
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::insp_ppa_tracking( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, MASK_INFO maskInfo, int nByn, PPA_INFO *pPpaInfo, double dOldPpaX, double dOldPpaY, int method )
{
	CString str;
	str.Format("insp_ppa_tracking : old x=%f, old y=%f", dOldPpaX, dOldPpaY );
	OutputDebugString(str);


	// 이미지 type 변경 : c++ -> halcon
	HImage orgImage("byte", width, height, pImage);

	if( m_bSaveOriginalImage )
	{
		CString strFile;
		strFile.Format("d:\\processing\\%d_insp_ppa_old x_%f_old y_%f", GetTickCount(), dOldPpaX, dOldPpaY );
		orgImage.WriteImage("bmp", 0, strFile);
	}

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, "입력 영상");
	}


	// threshold
	HTuple thresholdGray;
	HRegion thrRegion = orgImage.BinaryThreshold( "max_separability", "light", &thresholdGray );

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, thrRegion, "threshold");
	}

	if( thrRegion.CountObj()==0 )
	{
		return false;
	}


	// connection
	HRegion holeRegions = thrRegion.Connection();

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, holeRegions, "connection");
	}

	if( holeRegions.CountObj()==0 )
	{
		return false;
	}


	// remove false region
	HTuple paramShapeHole, paramMinHole, paramMaxHole;
	double rotWidth, rotHeight;
	switch( maskInfo.eHoleShape )
	{
	case HOLE_SHAPE_RECTANGLE:
	case HOLE_SHAPE_DIAMOND:
	case HOLE_SHAPE_CIRCLE:
	case HOLE_SHAPE_ELLIPSE:
		paramShapeHole[0]	= "width";
		paramMinHole[0]		= maskInfo.dRealHoleWidth/maskInfo.dResolution*0.8;
		paramMaxHole[0]		= maskInfo.dRealHoleWidth/maskInfo.dResolution*2.0;

		paramShapeHole[1]	= "height";
		paramMinHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*0.8;
		paramMaxHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*2.0;
		break;

	case HOLE_SHAPE_ELLIPSE_45:
		paramShapeHole[0]	= "area";
		paramMinHole[0]		= (maskInfo.dRealHoleWidth/maskInfo.dResolution)*(maskInfo.dRealHoleHeight/maskInfo.dResolution)*0.5;
		paramMaxHole[0]		= (maskInfo.dRealHoleWidth/maskInfo.dResolution)*(maskInfo.dRealHoleHeight/maskInfo.dResolution)*2.0;

		break;

	case HOLE_SHAPE_RECTANGLE_45:
		paramShapeHole[0]	= "width";
		rotWidth = max( maskInfo.dRealHoleWidth*cos(PI*45/180), maskInfo.dRealHoleHeight*sin(PI*45/180) );
		paramMinHole[0]		= rotWidth/maskInfo.dResolution*0.8;
		paramMaxHole[0]		= rotWidth/maskInfo.dResolution*2.0;

		paramShapeHole[1]	= "height";
		rotHeight = max( maskInfo.dRealHoleWidth*sin(PI*45/180), maskInfo.dRealHoleHeight*cos(PI*45/180) );
		paramMinHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*0.8;
		paramMaxHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*2.0;
		break;

	default:
		paramShapeHole[0]	= "width";
		paramMinHole[0]		= maskInfo.dRealHoleWidth/maskInfo.dResolution*0.8;
		paramMaxHole[0]		= maskInfo.dRealHoleWidth/maskInfo.dResolution*2.0;

		paramShapeHole[1]	= "height";
		paramMinHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*0.8;
		paramMaxHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*2.0;
	}

	HRegion validHoleRegions = holeRegions.SelectShape( paramShapeHole, "and", paramMinHole, paramMaxHole );

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, validHoleRegions, "유효 구멍들");
	}

	if( validHoleRegions.CountObj()==0 )
	{
		return false;
	}


	// select hole at center
	HTuple roiHoleIndex;
// 	findNearRegionIndex(validHoleRegions, height/2, width/2, &roiHoleIndex);
	findNearRegionIndex(validHoleRegions, height/2 + dOldPpaY/maskInfo.dResolution, width/2 + dOldPpaX/maskInfo.dResolution, &roiHoleIndex);
	HRegion roiHoleRegion = validHoleRegions.SelectObj(roiHoleIndex+1);
	HRegion rgRoiHole = roiHoleRegion;

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, roiHoleRegion, "가운데 구멍");
	}

	if( roiHoleRegion.CountObj()==0 )
	{
		return false;
	}

	//
	HTuple offsetX = roiHoleRegion.Column() - width/2.0;	// pixel
	HTuple offsetY = roiHoleRegion.Row() - height/2.0;		// pixel
	int pitchPixel = (int)(maskInfo.dRealPitchX/maskInfo.dResolution);


	//
	//
	// PPA 정보 추출
	//
	//
	int iRange = max( nByn/2, 0 );
	HTuple xPpa, yPpa, widthCd, heightCd;
	int nData = 0;
	for( int y=-iRange ; y<=iRange ; y++ )
	{
		for( int x=-iRange ; x<=iRange ; x++ )
		{
			// Hole의 예상위치
			double targetX = width/2.0 + x*maskInfo.dRealPitchX/maskInfo.dResolution;
			double targetY = height/2.0 + y*maskInfo.dRealPitchY/maskInfo.dResolution;
// 			double targetXTracking = width/2.0 + dOldPpaX/maskInfo.dResolution	+ x*maskInfo.dRealPitchX/maskInfo.dResolution;
// 			double targetYTracking = height/2.0 + dOldPpaY/maskInfo.dResolution + y*maskInfo.dRealPitchY/maskInfo.dResolution;

			//
			bool bValid = false;
			for( int i=0 ; i<validHoleRegions.CountObj() ; i++ )
			{
				// Target 위치에 있는 hole region을 찾는다.
				roiHoleRegion = validHoleRegions.SelectObj(i+1);

				if( !roiHoleRegion.TestRegionPoint( targetY + offsetY, targetX + offsetX ) )
// 				if( !roiHoleRegion.TestRegionPoint( targetYTracking + offsetY, targetXTracking + offsetX ) )	// tracking
				{
					continue;
				}


				// PPA 값 계산
				double ppaX = (roiHoleRegion.Column() - targetX)*maskInfo.dResolution;
				double ppaY = (roiHoleRegion.Row() - targetY)*maskInfo.dResolution;
				xPpa.Append(ppaX);
				yPpa.Append(ppaY);
				pPpaInfo->dPpaX[nData] = ppaX;
				pPpaInfo->dPpaY[nData] = ppaY;


				//////////////////////////////////////////////////////////////////////////
				// CD값 계산
				if( maskInfo.eHoleMeasureMethod==MEASURE_METHOD_EDGE )
				{
					// Edge 검색 영역 생성 : Hole 영역의 edge
					HRegion searchRegion = roiHoleRegion.DilationCircle(5.5).Difference( roiHoleRegion.ErosionCircle(5.5) );

					// ROI 영역 설정
					HImage roiImage = orgImage.ReduceDomain(searchRegion);

					// Edge 검출
					HXLDCont edgeXld = roiImage.EdgesSubPix("lanser1", 1, 50, 80);

					// 화면 표시
					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, edgeXld, "Edge 영상");
					}

					if( edgeXld.CountObj()<1 )
					{
						return false;
					}

					// Edge 선택 : 가장 긴 edge
					HTuple tempLength = 0;
					HTuple rowMBR, colMBR, phiMBR, len1MBR, len2MBR;
					for( int i=0 ; i<edgeXld.CountObj() ; i++ )
					{
						if( edgeXld.SelectObj(i+1).LengthXld()>tempLength )
						{
							tempLength = edgeXld.SelectObj(i+1).LengthXld();

							// 							edgeXld.SelectObj(i+1).SmallestRectangle1Xld( &row1, &col1, &row2, &col2 );
							edgeXld.SelectObj(i+1).SmallestRectangle2Xld(&rowMBR, &colMBR, &phiMBR, &len1MBR, &len2MBR);

							if( m_bDisplayImage )
							{
								HXLDCont temp;
								temp.GenRectangle2ContourXld(rowMBR, colMBR, phiMBR, len1MBR, len2MBR);
								DISPLAY(orgImage, temp, "MBR 영상");
							}
						}
					}


					double cdWidth = 2*len1MBR*maskInfo.dResolution;
					double cdHeight = 2*len2MBR*maskInfo.dResolution;
					widthCd.Append( cdWidth );
					heightCd.Append( cdHeight );
					pPpaInfo->dCdWidth[nData] = cdWidth;
					pPpaInfo->dCdHeight[nData] = cdHeight;
				} 
				else if( maskInfo.eHoleMeasureMethod==MEASURE_METHOD_PAIR )
				{
					// MBR 생성(회전)
					HTuple rowMBR, colMBR, phiMBR, len1MBR, len2MBR;
					roiHoleRegion.SmallestRectangle2( &rowMBR, &colMBR, &phiMBR, &len1MBR, &len2MBR);

					// measure 생성
					HMeasure measureX( rowMBR, colMBR, phiMBR+(HTuple(90).TupleRad()), len2MBR + HTuple(20), HTuple(30), HTuple(width), HTuple(height), HString("nearest_neighbor") ); 

					//
					HXLDCont xldRectX;
					xldRectX.GenRectangle2ContourXld( rowMBR, colMBR, phiMBR+(HTuple(90).TupleRad()), len2MBR + HTuple(20), HTuple(30) );

					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectX, "X 측정자 생성");
					}


					//
					HTuple RowEdgeFirst, ColumnEdgeFirst, AmplitudeFirst, RowEdgeSecond, ColumnEdgeSecond, AmplitudeSecond, IntraDistance, InterDistance;
					measureX.MeasurePairs(orgImage, 1.0, 30, "positive", "all", &RowEdgeFirst, &ColumnEdgeFirst, &AmplitudeFirst, &RowEdgeSecond, &ColumnEdgeSecond, &AmplitudeSecond, &IntraDistance, &InterDistance);

					if( IntraDistance.Length()==0 )
					{
						return false;
					}

					xldRectX.GenRectangle2ContourXld( RowEdgeFirst[0], ColumnEdgeFirst[0], phiMBR, HTuple(20), HTuple(0) );
					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectX, "edge 1");
					}

					xldRectX.GenRectangle2ContourXld( RowEdgeSecond[0], ColumnEdgeSecond[0], phiMBR, HTuple(20), HTuple(0) );
					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectX, "edge 2");
					}

					double cdWidth = IntraDistance[0]*maskInfo.dResolution;
					widthCd.Append( cdWidth );
					pPpaInfo->dCdWidth[nData] = cdWidth;


					// measure 생성
					HMeasure measureY( rowMBR, colMBR, phiMBR, len2MBR + HTuple(20), HTuple(30), HTuple(width), HTuple(height), HString("nearest_neighbor") ); 

					HXLDCont xldRectY;
					xldRectY.GenRectangle2ContourXld( rowMBR, colMBR, phiMBR, len2MBR + HTuple(20), HTuple(30) );

					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectY, "Y 측정자 생성");
					}


					//
					measureY.MeasurePairs(orgImage, 1.0, 30, "positive", "all", &RowEdgeFirst, &ColumnEdgeFirst, &AmplitudeFirst, &RowEdgeSecond, &ColumnEdgeSecond, &AmplitudeSecond, &IntraDistance, &InterDistance);

					if( IntraDistance.Length()==0 )
					{
						return false;
					}

					xldRectY.GenRectangle2ContourXld( RowEdgeFirst[0], ColumnEdgeFirst[0], phiMBR+(HTuple(90).TupleRad()), HTuple(20), HTuple(0) );
					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectY, "edge 1");
					}

					xldRectY.GenRectangle2ContourXld( RowEdgeSecond[0], ColumnEdgeSecond[0], phiMBR+(HTuple(90).TupleRad()), HTuple(20), HTuple(0) );
					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectY, "edge 2");
					}

					double cdHeight = IntraDistance[0]*maskInfo.dResolution;
					heightCd.Append( cdHeight );
					pPpaInfo->dCdHeight[nData] = cdHeight;
				}
				//
				//////////////////////////////////////////////////////////////////////////


				nData++;

				bValid = true;

				if( nData>25 )
				{
					return false;
				}
			}

			// 측정이 않된 경우 0값으로 설정
			if( !bValid )
			{
				pPpaInfo->dPpaX[nData] = 0;
				pPpaInfo->dPpaY[nData] = 0;
				pPpaInfo->dCdWidth[nData] = 0;
				pPpaInfo->dCdHeight[nData] = 0;
				nData++;

				xPpa.Append(0);
				yPpa.Append(0);
				widthCd.Append(0);
				heightCd.Append(0);
			}
		}
	}


	//
	pPpaInfo->dPpaXAvg = xPpa.TupleMean();
	pPpaInfo->dPpaXMin = xPpa.TupleMin();
	pPpaInfo->dPpaXMax = xPpa.TupleMax();
	pPpaInfo->dPpaXStd = xPpa.TupleDeviation();

	pPpaInfo->dPpaYAvg = yPpa.TupleMean();
	pPpaInfo->dPpaYMin = yPpa.TupleMin();
	pPpaInfo->dPpaYMax = yPpa.TupleMax();
	pPpaInfo->dPpaYStd = yPpa.TupleDeviation();

	pPpaInfo->dCdWidthAvg = widthCd.TupleMean();
	pPpaInfo->dCdWidthMin = widthCd.TupleMin();
	pPpaInfo->dCdWidthMax = widthCd.TupleMax();
	pPpaInfo->dCdWidthStd = widthCd.TupleDeviation();

	pPpaInfo->dCdHeightAvg = heightCd.TupleMean();
	pPpaInfo->dCdHeightMin = heightCd.TupleMin();
	pPpaInfo->dCdHeightMax = heightCd.TupleMax();
	pPpaInfo->dCdHeightStd = heightCd.TupleDeviation();

	pPpaInfo->nPoint = nData;

	CString strMessage;
	strMessage.Format("PPA 측정 > X > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dPpaXAvg, pPpaInfo->dPpaXMin, pPpaInfo->dPpaXMax, pPpaInfo->dPpaXStd );
	OutputDebugString(strMessage);

	strMessage.Format("PPA 측정 > Y > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dPpaYAvg, pPpaInfo->dPpaYMin, pPpaInfo->dPpaYMax, pPpaInfo->dPpaYStd );
	OutputDebugString(strMessage);

	strMessage.Format("PPA 측정 > width > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dCdWidthAvg, pPpaInfo->dCdWidthMin, pPpaInfo->dCdWidthMax, pPpaInfo->dCdWidthStd );
	OutputDebugString(strMessage);

	strMessage.Format("PPA 측정 > height > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dCdHeightAvg, pPpaInfo->dCdHeightMin, pPpaInfo->dCdHeightMax, pPpaInfo->dCdHeightStd );
	OutputDebugString(strMessage);


	// 확인용 > PPA 측정 결과 저장
// 	savePpaResultImage( orgImage,									// 원본 영상
// 						rgRoiHole,									// PPA를 측정한 구멍 영역
// 						width/2 + dOldPpaX/maskInfo.dResolution,	// 이전 PPA 위치( X 영상 좌표 )
// 						height/2 + dOldPpaY/maskInfo.dResolution,	// 이전 PPA 위치( Y 영상 좌표 )
// 						m_strFilePath,								// 저장 시 위치
// 						m_strFileName );							// 저장 시 이름


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// 특정 위치에 대한 구멍 위치 측정
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::insp_ppa_pos( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, MASK_INFO maskInfo, int iPos, int xTarget, int yTarget, int nByn, PPA_INFO *pPpaInfo, int method )
{
	CString str;
	str.Format("insp_ppa_pos : Start" );
	OutputDebugString(str);


	// 이미지 type 변경 : c++ -> halcon
	HImage orgImage("byte", width, height, pImage);


	// 입력 영상 저장
	if( m_bSaveOriginalImage )
	{
		CString strFile;
		strFile.Format("d:\\processing\\%d_insp_ppa_pos", GetTickCount() );
		orgImage.WriteImage("bmp", 0, strFile);
	}

	// 입력 영상 화면 표시
	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, "입력 영상");
	}


	//
	// threshold
	//
	HTuple thresholdGray;
	HRegion thrRegion = orgImage.BinaryThreshold( "max_separability", "light", &thresholdGray );

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, thrRegion, "threshold");
	}

	if( thrRegion.CountObj()==0 )
	{
		return false;
	}


	//
	// connection
	//
	HRegion holeRegions = thrRegion.Connection();

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, holeRegions, "connection");
	}

	if( holeRegions.CountObj()==0 )
	{
		return false;
	}


	//
	// remove false region
	//
	HTuple paramShapeHole, paramMinHole, paramMaxHole;
	double rotWidth, rotHeight;
	switch( maskInfo.eHoleShape )
	{
	case HOLE_SHAPE_RECTANGLE:
	case HOLE_SHAPE_DIAMOND:
	case HOLE_SHAPE_ELLIPSE:
		paramShapeHole[0]	= "width";
		paramMinHole[0]		= maskInfo.dRealHoleWidth/maskInfo.dResolution*0.8;
		paramMaxHole[0]		= maskInfo.dRealHoleWidth/maskInfo.dResolution*2.0;

		paramShapeHole[1]	= "height";
		paramMinHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*0.8;
		paramMaxHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*2.0;
		break;

	case HOLE_SHAPE_CIRCLE:
		paramShapeHole[0]	= "width";
		paramMinHole[0]		= maskInfo.dRealHoleWidth/maskInfo.dResolution*0.8;
		paramMaxHole[0]		= maskInfo.dRealHoleWidth/maskInfo.dResolution*2.0;

		paramShapeHole[1]	= "height";
		paramMinHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*0.8;
		paramMaxHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*2.0;

		paramShapeHole[2]	= "circularity";
		paramMinHole[2]		= 0.8;
		paramMaxHole[2]		= 1.0;
		break;

	case HOLE_SHAPE_ELLIPSE_45:
		paramShapeHole[0]	= "area";
		paramMinHole[0]		= (maskInfo.dRealHoleWidth/maskInfo.dResolution)*(maskInfo.dRealHoleHeight/maskInfo.dResolution)*0.5;
		paramMaxHole[0]		= (maskInfo.dRealHoleWidth/maskInfo.dResolution)*(maskInfo.dRealHoleHeight/maskInfo.dResolution)*2.0;

		break;

	case HOLE_SHAPE_RECTANGLE_45:
		paramShapeHole[0]	= "width";
		rotWidth = max( maskInfo.dRealHoleWidth*cos(PI*45/180), maskInfo.dRealHoleHeight*sin(PI*45/180) );
		paramMinHole[0]		= rotWidth/maskInfo.dResolution*0.8;
		paramMaxHole[0]		= rotWidth/maskInfo.dResolution*2.0;

		paramShapeHole[1]	= "height";
		rotHeight = max( maskInfo.dRealHoleWidth*sin(PI*45/180), maskInfo.dRealHoleHeight*cos(PI*45/180) );
		paramMinHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*0.8;
		paramMaxHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*2.0;
		break;

	default:
		paramShapeHole[0]	= "width";
		paramMinHole[0]		= maskInfo.dRealHoleWidth/maskInfo.dResolution*0.8;
		paramMaxHole[0]		= maskInfo.dRealHoleWidth/maskInfo.dResolution*2.0;

		paramShapeHole[1]	= "height";
		paramMinHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*0.8;
		paramMaxHole[1]		= maskInfo.dRealHoleHeight/maskInfo.dResolution*2.0;
	}

	HRegion validHoleRegions = holeRegions.SelectShape( paramShapeHole, "and", paramMinHole, paramMaxHole );

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, validHoleRegions, "유효 구멍들");
	}

	if( validHoleRegions.CountObj()==0 )
	{
		return false;
	}


	//
	// 위치에 따른 분류
	//
	HTuple rows = validHoleRegions.Row();

	HTuple rowsSorting = rows.TupleSort();

// 	double d = rowsSorting.TupleSelect(rowsSorting.TupleLength()-1);
	

	HRegion rgInRow;
	double startY;

	switch( iPos )
	{
	case BHIP_POS_LT:
		startY = rowsSorting.TupleSelect(0);
		if( !findRegionOnLine( validHoleRegions, rgInRow, startY, (maskInfo.dRealPitchY / maskInfo.dResolution) / 4, BHIP_LEFT ) )
		{
			return FALSE;
		}
		break;

	case BHIP_POS_LB:
		startY = rowsSorting.TupleSelect(rowsSorting.TupleLength()-1);
		if( !findRegionOnLine( validHoleRegions, rgInRow, startY, (maskInfo.dRealPitchY / maskInfo.dResolution) / 4, BHIP_LEFT ) )
		{
			return FALSE;
		}
		break;

	case BHIP_POS_RT:
		startY = rowsSorting.TupleSelect(0);
		if( !findRegionOnLine( validHoleRegions, rgInRow, startY, (maskInfo.dRealPitchY / maskInfo.dResolution) / 4, BHIP_RIGHT ))
		{
			return FALSE;
		}
		break;

	case BHIP_POS_RB:
		startY = rowsSorting.TupleSelect(rowsSorting.TupleLength()-1);
		if( !findRegionOnLine( validHoleRegions, rgInRow, startY, (maskInfo.dRealPitchY / maskInfo.dResolution) / 4, BHIP_RIGHT ))
		{
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, rgInRow, "최종 구멍들");
	}


	//
	//
	//
	double targetX = width/2.0;
	double targetY = height/2.0;

	pPpaInfo->dPpaXAvg = (rgInRow.Column()-targetX)*maskInfo.dResolution;
	pPpaInfo->dPpaXMin = (rgInRow.Column()-targetX)*maskInfo.dResolution;
	pPpaInfo->dPpaXMax = (rgInRow.Column()-targetX)*maskInfo.dResolution;
	pPpaInfo->dPpaXStd = (rgInRow.Column()-targetX)*maskInfo.dResolution;

	pPpaInfo->dPpaYAvg = (rgInRow.Row()-targetY)*maskInfo.dResolution;
	pPpaInfo->dPpaYMin = (rgInRow.Row()-targetY)*maskInfo.dResolution;
	pPpaInfo->dPpaYMax = (rgInRow.Row()-targetY)*maskInfo.dResolution;
	pPpaInfo->dPpaYStd = (rgInRow.Row()-targetY)*maskInfo.dResolution;

	pPpaInfo->dCdWidthAvg = 0;
	pPpaInfo->dCdWidthMin = 0;
	pPpaInfo->dCdWidthMax = 0;
	pPpaInfo->dCdWidthStd = 0;

	pPpaInfo->dCdHeightAvg = 0;
	pPpaInfo->dCdHeightMin = 0;
	pPpaInfo->dCdHeightMax = 0;
	pPpaInfo->dCdHeightStd = 0;

	pPpaInfo->nPoint = 1;

	savePpaResultImage( orgImage, rgInRow, width/2, height/2, m_strFilePath, m_strFileName );

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
// PPA 측정 
//	- Template image 사용
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::insp_ppa_image_based( char *pFileName, BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, double dScore, int xTarget, int yTarget, double *dResultX, double *dResultY )
{
	//
	// template image 설정
	//
	load_ncc_model( pFileName );


	//
	// 찾기
	//
	// 이미지 type 변경 : c++ -> halcon
	HImage inputImage("byte", width, height, pImage);
	if( m_bDisplayImage )
	{
		DISPLAY( inputImage, "inputImage" );
	}


	HTuple tRows, tCols, tAngles, tScores;

	m_NCCModel.FindNccModel(	inputImage,
								0,		// AngleStart
								0,		//  AngleExtent
								dScore,	// min score
								0,		// NumMatches
								1.0,	// MaxOverlap
								"true",	// SubPixel
								0,		// NumLevels
								&tRows,
								&tCols,
								&tAngles,
								&tScores );


	// *
	if( m_bDisplayImage )
	{
		if( tRows.TupleLength()>0 )
		{
			HTuple tPhis, tLen1, tLen2;
			for( int i=0 ; i<tRows.TupleLength() ; i++ )
			{
				tPhis.Append(0);
				tLen1.Append(m_szNCCModel.cx);
				tLen2.Append(m_szNCCModel.cy);
			}

			HXLDCont xldFound;
			xldFound.GenRectangle2ContourXld(tRows, tCols, tPhis, tLen1, tLen2 );

			DISPLAY( inputImage, xldFound, "후보자들" );
		} else {
			DISPLAY( inputImage, "후보자들 없음" );
		}
	}

	// Matching 결과가 없을 경우
	int nFound = tRows.TupleLength();
	if( tRows.TupleLength() ==0 )
	{
		return false;
	}


	//
	//
	//
	HTuple tIndex;
	if( !findNearIndex( tRows, tCols, HTuple(yTarget), HTuple(xTarget), &tIndex ) )
	{
		return false;
	}

	*dResultX = tCols[tIndex];
	*dResultY = tRows[tIndex];

	if( m_bDisplayImage )
	{
		HXLDCont xldFound;
		xldFound.GenRectangle2ContourXld(*dResultY, *dResultX, 0, m_szNCCModel.cx, m_szNCCModel.cy );

		DISPLAY( inputImage, xldFound, "최종" );
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//
//	 이물검사 알고리즘
//	
//		- 영상이 획득될 때 마다 검사를 수행한다.
//		- 영상 버퍼와 시작 위치(모터 좌표)를 입력 받아 검사를 수행하고, 
//		  검출된 불량의 좌표를 도면좌표로 변환하여 잘못된 놈들은 삭제한다.
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::inspectParticleDefect( BYTE* pImage, int width, int height, double sxCoord, double syCoord, int iDirection, DEFECT_INFO *pDefects, int *nDefect )
{
	int iOverlapLeft = 2500;
	int iOverlapRight = 500;

	int n=0;
	CString strFileName, strMsg;
	*nDefect = 0;
	int iCnt = 0;

	strMsg.Format("이물검사 > 시작좌표=(%.3f, %.3f), size(%d,%d), 방향=(%d), width=%d, height =%d\n", sxCoord, syCoord, width, height, iDirection,width, height);
	OutputDebugString(strMsg);


// 	BYTE *pTmpImg = new BYTE[width*height];
// 	memcpy( pTmpImg, pImage, width*height);

	// 원본 이미지 저장
	OutputDebugString("이물검사 > HImage 생성 > 시작");

	HImage orgImage("byte", width, height, pImage);

	if(m_bSaveOriginalImage)
	{
		strFileName.Format(_T("d:\\processing\\[Y=%10.5f][X=%10.5f][%d]1 original image.bmp"), syCoord, sxCoord, iDirection );
		orgImage.WriteImage("bmp", 0, strFileName);
	}


// 	delete[] pTmpImg;

// 	return true;

	if( !m_bProcessing )
	{
		return true;
	}


	// 임시 버퍼 생성
	BYTE *pTmpImage1 = new BYTE[ width*height ];
	memset( pTmpImage1, 0, width*height );
	BYTE *pTmpImage2 = new BYTE[ width*height ];
	memset( pTmpImage2, 0, width*height );

	if( pTmpImage1==NULL || pTmpImage2==NULL )
	{
		AfxMessageBox("No memory...");
	}


	// 1. Pitch compare
	start_global_counter();

	pitch_compare(	pImage, pTmpImage1, 
					width, height, 
					0, width-1, 0, height-1,
					m_stParamFMMParticle.uiPitch);

	end_global_counter("Pitch compare");
	if(m_bSaveProcessingImage)
	{
		strFileName.Format(_T("d:\\processing\\[Y=%10.5f][X=%10.5f][%d]2 Pitch.bmp"), syCoord, sxCoord, iDirection );
		HImage saveImage("byte", width, height, pTmpImage1);
		saveImage.WriteImage("bmp", 0, strFileName);
	}



	// 2. threshold
	start_global_counter();

	threshold(	pTmpImage1, pTmpImage2, width, height, 
		0, width-1, 0, height-1, 
		127 - m_stParamFMMParticle.uiDarkThreshold, 127 + m_stParamFMMParticle.uiBrightThreshold,
		0, 255);

	end_global_counter("Threshold");

	if(m_bSaveProcessingImage)
	{
		strFileName.Format(_T("d:\\processing\\[Y=%10.5f][X=%10.5f][%d]3 threshold.bmp"), syCoord, sxCoord, iDirection);
		HImage saveImage("byte", width, height, pTmpImage2);
		saveImage.WriteImage("bmp", 0, strFileName);
	}


	// 2.1 Remove false
	start_global_counter();
	removeFalse(	pTmpImage2, pImage, width, height, 
					m_stParamFMMParticle.uiPitch, 
					255, 
					30 );
	end_global_counter("Remove false defect");

	if(m_bSaveProcessingImage)
	{
		strFileName.Format(_T("d:\\processing\\[Y=%10.5f][X=%10.5f][%d]4 Remove ghost.bmp"), syCoord, sxCoord, iDirection);
		HImage saveImage("byte", width, height, pTmpImage2);
		saveImage.WriteImage("bmp", 0, strFileName);
	}


	// overlap
	for( int y=0 ; y<height ; y++ )
	{
		int iPosY = y*width;
		for( int x=0 ; x<iOverlapLeft ; x++ )
		{
			pTmpImage2[ x + iPosY ] = 0;
		}
		for( int x=width-iOverlapRight ; x<width ; x++ )
		{
			pTmpImage2[ x + iPosY ] = 0;
		}
	}


	// convert image
	HImage threImage("byte", width, height, pTmpImage2);

	HRegion regionThreshold = threImage.Threshold( 127, 255 );


	start_global_counter();
	//////////////////////////////////////////////////////////////////////////
	// filtering : 2 pixel 폭 불량 제거
	HRegion regionOpen = regionThreshold.OpeningCircle(1.5);

	if(m_bSaveProcessingImage)
	{
		strFileName.Format(_T("d:\\processing\\[Y=%10.5f][X=%10.5f][%d]5 filtering edge.hobj"), syCoord, sxCoord, iDirection);
 		regionOpen.WriteRegion(strFileName);
	}
	//
	//////////////////////////////////////////////////////////////////////////
	end_global_counter("미세 형상 변화 filtering : ");


	start_global_counter();
	//////////////////////////////////////////////////////////////////////////
	// Merge : 인접불량
	HRegion regionClose = regionOpen.ClosingCircle(m_stParamFMMParticle.uiPitch/2);

	if(m_bSaveProcessingImage)
	{
		strFileName.Format(_T("d:\\processing\\[Y=%10.5f][X=%10.5f][%d]6 merge.hobj"), syCoord, sxCoord, iDirection);
		regionClose.WriteRegion(strFileName);
	}
	//
	//////////////////////////////////////////////////////////////////////////
	end_global_counter("Merge : ");


	start_global_counter();
	//////////////////////////////////////////////////////////////////////////
	// 3. Labeling

	HRegion regionDefects = regionClose.Connection();

	//
	//////////////////////////////////////////////////////////////////////////
	end_global_counter("Connection");


	// 4. Remove false


	// 5. Change coordinate


	// 6. remove defect at false region
	// 6.1 image coord to motor coord
	// 
	// 6.2 motor coord to absolute coord


	start_global_counter();

	UINT nValid = 0;
	bool bSaveError = false;
	for( int i=0 ; i<regionDefects.CountObj() ;i++ )
	{
		HRegion regionDefect = regionDefects.SelectObj(i+1);
		double dX = regionDefect.Column();
		double dY = regionDefect.Row(); 
		regionDefect.AreaCenter(&dY, &dX);
		HTuple x1, x2, y1, y2;
		regionDefect.SmallestRectangle1( &y1, &x1, &y2, &x2 );


		// 좌표변환 : image -> stage
		double xStageCoord;	// 우하 방향이 +인 좌표계
		double yStageCoord;	// 우하 방향이 +인 좌표계	
		changeCoordinate(	sxCoord, syCoord, 
							iDirection, 
							(int)dX, (int)dY, 
							&xStageCoord, &yStageCoord );


		// 좌표변환 : stage -> 도면
		double xAbsoluteCoord, yAbsoluteCoord;

		Calculate_coordinate( xStageCoord, yStageCoord, &xAbsoluteCoord, &yAbsoluteCoord );


		// 불량 정보 생성
		DEFECT_INFO defectInfo;
		defectInfo.x		= (int)dX;
		defectInfo.y		= (int)dY;
		defectInfo.dRealX	= xAbsoluteCoord;
		defectInfo.dRealY	= yAbsoluteCoord;
		defectInfo.iArea	= regionDefect.Area();
		defectInfo.iWidth	= x2 - x1 + 1;
		defectInfo.iHeight	= y2 - y1 + 1;
		defectInfo.dRealArea	= defectInfo.iArea*m_stParamFMMParticle.x_resolution;
		defectInfo.dRealWidth	= defectInfo.iWidth*m_stParamFMMParticle.x_resolution;
		defectInfo.dRealHeight	= defectInfo.iHeight*m_stParamFMMParticle.y_resolution;
		defectInfo.iBrightness	= 0;
		defectInfo.iStrength	= 0;	
		defectInfo.iMaxStrength	= 0;


		// Ghost 불량의 경우 제왼한다.


		// * 결과 영상 저장 : 임시로 모든 영상을 저장한다.
		// 불량의 크기가 작으면 처리하지 않는다.
		if( defectInfo.iArea <m_stParamFMMParticle.dMinSize/m_stParamFMMParticle.x_resolution)
		{
			if( m_bSaveProcessingImage )
			{
				strFileName.Format("[Y=%10.5f][X=%10.5f][Dir=%d][x=%.4f][y=%.4f][size=%d pixel][%d th]MinSize.bmp", syCoord, sxCoord, iDirection, defectInfo.dRealX, defectInfo.dRealY, defectInfo.iArea, iCnt++);
			
				saveImagePart( pImage, width, height, dX, dY, 256, 256, m_strFilePath+strFileName, 0 );
			}
			continue;
		}


		// 불량이 무효 영역에 존재하면 처리하지 않는다.
		if( !m_activeRegions.checkIn( xAbsoluteCoord, yAbsoluteCoord ) )
		{
			if( m_bSaveProcessingImage )
			{
				strFileName.Format("[Y=%10.5f][X=%10.5f][Dir=%d][x=%.4f][y=%.4f][size=%d pixel][%d th]NonActive.bmp", syCoord, sxCoord, iDirection, defectInfo.dRealX, defectInfo.dRealY, defectInfo.iArea, iCnt++);

				saveImagePart( pImage, width, height, dX, dY, 256, 256, m_strFilePath+strFileName, 0 );
			}
			continue;
		}


		// 결과 데이터에 추가
		if( n<100 )
		{
			// 불량 영상 생성&저장
			if( m_bSaveProcessingImage )
			{
				strFileName.Format("[Y=%10.5f][X=%10.5f][Dir=%d][x=%.4f][y=%.4f][size=%d pixel][%d th].bmp", syCoord, sxCoord, iDirection, defectInfo.dRealX, defectInfo.dRealY, defectInfo.iArea, iCnt++);
				saveImagePart( pImage, width, height, dX, dY, 256, 256, m_strFilePath+strFileName, 0 );
			}

			strFileName.Format("[x=%.4f][y=%.4f][size=%d].bmp", defectInfo.dRealX, defectInfo.dRealY, defectInfo.iArea);
			saveImagePart( pImage, width, height, dX, dY, 256, 256, m_strFilePath+strFileName, iDirection );

			// 불량 영상 위치 기록
			CString strFile = m_strFilePath+strFileName;
			memcpy(defectInfo.cFilePath, strFile.GetBuffer(), strFile.GetLength());
			OutputDebugString("save defect file at" + strFile );

			// 불량 정보 추가
			pDefects[n++] = defectInfo;
			*nDefect = *nDefect + 1;
		}

		nValid++;
	}
	end_global_counter("Make defect info");

	strMsg.Format("이물검사 > Filter active region  : %d ea => %d ea\n", regionDefects.CountObj() , nValid);
	OutputDebugString(strMsg);



	//////////////////////////////////////////////////////////////////////////
	// 기타 참고정보 남기기
	//////////////////////////////////////////////////////////////////////////
	// Align mark 위치 남기기
	if( m_bSaveAlignImage )
	{
		double x1, y1, x2, y2;	// 우하 방향이 +인 좌표계
		changeCoordinate(	sxCoord, syCoord, iDirection, 0, 0, &x1, &y1 );
		changeCoordinate(	sxCoord, syCoord, iDirection, width, height, &x2, &y2 );

		double sx = min( x1, x2 );
		double ex = max( x1, x2 );
		double sy = min( y1, y2 );
		double ey = max( y1, y2 );

		// Align mark
		for( int i=0 ; i<4 ; i++ )
		{
			if( m_dAlignMarkMotorX[i]>=sx && m_dAlignMarkMotorX[i]<ex && 
				m_dAlignMarkMotorY[i]>=sy && m_dAlignMarkMotorY[i]<ey )
			{
				int dxImage = 12000 - (m_dAlignMarkMotorY[i] - sy)/m_stParamFMMParticle.x_resolution;	// 일단 임시
				int dyImage;
				if( iDirection==-1 )
				{
					// 					dyImage = (m_dAlignMarkMotorX[i] - sx)/m_stParamFMMParticle.x_resolution;
					dyImage = (m_dAlignMarkMotorX[i] - sxCoord)/m_stParamFMMParticle.x_resolution;
				} else {
					dyImage = (sxCoord - (m_dAlignMarkMotorX[i] - m_dScanOffsetForward) )/m_stParamFMMParticle.x_resolution;
				}

				CString strFileName;
				strFileName.Format("Align mark [%dth][x=%10.5f][y=%10.5f][dir=%d].bmp", i, sxCoord, syCoord, iDirection );

				saveImagePart( pImage, width, height, dxImage, dyImage, m_szSaveAlignImage.cx, m_szSaveAlignImage.cy, m_strFilePath+strFileName, 0 );

				strMsg.Format("이물검사 > Save align image  : (%.3f, %.3f)\n", m_dAlignMarkMotorX[i], m_dAlignMarkMotorY[i]);
				OutputDebugString(strMsg);

				strFileName.Format(_T("[이물검사_align][Y=%10.5f][X=%10.5f][%d]1 Original image.bmp"), syCoord, sxCoord, iDirection );
				orgImage.WriteImage("bmp", 0, m_strFilePath+strFileName);

			}
		}


		// Active region
	}

	
	delete[] pTmpImage1;
	delete[] pTmpImage2;

	strMsg.Format("이물검사 > 시작좌표=(%.3f, %.3f), size(%d,%d), 방향=(%d), width=%d, height =%d > 종료\n", sxCoord, syCoord, width, height, iDirection,width, height);
	OutputDebugString(strMsg);

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// 좌표변환 행렬 구성
//
//
//	- Sove Ax = b
//	- 모터 좌표를 도면 좌표롤 변환하기 위함
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::SetParam_coordinate( double *xSrc, double *ySrc, int nSrc, double *xDst, double *yDst, int nDst )
{
	if( nSrc<4 || nDst<4 || nSrc!=nDst )
	{
		return false;
	}


// 	mat A( 2*nSrc, 6 );
// 	mat B( 2*nDst, 1 );
// 
// 
// 	// Set matrix A
// 	A.zeros();
// 
// 	for( int i=0 ; i<nSrc ; i++ )
// 	{
// 		A( 2*i, 0 ) = xSrc[i];
// 		A( 2*i, 1 ) = ySrc[i];
// 		A( 2*i, 2 ) = 1;
// 
// 		A( 2*i+1, 3 ) = xSrc[i];
// 		A( 2*i+1, 4 ) = ySrc[i];
// 		A( 2*i+1, 5 ) = 1;
// 	}
// 
// 
// 	// Set matrix B
// 	B.zeros();
// 
// 	for( int i=0 ; i<nDst ; i++ )
// 	{
// 		B( 2*i,	0 )	= xDst[i];
// 		B( 2*i+1,0 ) = yDst[i];
// 	}
// 
// 
// 	//
// 	mat x = solve(A, B);
// 
// 	m_tansMatrix[0] = x(0,0);
// 	m_tansMatrix[1] = x(1,0);
// 	m_tansMatrix[2] = x(2,0);
// 	m_tansMatrix[3] = x(3,0);
// 	m_tansMatrix[4] = x(4,0);
// 	m_tansMatrix[5] = x(5,0);
// 
// 
// 	//
// 	for( int i=0 ; i<4 ; i++ )
// 	{
// 		m_dAlignMarkRealX[i] = xDst[i];
// 		m_dAlignMarkRealY[i] = yDst[i];
// 		m_dAlignMarkMotorX[i] = xSrc[i];
// 		m_dAlignMarkMotorY[i] = ySrc[i];
// 	}


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// 좌표 변환
//
//	- Line scan image 좌표를 stage 이미지 좌표로 변환한다.
//
//	- direction : 
//		1 : Forward
//		-1 : Backward
//
//	- image의 길이는 1000 pixel로 고정
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::changeCoordinate( double xStageLT, double yStageLT, int direction, int xImage, int yImage, double *pXStage, double *pYStage )
{
	if( direction>0 )
	{	// forward
		*pXStage = xStageLT - yImage*m_stParamFMMParticle.y_resolution + m_dScanOffsetForward;
		*pYStage = yStageLT - xImage*m_stParamFMMParticle.y_resolution;
	} 
	else 
	{	// backward
		*pXStage = xStageLT + yImage*m_stParamFMMParticle.y_resolution;
		*pYStage = yStageLT - xImage*m_stParamFMMParticle.y_resolution;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	조각 영상 저장하기
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::removeFalse( BYTE *pThresholdImage, BYTE *pOrgImage, UINT width, UINT height, int iPitch, BYTE defectGray, int iDifferenceLimit )
{
	unsigned int x, y, iPosY;

	for( y=0 ; y<height ; y++ )
	{
		iPosY = y*width;

		for( x=0 ; x<width ; x++ )
		{
			if( pThresholdImage[x+iPosY]==defectGray )
			{
				bool bReal = false;

				if( y>(UINT)iPitch )
				{
					int tempGray = pOrgImage[ x + (y-iPitch)*width ];

					if( abs(tempGray-pOrgImage[x+iPosY])>=iDifferenceLimit )
					{
						bReal = true;
					}
				}

				if( y<height-iPitch )
				{
					int tempGray = pOrgImage[ x + (y+iPitch)*width ];

					if( abs(tempGray-pOrgImage[x+iPosY])>=iDifferenceLimit )
					{
						bReal = true;
					}
				}

				if( !bReal )
				{
					pThresholdImage[x+iPosY] = 0;
				}
			}
		}
	}

	return true;
}


//
//
// 검사 유효 영역 설정
//
//
bool CImageProcess_H::AddActiveRegion( DRect rect )
{
	m_activeRegions.addRegion(rect);


	CString strMsg;
	strMsg.Format("AddActiveRegion sx=%f, ex=%f, sy=%f, ey=%f", rect.sx, rect.ex, rect.sy, rect.ey );
	OutputDebugString(strMsg);


	return true;
}



bool CImageProcess_H::setParam_surfaceInspection( PARAM_FMM_SURFACE_INSPECTIOM *pParam )
{

	if( pParam )
	{
		m_stParamFMMParticle = *pParam;
	} else {
		return false;
	}

	CString strMsg;
	strMsg.Format("setParam_surfaceInspection : pitch=%d [pixel], resolution(%f,%f) [mm], threshold (%d,%d) \n", 
		pParam->uiPitch, pParam->x_resolution, pParam->y_resolution, pParam->uiDarkThreshold, pParam->uiBrightThreshold);
	OutputDebugString(strMsg);

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::pitch_compare( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, UINT uiPitch )
{
	unsigned int x, y, iPosY;
	int diff;
	int sxValid = max(sx, uiPitch);
	int exValid = min( ex, width-uiPitch-1);

	for( y=sy ; y<=ey ; y++ )
	{
		iPosY = y*width;

		for( x=sx ; x<(UINT)sxValid ; x++ )
		{
			pRstImage[ x + iPosY ] = 127;
		}
		for( x=max(sx, uiPitch) ; x<=min( ex, width-uiPitch-1) ; x++ )
		{
			diff = 127 + ( 2*(int)pSrcImage[ x + iPosY ] - (int)pSrcImage[ x-uiPitch + iPosY ] - (int)pSrcImage[ x+uiPitch + iPosY ] )/2;
			pRstImage[ x + iPosY ] = min( 255, max( diff, 0 ));
		}
		for( x=exValid+1 ; x<=ex ; x++ )
		{
			pRstImage[ x + iPosY ] = 127;
		}
	}


// 	int syValid = max(sy, uiPitch);
// 	int eyValid = min( ey, height-uiPitch-1);
// 	for( int y=0 ; y<syValid ; y++ )
// 	{
// 		int iPosY = y*width;
// 		for( int x=0 ; x<width ; x++ )
// 		{
// 			pRstImage[ x + iPosY ] = 127;
// 		}
// 	}
// 
// 	for( int y=eyValid+1 ; y<height ; y++ )
// 	{
// 		int iPosY = y*width;
// 		for( int x=0 ; x<width ; x++ )
// 		{
// 			pRstImage[ x + iPosY ] = 127;
// 		}
// 	}
// 
// 	for( y=syValid ; y<=eyValid ; y++ )
// 	{
// 		int iPosYUp = (y-uiPitch)*width;
// 		int iPosY = y*width;
// 		int iPosYDown = (y+uiPitch)*width;
// 
// 		for( x=0 ; x<width ; x++ )
// 		{
// 			diff = 127 + ( 2*(int)pSrcImage[ x + iPosY ] - (int)pSrcImage[ x + iPosYUp ] - (int)pSrcImage[ x + iPosYDown ] )/2;
// 			pRstImage[ x + iPosY ] = min( 255, max( diff, 0 ));
// 		}
// 	}



	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// 좌표변환 
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::Calculate_coordinate( double xIn, double yIn, double *xOut, double *yOut )
{
// 	mat inMat;
// 
// 	//
// 	inMat.zeros( 3, 1 );	// 초기화
// 
// 	//
// 	inMat( 0, 0 ) = xIn;
// 	inMat( 1, 0 ) = yIn;
// 	inMat( 2, 0 ) = 1.0;
// 
// 
// 	*xOut = m_tansMatrix[0]*xIn +  m_tansMatrix[1]*yIn +  m_tansMatrix[2]*1;
// 	*yOut = m_tansMatrix[3]*xIn +  m_tansMatrix[4]*yIn +  m_tansMatrix[5]*1;


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	조각 영상 저장하기
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::saveImagePart( BYTE *pSrcImage, int imageWidth, int imageHeight, int targetX, int targetY, int targetWidth, int targetHeight, CString strFileName, int rot )
{
	if( pSrcImage==NULL || imageWidth<1 || imageHeight<1 || targetWidth<1 || targetHeight<1 )
	{
		return false;
	}


	//
	BYTE *pImageBuffer = new BYTE[targetWidth*targetHeight];
	memset( pImageBuffer, 0, targetWidth*targetHeight);


	for( int y= 0, yy=targetY-targetHeight/2 ; y<targetHeight ; y++, yy++ )
	{
		for( int x=0, xx=targetX-targetWidth/2 ; x<targetWidth ; x++, xx++ )
		{
			if( xx>=0 && xx<imageWidth && yy>=0 && yy<imageHeight )
			{
				pImageBuffer[ x + y*targetWidth ] = pSrcImage[ xx + yy*imageWidth];
			}
		}
	}


	//
	HImage hImage;
	hImage.GenImageConst( "byte", targetWidth, targetHeight );

	HTuple hv_Pointer, hv_Type, hv_Width1, hv_Height1;
	GetImagePointer1(hImage, &hv_Pointer, &hv_Type, &hv_Width1, &hv_Height1);

	BYTE *pData = (LPBYTE)hv_Pointer.L();


	// 1. 빈영상 생성
// 	CBhImage img;
// 	img.Create(targetWidth, targetHeight, 8, 0 );
// 	memset(img.GetImage(), 0, targetWidth*targetHeight);


	//
	if( rot==0 )
	{
		for( int y= 0, yy=targetY-targetHeight/2 ; y<targetHeight ; y++, yy++ )
		{
			for( int x=0, xx=targetX-targetWidth/2 ; x<targetWidth ; x++, xx++ )
			{
				if( xx>=0 && xx<imageWidth && yy>=0 && yy<imageHeight )
				{
					pData[ x + y*targetWidth ] = pSrcImage[ xx + yy*imageWidth];
				}
			}
		}
	} else if( rot==-1 )
	{
		// 역방향 회전( 좌 -> 우 )
		for( int y=0 ; y<targetHeight ; y++ )
		{
			for( int x=0 ; x<targetWidth ; x++ )
			{
				int xx = y;
				int yy = x;

				pData[ x + y*targetWidth ] = pImageBuffer[ xx + yy*targetWidth ];
			}
		}
	} else if ( rot==1)
	{
		// 역방향 회전( 우 -> 좌 )
		for( int y=0 ; y<targetHeight ; y++ )
		{
			for( int x=0 ; x<targetWidth ; x++ )
			{
				int xx = y;
				int yy = targetHeight - 1 - x;

				pData[ x + y*targetWidth ] = pImageBuffer[ xx + yy*targetWidth ];
			}
		}
	}

	//
	hImage.WriteImage("bmp", 0, strFileName );
// 	img.Save(strFileName);


	delete[] pImageBuffer;

	return true;
}


