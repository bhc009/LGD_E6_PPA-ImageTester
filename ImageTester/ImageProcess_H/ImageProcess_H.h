// ImageProcess_H.h : ImageProcess_H DLL의 기본 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

// #include "resource.h"		// 주 기호입니다.
#include "..\common\ImageProcess.h"

// #include "HalconCpp.h"
// using namespace HalconCpp;


#ifdef DLLEXPORT_H
#define DLLTYPE_H __declspec(dllexport)
#else
#define DLLTYPE_H __declspec(dllimport)
#endif

class DLLTYPE_H CImageProcess_H : public CImageProcess
{
public:
	CImageProcess_H(void);
	virtual ~CImageProcess_H(void);

	void Test();

// 	void DISPLAY( const HalconCpp::HImage & image, CString strMsg );
// 	void DISPLAY( const HalconCpp::HRegion & region, CString strMsg );
// 	void DISPLAY( const HalconCpp::HObject & object, CString strMsg);
// 	void DISPLAY( const HalconCpp::HImage & image, HalconCpp::HRegion & region, CString strMsg );
// 	void DISPLAY( const HalconCpp::HImage & image, HalconCpp::HXLDCont & xld, CString strMsg );

// 	bool findNearRegionIndex( HRegion regions, HTuple rowTarget, HTuple colTarget, HTuple *index );

	void initialize();

	void IsProcessing( bool bProcessing );

	void saveOrigialImage( bool bSave );
	void saveProcessingImage( bool bSave );
	void saveAlignImage( bool bSave );
	void displayImage( bool bDisplay );

	void setScanOffset( double offset );
	void setFilePath( char *path, int len );
	void setFileName( char *path, int len );


	bool m_bProcessing;	// 이미지 처리 여부

	bool m_bSaveOriginalImage;
	bool m_bSaveProcessingImage;
	bool m_bSaveAlignImage;	// Align mark 저장여부 : 이물검사

	bool m_bDisplayImage;
	CSize m_szSaveAlignImage;
	double m_dScanOffsetForward;
	CString m_strFilePath;	// file 저장 위치
	CString m_strFileName;	// file 저장 이름



	// threshold
	bool threshold( BYTE *pImage, BYTE *pImageResult, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int lowThreshold, int highThreshold, int objectGray=255, int backgroundGray=0 );
	bool threshold( float *pImage, BYTE *pImageResult, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, float lowThreshold, float highThreshold, int objectGray=255, int backgroundGray=0 );
	bool threshold( BYTE *pImage, BYTE *pImageResult, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, THRESHOLD_METHOD eMethod );


	// smoothing
	bool mean_image(BYTE *pImage, BYTE *pImage_out, int width, int height, int sx, int sy, int ex, int ey, int szFilterHalf);
	bool blur(BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, CSize szFilter);
	bool median_filter(BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, CSize szFilter);
	bool gaussian_filter(BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, CSize szFilter);


	// Derivative
	bool sobel( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey );
	bool laplacian( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey );

	// Edge
	bool edge( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int iMethod, int iParam );


	//////////////////////////////////////////////////////////////////////////
	//
	// Template matching : shape based
	//
	//////////////////////////////////////////////////////////////////////////
	bool create_shape_model( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey, int szSmooth );
	bool find_shape_model( BYTE *pImage, int width, int height, UINT sx, UINT ex, UINT sy, UINT ey, float *x_pos, float *y_pos, int pyramidLevel, int method );
	bool save_shape_model(char *pFileName);
	bool load_shape_model(char *pFileName);

	// NCC based
	bool load_ncc_model(char *pFileName);
	bool find_ncc_model( BYTE *pImage, int width, int height, int sx, int ex, int sy, int ey, float *resultX, float *resultY );


	//////////////////////////////////////////////////////////////////////////
	//
	// PPA 검사
	//
	//////////////////////////////////////////////////////////////////////////
	bool insp_ppa( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, MASK_INFO maskInfo, int nByn, PPA_INFO *pPpaInfo, int method=0 );
	bool insp_ppa_tracking( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, MASK_INFO maskInfo, int nByn, PPA_INFO *pPpaInfo, double dOldPpaX, double dOldPpaY, int method=0 );
	bool insp_ppa_pos( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, MASK_INFO maskInfo, int iPos, int xTarget, int yTarget, int nByn, PPA_INFO *pPpaInfo, int method=0 );
	bool insp_ppa_image_based( char *pFileName, BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, double dScore, int xTarget, int yTarget, double *dResultX, double *dResultY );


	//////////////////////////////////////////////////////////////////////////
	PARAM_FMM_SURFACE_INSPECTIOM m_stParamFMMParticle;	// 검사 parameter
	ACTIVE_REGIONS m_activeRegions;	// 검사 영역

	double m_tansMatrix[6];

	double m_dAlignMarkRealX[4];
	double m_dAlignMarkRealY[4];
	double m_dAlignMarkMotorX[4];
	double m_dAlignMarkMotorY[4];

	bool inspectParticleDefect( BYTE* pImage, int width, int height, double sxCoord, double syCoord, int iDirection, DEFECT_INFO *pDefects, int *nDefect );

	bool setParam_surfaceInspection( PARAM_FMM_SURFACE_INSPECTIOM *pParam );
	bool SetParam_coordinate( double *xSrc, double *ySrc, int nSrc, double *xDst, double *yDst, int nDst );
	bool AddActiveRegion( DRect rect );
	bool removeFalse( BYTE *pThresholdImage, BYTE *pOrgImage, UINT width, UINT height, int iPitch, BYTE defectGray, int iDifferenceLimit );
	bool pitch_compare( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, UINT uiPitch );
	bool Calculate_coordinate( double xIn, double yIn, double *xOut, double *yOut );
	bool changeCoordinate( double xStageLT, double yStageLT, int direction, int xImage, int yImage, double *pXStage, double *pYStage );
	bool saveImagePart( BYTE *pSrcImage, int imageWidth, int imageHeight, int targetX, int targetY, int targetWidth, int targetHeight, CString strFileName, int rot );


private:
		
// 	HNCCModel m_NCCModel;
// 	CSize m_szNCCModel;
};
