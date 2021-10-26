// ImageProcess_H.cpp : �ش� DLL�� �ʱ�ȭ ��ƾ�� �����մϴ�.
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
	// �̹��˻� ����
	//////////////////////////////////////////////////////////////////////////

	m_dScanOffsetForward = 0.445;//1.5924;//0.9445;

	// align ���� ����
	m_bSaveAlignImage = true;
	m_szSaveAlignImage = CSize( 1024, 1024 );

	//
	m_bProcessing = true;

	// align mark ��ġ��
	for( int i=0 ; i<4 ; i++ )
	{
		m_dAlignMarkRealX[i]	= 0.0;
		m_dAlignMarkRealY[i]	= 0.0;
		m_dAlignMarkMotorX[i]	= 0.0;
		m_dAlignMarkMotorY[i]	= 0.0;
	}

	// PPA ��� ����
	m_strFilePath.Format("d:\\processing\\");	// ����
	m_strFileName.Format("PPA_result");			// �̸�

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
	// Region���� �߽���ǥ ȹ��
	HTuple rowsRegions, colsRegions;
	regions.AreaCenter( &rowsRegions, &colsRegions );	


	double rowHole=0, colHole=0;
	double minDistance = DBL_MAX;

	bool bValid = false;

	for( int i=0 ; i<regions.CountObj() ; i++ )
	{
		// Target ��ġ�� �Ÿ� ���
		HTuple distance;
		DistancePp(rowsRegions[i], colsRegions[i], rowTarget, colTarget, &distance );


		// �ּ� �Ÿ����� Ȯ��
		if( distance<minDistance )
		{
			// �ּҰŸ� ����
			minDistance = distance;	

			// index ����
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
		// Target ��ġ�� �Ÿ� ���
		HTuple distance;
		DistancePp(tRows[i], tCols[i], tRowTarget, tColTarget, &distance );


		// �ּ� �Ÿ����� Ȯ��
		if( distance<minDistance )
		{
			// �ּҰŸ� ����
			minDistance = distance;	

			// index ����
			*index = i;
			bValid = true;
		}
	}


	return bValid;
}

//////////////////////////////////////////////////////////////////////////
//
// PPA ���� ��� ������ �����Ѵ�.
//	- ������ �ߵǰ� �ִ��� Ȯ���ϱ� ���� �뵵
//
//////////////////////////////////////////////////////////////////////////
BOOL savePpaResultImage( HImage orgImage, HRegion rgResult, double xOld, double yOld, CString strFilePath, CString strFileName )
{
	Hlong imgWidth, imgHeight;
	orgImage.GetImageSize( &imgWidth, &imgHeight );

	// Color ������ �����Ѵ�.
	HImage imageCh1 = orgImage.CopyImage();
	HImage imageCh2 = orgImage.CopyImage();
	HImage imageCh3 = orgImage.CopyImage();

	HImage imgResult = imageCh1.Compose3(imageCh2, imageCh3);


	// ���� �߽��� ǥ���ϱ� ���� ���� region ����
	HRegion rgCenter, rgCenter1, rgCenter2;
	double cx = (double)imgWidth/2;
	double cy = (double)imgHeight/2;
	rgCenter1.GenRectangle1(cy, 0, cy, (double)imgWidth-1 );
	rgCenter2.GenRectangle1(0, cx, (double)imgWidth, cx );
	rgCenter = rgCenter1.Union2(rgCenter2);


	// ���� PPA���� �߽��� ǥ���ϱ� ���� ���� region ����
	HRegion rgOld, rgOld1, rgOld2;
	cx = xOld;
	cy = yOld;
	rgOld1.GenRectangle1(cy, 0, cy, imgWidth-1 );
	rgOld2.GenRectangle1(0, cx, imgWidth, cx );
	rgOld = rgOld1.Union2(rgOld2);


	// ���� ���� region ����
	HRegion rgHole = rgResult.Difference(rgResult.ErosionCircle(1.5));


	// ���� ���� 3���� ������ ǥ���Ѵ�.
	HImage imgMergeHole = imgResult.PaintRegion( rgResult, ((HTuple(0).Append(0)).Append(255)), HString("fill") );

	HImage imgMergeCenter = imgMergeHole.PaintRegion( rgCenter, ((HTuple(0).Append(255)).Append(0)), HString("fill") );

	HImage imgMergeOld = imgMergeCenter.PaintRegion( rgOld, ((HTuple(255).Append(0)).Append(0)), HString("fill") );


	// d:\\processing ������ ����
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



	// � SW���� ������ ������ ����
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
// PPA ���� ��� ������ �����Ѵ�.
//	- ������ �ߵǰ� �ִ��� Ȯ���ϱ� ���� �뵵
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
	// ���� ������ ���۵��� ��� y��ǥ ����
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
	// ����� Y ��ǥ�� �ش��ϴ� ���� ����
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
// 		DISPLAY(rgHoles, "���� ���� ����");
// 	}


	//
	// ���⿡ ���� ���� ��(��)�� ���� ����
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
// 		DISPLAY(rgHoleRoi, "���� ���� ����");
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
// 	// ROI ����
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
// 	// ROI ����
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
// 	// ROI ����
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
// 	// ROI ����
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
// 	// ROI ����
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
// 	// ROI ����
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
// 		// point ����
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
// 	// ROI ����
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
// 		// ���� contour�� ���� point ������ �����Ͽ� ����
// 		//
// 
// 		// �ӽ� ����� ����
// 		list<pointShape> tmpShapeContour;
// 		
// 		// point ���� ����
// 		HTuple rowCont, colCont;
// // 		Hobject objCont = xldConts[i].Id();
// // 		get_contour_xld(objCont, &rowCont, &colCont );
// 		rowCont = xldConts[i].GetContourXld(&colCont);
// 
// 		// point ����
// 		double dData;
// 		tuple_length( rowCont, &dData );
// 
// 		// point ���� �Է�
// 		for( int j=0 ; j<dData ; j++ )
// 		{
// 			pointShape tmpPt;
// 			tmpPt.x = (float)(double)colCont[j];
// 			tmpPt.y = (float)(double)rowCont[j];
// 
// 			tmpShapeContour.push_back(tmpPt);
// 		}
// 
// 		// ��� ����
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
// 	// ROI ����
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
// 		// ���� contour �����
// 		list<pointShape> tmpShapeContour;
// 
// 
// 		// ���� contour�� ���� ������ ����
// 		HTuple rowCont, colCont;
// 		rowCont = xldConts[i].GetContourXld(&colCont);
// 
// 		// ����� �������� ����
// 		double dData;
// 		tuple_length( rowCont, &dData );
// 
// 		// ���� �Է�
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
// Template image�� ����Ѵ�.
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
	// �̹��� type ���� : c++ -> halcon
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

	// Matching ����� ���� ���
	int nFound = tRows.TupleLength();
	if( tRows.TupleLength() ==0 )
	{
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	PPA ����
//
//	1. �߽� ���� ã��
//	2. �ֺ� ���� ã��
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::insp_ppa( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, MASK_INFO maskInfo, int nByn, PPA_INFO *pPpaInfo, int method )
{

	// �̹��� type ���� : c++ -> halcon
	HImage orgImage("byte", width, height, pImage);

	if( m_bSaveOriginalImage )
	{
		CString strFile;
		strFile.Format("d:\\processing\\%d_insp_ppa", GetTickCount());
		orgImage.WriteImage("bmp", 0, strFile);
	}

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, "�Է� ����");
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
		DISPLAY( orgImage, validHoleRegions, "��ȿ ���۵�");
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
		DISPLAY( orgImage, roiHoleRegion, "��� ����");
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
	// PPA ���� ����
	//
	//
	int iRange = max( nByn/2, 0 );
	HTuple xPpa, yPpa, widthCd, heightCd;
	int nData = 0;
	for( int y=-iRange ; y<=iRange ; y++ )
	{
		for( int x=-iRange ; x<=iRange ; x++ )
		{
			// Hole�� ������ġ
			double targetX = width/2.0	+ x*maskInfo.dRealPitchX/maskInfo.dResolution;
			double targetY = height/2.0 + y*maskInfo.dRealPitchY/maskInfo.dResolution;

			//
			bool bValid = false;
			for( int i=0 ; i<validHoleRegions.CountObj() ; i++ )
			{
				// Target ��ġ�� �ִ� hole region�� ã�´�.
				roiHoleRegion = validHoleRegions.SelectObj(i+1);

				if( !roiHoleRegion.TestRegionPoint( targetY + offsetY, targetX + offsetX ) )
				{
					continue;
				}


				// PPA �� ���
				double ppaX = (roiHoleRegion.Column() - targetX)*maskInfo.dResolution;
				double ppaY = (roiHoleRegion.Row() - targetY)*maskInfo.dResolution;
				xPpa.Append(ppaX);
				yPpa.Append(ppaY);
				pPpaInfo->dPpaX[nData] = ppaX;
				pPpaInfo->dPpaY[nData] = ppaY;


				//////////////////////////////////////////////////////////////////////////
				// CD�� ���
				if( maskInfo.eHoleMeasureMethod==MEASURE_METHOD_EDGE )
				{
					// Edge �˻� ���� ���� : Hole ������ edge
					HRegion searchRegion = roiHoleRegion.DilationCircle(5.5).Difference( roiHoleRegion.ErosionCircle(5.5) );

					// ROI ���� ����
					HImage roiImage = orgImage.ReduceDomain(searchRegion);

					// Edge ����
					HXLDCont edgeXld = roiImage.EdgesSubPix("lanser1", 1, 50, 80);

					// ȭ�� ǥ��
					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, edgeXld, "Edge ����");
					}

					if( edgeXld.CountObj()<1 )
					{
						return false;
					}

					// Edge ���� : ���� �� edge
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
								DISPLAY(orgImage, temp, "MBR ����");
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
					// MBR ����(ȸ��)
					HTuple rowMBR, colMBR, phiMBR, len1MBR, len2MBR;
					roiHoleRegion.SmallestRectangle2( &rowMBR, &colMBR, &phiMBR, &len1MBR, &len2MBR);

					// measure ����
					HMeasure measureX( rowMBR, colMBR, phiMBR+(HTuple(90).TupleRad()), len2MBR + HTuple(20), HTuple(30), HTuple(width), HTuple(height), HString("nearest_neighbor") ); 

					//
					HXLDCont xldRectX;
					xldRectX.GenRectangle2ContourXld( rowMBR, colMBR, phiMBR+(HTuple(90).TupleRad()), len2MBR + HTuple(20), HTuple(30) );

					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectX, "X ������ ����");
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


					// measure ����
					HMeasure measureY( rowMBR, colMBR, phiMBR, len2MBR + HTuple(20), HTuple(30), HTuple(width), HTuple(height), HString("nearest_neighbor") ); 

					HXLDCont xldRectY;
					xldRectY.GenRectangle2ContourXld( rowMBR, colMBR, phiMBR, len2MBR + HTuple(20), HTuple(30) );

					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectY, "Y ������ ����");
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
// 				// CD�� ���
// 				if( maskInfo.eHoleShape!=HOLE_SHAPE_ELLIPSE_45 )
// 				{	// edge�� �̿��Ͽ� CD�� �����Ѵ�.
// 					HRegion searchRegion = roiHoleRegion.DilationCircle(5.5).Difference( roiHoleRegion.ErosionCircle(5.5) );
// 
// 					HImage roiImage = orgImage.ReduceDomain(searchRegion);
// 
// 					HXLDCont edgeXld = roiImage.EdgesSubPix("lanser1", 1, 50, 80);
// 
// 					if( m_bProcessingImage )
// 					{
// 						DISPLAY(orgImage, edgeXld, "Edge ����");
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
// 				{	// Edge�� �̿��Ͽ� CD�� �����Ѵ�.
// 					HRegion searchRegion = roiHoleRegion.DilationCircle(5.5).Difference( roiHoleRegion.ErosionCircle(5.5) );
// 
// 					HImage roiImage = orgImage.ReduceDomain(searchRegion);
// 
// 					HXLDCont edgeXld = roiImage.EdgesSubPix("lanser2_junctions", 1, 50, 100);
// 
// 					if( m_bProcessingImage )
// 					{
// 						DISPLAY(orgImage, edgeXld, "Edge ����");
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
// 								DISPLAY(orgImage, temp, "MBR ����");
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
// 					// ������ �̿���
// 					// MBR
// 					HTuple rowMBR, colMBR, phiMBR, len1MBR, len2MBR;
// 					roiHoleRegion.SmallestRectangle2( &rowMBR, &colMBR, &phiMBR, &len1MBR, &len2MBR);
// 
// 					// measure ����
// 					HMeasure measureX( rowMBR, colMBR, phiMBR+(HTuple(90).TupleRad()), len2MBR + HTuple(20), HTuple(30), HTuple(width), HTuple(height), HString("nearest_neighbor") ); 
// 
// 					HXLDCont xldRectX;
// 					xldRectX.GenRectangle2ContourXld( rowMBR, colMBR, phiMBR+(HTuple(90).TupleRad()), len2MBR + HTuple(20), HTuple(30) );
// 
// 					if( m_bProcessingImage )
// 					{
// 						DISPLAY(orgImage, xldRectX, "X ������ ����");
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
// 					// measure ����
// 					HMeasure measureY( rowMBR, colMBR, phiMBR, len2MBR + HTuple(20), HTuple(30), HTuple(width), HTuple(height), HString("nearest_neighbor") ); 
// 
// 					HXLDCont xldRectY;
// 					xldRectY.GenRectangle2ContourXld( rowMBR, colMBR, phiMBR, len2MBR + HTuple(20), HTuple(30) );
// 
// 					if( m_bProcessingImage )
// 					{
// 						DISPLAY(orgImage, xldRectY, "Y ������ ����");
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

			// ������ �ʵ� ��� 0������ ����
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
	strMessage.Format("PPA ���� > X > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dPpaXAvg, pPpaInfo->dPpaXMin, pPpaInfo->dPpaXMax, pPpaInfo->dPpaXStd );
	OutputDebugString(strMessage);

	strMessage.Format("PPA ���� > Y > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dPpaYAvg, pPpaInfo->dPpaYMin, pPpaInfo->dPpaYMax, pPpaInfo->dPpaYStd );
	OutputDebugString(strMessage);

	strMessage.Format("PPA ���� > width > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dCdWidthAvg, pPpaInfo->dCdWidthMin, pPpaInfo->dCdWidthMax, pPpaInfo->dCdWidthStd );
	OutputDebugString(strMessage);

	strMessage.Format("PPA ���� > height > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dCdHeightAvg, pPpaInfo->dCdHeightMin, pPpaInfo->dCdHeightMax, pPpaInfo->dCdHeightStd );
	OutputDebugString(strMessage);


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	PPA ����
//
//	1. �߽� ���� ã��
//	2. �ֺ� ���� ã��
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::insp_ppa_tracking( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, MASK_INFO maskInfo, int nByn, PPA_INFO *pPpaInfo, double dOldPpaX, double dOldPpaY, int method )
{
	CString str;
	str.Format("insp_ppa_tracking : old x=%f, old y=%f", dOldPpaX, dOldPpaY );
	OutputDebugString(str);


	// �̹��� type ���� : c++ -> halcon
	HImage orgImage("byte", width, height, pImage);

	if( m_bSaveOriginalImage )
	{
		CString strFile;
		strFile.Format("d:\\processing\\%d_insp_ppa_old x_%f_old y_%f", GetTickCount(), dOldPpaX, dOldPpaY );
		orgImage.WriteImage("bmp", 0, strFile);
	}

	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, "�Է� ����");
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
		DISPLAY( orgImage, validHoleRegions, "��ȿ ���۵�");
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
		DISPLAY( orgImage, roiHoleRegion, "��� ����");
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
	// PPA ���� ����
	//
	//
	int iRange = max( nByn/2, 0 );
	HTuple xPpa, yPpa, widthCd, heightCd;
	int nData = 0;
	for( int y=-iRange ; y<=iRange ; y++ )
	{
		for( int x=-iRange ; x<=iRange ; x++ )
		{
			// Hole�� ������ġ
			double targetX = width/2.0 + x*maskInfo.dRealPitchX/maskInfo.dResolution;
			double targetY = height/2.0 + y*maskInfo.dRealPitchY/maskInfo.dResolution;
// 			double targetXTracking = width/2.0 + dOldPpaX/maskInfo.dResolution	+ x*maskInfo.dRealPitchX/maskInfo.dResolution;
// 			double targetYTracking = height/2.0 + dOldPpaY/maskInfo.dResolution + y*maskInfo.dRealPitchY/maskInfo.dResolution;

			//
			bool bValid = false;
			for( int i=0 ; i<validHoleRegions.CountObj() ; i++ )
			{
				// Target ��ġ�� �ִ� hole region�� ã�´�.
				roiHoleRegion = validHoleRegions.SelectObj(i+1);

				if( !roiHoleRegion.TestRegionPoint( targetY + offsetY, targetX + offsetX ) )
// 				if( !roiHoleRegion.TestRegionPoint( targetYTracking + offsetY, targetXTracking + offsetX ) )	// tracking
				{
					continue;
				}


				// PPA �� ���
				double ppaX = (roiHoleRegion.Column() - targetX)*maskInfo.dResolution;
				double ppaY = (roiHoleRegion.Row() - targetY)*maskInfo.dResolution;
				xPpa.Append(ppaX);
				yPpa.Append(ppaY);
				pPpaInfo->dPpaX[nData] = ppaX;
				pPpaInfo->dPpaY[nData] = ppaY;


				//////////////////////////////////////////////////////////////////////////
				// CD�� ���
				if( maskInfo.eHoleMeasureMethod==MEASURE_METHOD_EDGE )
				{
					// Edge �˻� ���� ���� : Hole ������ edge
					HRegion searchRegion = roiHoleRegion.DilationCircle(5.5).Difference( roiHoleRegion.ErosionCircle(5.5) );

					// ROI ���� ����
					HImage roiImage = orgImage.ReduceDomain(searchRegion);

					// Edge ����
					HXLDCont edgeXld = roiImage.EdgesSubPix("lanser1", 1, 50, 80);

					// ȭ�� ǥ��
					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, edgeXld, "Edge ����");
					}

					if( edgeXld.CountObj()<1 )
					{
						return false;
					}

					// Edge ���� : ���� �� edge
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
								DISPLAY(orgImage, temp, "MBR ����");
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
					// MBR ����(ȸ��)
					HTuple rowMBR, colMBR, phiMBR, len1MBR, len2MBR;
					roiHoleRegion.SmallestRectangle2( &rowMBR, &colMBR, &phiMBR, &len1MBR, &len2MBR);

					// measure ����
					HMeasure measureX( rowMBR, colMBR, phiMBR+(HTuple(90).TupleRad()), len2MBR + HTuple(20), HTuple(30), HTuple(width), HTuple(height), HString("nearest_neighbor") ); 

					//
					HXLDCont xldRectX;
					xldRectX.GenRectangle2ContourXld( rowMBR, colMBR, phiMBR+(HTuple(90).TupleRad()), len2MBR + HTuple(20), HTuple(30) );

					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectX, "X ������ ����");
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


					// measure ����
					HMeasure measureY( rowMBR, colMBR, phiMBR, len2MBR + HTuple(20), HTuple(30), HTuple(width), HTuple(height), HString("nearest_neighbor") ); 

					HXLDCont xldRectY;
					xldRectY.GenRectangle2ContourXld( rowMBR, colMBR, phiMBR, len2MBR + HTuple(20), HTuple(30) );

					if( m_bDisplayImage )
					{
						DISPLAY(orgImage, xldRectY, "Y ������ ����");
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

			// ������ �ʵ� ��� 0������ ����
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
	strMessage.Format("PPA ���� > X > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dPpaXAvg, pPpaInfo->dPpaXMin, pPpaInfo->dPpaXMax, pPpaInfo->dPpaXStd );
	OutputDebugString(strMessage);

	strMessage.Format("PPA ���� > Y > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dPpaYAvg, pPpaInfo->dPpaYMin, pPpaInfo->dPpaYMax, pPpaInfo->dPpaYStd );
	OutputDebugString(strMessage);

	strMessage.Format("PPA ���� > width > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dCdWidthAvg, pPpaInfo->dCdWidthMin, pPpaInfo->dCdWidthMax, pPpaInfo->dCdWidthStd );
	OutputDebugString(strMessage);

	strMessage.Format("PPA ���� > height > avg=%.2f, min=%.2f, max=%.2f, deviation=%.2f \n", pPpaInfo->dCdHeightAvg, pPpaInfo->dCdHeightMin, pPpaInfo->dCdHeightMax, pPpaInfo->dCdHeightStd );
	OutputDebugString(strMessage);


	// Ȯ�ο� > PPA ���� ��� ����
// 	savePpaResultImage( orgImage,									// ���� ����
// 						rgRoiHole,									// PPA�� ������ ���� ����
// 						width/2 + dOldPpaX/maskInfo.dResolution,	// ���� PPA ��ġ( X ���� ��ǥ )
// 						height/2 + dOldPpaY/maskInfo.dResolution,	// ���� PPA ��ġ( Y ���� ��ǥ )
// 						m_strFilePath,								// ���� �� ��ġ
// 						m_strFileName );							// ���� �� �̸�


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// Ư�� ��ġ�� ���� ���� ��ġ ����
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::insp_ppa_pos( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, MASK_INFO maskInfo, int iPos, int xTarget, int yTarget, int nByn, PPA_INFO *pPpaInfo, int method )
{
	CString str;
	str.Format("insp_ppa_pos : Start" );
	OutputDebugString(str);


	// �̹��� type ���� : c++ -> halcon
	HImage orgImage("byte", width, height, pImage);


	// �Է� ���� ����
	if( m_bSaveOriginalImage )
	{
		CString strFile;
		strFile.Format("d:\\processing\\%d_insp_ppa_pos", GetTickCount() );
		orgImage.WriteImage("bmp", 0, strFile);
	}

	// �Է� ���� ȭ�� ǥ��
	if( m_bDisplayImage )
	{
		DISPLAY( orgImage, "�Է� ����");
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
		DISPLAY( orgImage, validHoleRegions, "��ȿ ���۵�");
	}

	if( validHoleRegions.CountObj()==0 )
	{
		return false;
	}


	//
	// ��ġ�� ���� �з�
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
		DISPLAY( orgImage, rgInRow, "���� ���۵�");
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
// PPA ���� 
//	- Template image ���
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::insp_ppa_image_based( char *pFileName, BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, double dScore, int xTarget, int yTarget, double *dResultX, double *dResultY )
{
	//
	// template image ����
	//
	load_ncc_model( pFileName );


	//
	// ã��
	//
	// �̹��� type ���� : c++ -> halcon
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

			DISPLAY( inputImage, xldFound, "�ĺ��ڵ�" );
		} else {
			DISPLAY( inputImage, "�ĺ��ڵ� ����" );
		}
	}

	// Matching ����� ���� ���
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

		DISPLAY( inputImage, xldFound, "����" );
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//
//	 �̹��˻� �˰���
//	
//		- ������ ȹ��� �� ���� �˻縦 �����Ѵ�.
//		- ���� ���ۿ� ���� ��ġ(���� ��ǥ)�� �Է� �޾� �˻縦 �����ϰ�, 
//		  ����� �ҷ��� ��ǥ�� ������ǥ�� ��ȯ�Ͽ� �߸��� ����� �����Ѵ�.
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

	strMsg.Format("�̹��˻� > ������ǥ=(%.3f, %.3f), size(%d,%d), ����=(%d), width=%d, height =%d\n", sxCoord, syCoord, width, height, iDirection,width, height);
	OutputDebugString(strMsg);


// 	BYTE *pTmpImg = new BYTE[width*height];
// 	memcpy( pTmpImg, pImage, width*height);

	// ���� �̹��� ����
	OutputDebugString("�̹��˻� > HImage ���� > ����");

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


	// �ӽ� ���� ����
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
	// filtering : 2 pixel �� �ҷ� ����
	HRegion regionOpen = regionThreshold.OpeningCircle(1.5);

	if(m_bSaveProcessingImage)
	{
		strFileName.Format(_T("d:\\processing\\[Y=%10.5f][X=%10.5f][%d]5 filtering edge.hobj"), syCoord, sxCoord, iDirection);
 		regionOpen.WriteRegion(strFileName);
	}
	//
	//////////////////////////////////////////////////////////////////////////
	end_global_counter("�̼� ���� ��ȭ filtering : ");


	start_global_counter();
	//////////////////////////////////////////////////////////////////////////
	// Merge : �����ҷ�
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


		// ��ǥ��ȯ : image -> stage
		double xStageCoord;	// ���� ������ +�� ��ǥ��
		double yStageCoord;	// ���� ������ +�� ��ǥ��	
		changeCoordinate(	sxCoord, syCoord, 
							iDirection, 
							(int)dX, (int)dY, 
							&xStageCoord, &yStageCoord );


		// ��ǥ��ȯ : stage -> ����
		double xAbsoluteCoord, yAbsoluteCoord;

		Calculate_coordinate( xStageCoord, yStageCoord, &xAbsoluteCoord, &yAbsoluteCoord );


		// �ҷ� ���� ����
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


		// Ghost �ҷ��� ��� �����Ѵ�.


		// * ��� ���� ���� : �ӽ÷� ��� ������ �����Ѵ�.
		// �ҷ��� ũ�Ⱑ ������ ó������ �ʴ´�.
		if( defectInfo.iArea <m_stParamFMMParticle.dMinSize/m_stParamFMMParticle.x_resolution)
		{
			if( m_bSaveProcessingImage )
			{
				strFileName.Format("[Y=%10.5f][X=%10.5f][Dir=%d][x=%.4f][y=%.4f][size=%d pixel][%d th]MinSize.bmp", syCoord, sxCoord, iDirection, defectInfo.dRealX, defectInfo.dRealY, defectInfo.iArea, iCnt++);
			
				saveImagePart( pImage, width, height, dX, dY, 256, 256, m_strFilePath+strFileName, 0 );
			}
			continue;
		}


		// �ҷ��� ��ȿ ������ �����ϸ� ó������ �ʴ´�.
		if( !m_activeRegions.checkIn( xAbsoluteCoord, yAbsoluteCoord ) )
		{
			if( m_bSaveProcessingImage )
			{
				strFileName.Format("[Y=%10.5f][X=%10.5f][Dir=%d][x=%.4f][y=%.4f][size=%d pixel][%d th]NonActive.bmp", syCoord, sxCoord, iDirection, defectInfo.dRealX, defectInfo.dRealY, defectInfo.iArea, iCnt++);

				saveImagePart( pImage, width, height, dX, dY, 256, 256, m_strFilePath+strFileName, 0 );
			}
			continue;
		}


		// ��� �����Ϳ� �߰�
		if( n<100 )
		{
			// �ҷ� ���� ����&����
			if( m_bSaveProcessingImage )
			{
				strFileName.Format("[Y=%10.5f][X=%10.5f][Dir=%d][x=%.4f][y=%.4f][size=%d pixel][%d th].bmp", syCoord, sxCoord, iDirection, defectInfo.dRealX, defectInfo.dRealY, defectInfo.iArea, iCnt++);
				saveImagePart( pImage, width, height, dX, dY, 256, 256, m_strFilePath+strFileName, 0 );
			}

			strFileName.Format("[x=%.4f][y=%.4f][size=%d].bmp", defectInfo.dRealX, defectInfo.dRealY, defectInfo.iArea);
			saveImagePart( pImage, width, height, dX, dY, 256, 256, m_strFilePath+strFileName, iDirection );

			// �ҷ� ���� ��ġ ���
			CString strFile = m_strFilePath+strFileName;
			memcpy(defectInfo.cFilePath, strFile.GetBuffer(), strFile.GetLength());
			OutputDebugString("save defect file at" + strFile );

			// �ҷ� ���� �߰�
			pDefects[n++] = defectInfo;
			*nDefect = *nDefect + 1;
		}

		nValid++;
	}
	end_global_counter("Make defect info");

	strMsg.Format("�̹��˻� > Filter active region  : %d ea => %d ea\n", regionDefects.CountObj() , nValid);
	OutputDebugString(strMsg);



	//////////////////////////////////////////////////////////////////////////
	// ��Ÿ �������� �����
	//////////////////////////////////////////////////////////////////////////
	// Align mark ��ġ �����
	if( m_bSaveAlignImage )
	{
		double x1, y1, x2, y2;	// ���� ������ +�� ��ǥ��
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
				int dxImage = 12000 - (m_dAlignMarkMotorY[i] - sy)/m_stParamFMMParticle.x_resolution;	// �ϴ� �ӽ�
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

				strMsg.Format("�̹��˻� > Save align image  : (%.3f, %.3f)\n", m_dAlignMarkMotorX[i], m_dAlignMarkMotorY[i]);
				OutputDebugString(strMsg);

				strFileName.Format(_T("[�̹��˻�_align][Y=%10.5f][X=%10.5f][%d]1 Original image.bmp"), syCoord, sxCoord, iDirection );
				orgImage.WriteImage("bmp", 0, m_strFilePath+strFileName);

			}
		}


		// Active region
	}

	
	delete[] pTmpImage1;
	delete[] pTmpImage2;

	strMsg.Format("�̹��˻� > ������ǥ=(%.3f, %.3f), size(%d,%d), ����=(%d), width=%d, height =%d > ����\n", sxCoord, syCoord, width, height, iDirection,width, height);
	OutputDebugString(strMsg);

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// ��ǥ��ȯ ��� ����
//
//
//	- Sove Ax = b
//	- ���� ��ǥ�� ���� ��ǥ�� ��ȯ�ϱ� ����
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
// ��ǥ ��ȯ
//
//	- Line scan image ��ǥ�� stage �̹��� ��ǥ�� ��ȯ�Ѵ�.
//
//	- direction : 
//		1 : Forward
//		-1 : Backward
//
//	- image�� ���̴� 1000 pixel�� ����
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
//	���� ���� �����ϱ�
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
// �˻� ��ȿ ���� ����
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
// ��ǥ��ȯ 
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::Calculate_coordinate( double xIn, double yIn, double *xOut, double *yOut )
{
// 	mat inMat;
// 
// 	//
// 	inMat.zeros( 3, 1 );	// �ʱ�ȭ
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
//	���� ���� �����ϱ�
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


	// 1. �󿵻� ����
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
		// ������ ȸ��( �� -> �� )
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
		// ������ ȸ��( �� -> �� )
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


