#pragma once

#include "ImageProcess_define.h"

#include <list>
#include <vector>
using namespace std;


class __declspec(dllexport) CImageProcess
{
public:
	CImageProcess(void);
	virtual ~CImageProcess();

	virtual void initialize() {return;};

	virtual void IsProcessing( bool bProcessing ) {return;};

	virtual void saveOrigialImage( bool bSave ) {return;};
	virtual void saveProcessingImage( bool bSave ) {return;};
	virtual void saveAlignImage( bool bSave ) {return;};
	virtual void displayImage( bool bDisplay ) {return;};

	virtual void setScanOffset( double offset ) {return;};
	virtual void setFilePath( char *path, int len ) {return;};
	virtual void setFileName( char *path, int len ) {return;};
	

	list<DEFECT_INFO>* GetDefectList() { return NULL; };


	//////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER m_globalStartCounter, m_globalEndCounter, m_counter_Freq;
	void start_global_counter();
	void end_global_counter(CString strMsg);
	//////////////////////////////////////////////////////////////////////////


	virtual void Test(void)
	{
		int a = 0;
	}

	virtual BYTE* load_image( char *pFileName, int *pWidth, int *pHeight );

	// arithmatic
	virtual bool sqrt_image( BYTE *pImage1, BYTE *pImage2, BYTE *pImageResult, UINT width, UINT height, UINT sx, UINT sy, UINT ex, UINT ey );
	virtual float calc_angle_distance_degree( float fAngle1, float fAngle2 );
	virtual float calc_angle_distance_radian( float fAngle1, float fAngle2 );
	virtual bool calc_taylor_param( float *pIn, int width, int x, int y, float *pDx, float *pDy, float *pDxx, float *pDxy, float *pDyy );
	virtual bool calc_taylor_param( BYTE *pIn, int width, int x, int y, float *pDx, float *pDy, float *pDxx, float *pDxy, float *pDyy );
	virtual float convert_angle_range( float fAngle );


	// threshold
	virtual bool threshold( BYTE *pImage, BYTE *pImageResult, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int lowThreshold, int highThreshold, int objectGray=255, int backgroundGray=0 );
	virtual bool threshold( float *pImage, BYTE *pImageResult, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, float lowThreshold, float highThreshold, int objectGray=255, int backgroundGray=0 );
	virtual bool threshold( BYTE *pImage, BYTE *pImageResult, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, THRESHOLD_METHOD eMethod );


	// smoothing
	virtual bool Gauss_image(BYTE *pImage, BYTE *pImage_out, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int size);
	virtual bool mean_image(BYTE *pImage, BYTE *pImage_out, int width, int height, int sx, int sy, int ex, int ey, int szFilterHalf);
	virtual bool blur(BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, CSize szFilter);
	virtual bool box_filter(BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, CSize szFilter);
	virtual bool median_filter(BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, CSize szFilter);
	virtual bool gaussian_filter(BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, CSize szFilter);


	// Derivative
	virtual bool prewitt( BYTE *pImage, BYTE *pImage_out, int width, int height, int sx, int ex, int sy, int ey );
	virtual bool sobel( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey );
	virtual bool scharr( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey );
	virtual bool laplacian( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey );


	// Edge
	virtual bool edge( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int iMethod, int iParam );
	
	virtual bool directional_edge_detection(BYTE *pImage_in, float *pMagnitude, float *pDirection, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey );
	virtual bool thin_edge( BYTE *pEdgeImage, float *pMag, float *pDir, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey);	
	virtual bool edge_subpixel( BYTE *pOrgImage, BYTE *pEdgeImage, float *pMag, float *pPointX, float *pPointY, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey);
	virtual bool search_neighbor_edge_point( BYTE *pImage, BYTE *pEdgeImage, float *pMag, UINT width, UINT height, int x, int y, float fAngle_edge, int *x_out, int *y_out );


	// Morphology
	virtual bool erode( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int nSzKernel );
	virtual bool dilation( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int nSzKernel );


	// Masking
	virtual bool Mask_derivative(BYTE *pImage, BYTE *pImage_out, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int size);
	virtual bool Mask_image_include_boundary(BYTE *pImage, BYTE *pImage_out, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int size, int *pMask, bool bProcessBoundary, bool bBias, UINT bias);
	virtual bool Mask_image_except_boundary(BYTE *pImage_in, BYTE *pImage_out, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, int size, int *pMask, UINT bias );
	

	//
	virtual bool pitch_compare( BYTE *pSrcImage, BYTE *pRstImage, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, UINT uiPitch );


	//
	// Interpolation
	//
	virtual bool calc_max_point_taylor( float *pScoreMap, int width, int height, int sx, int ex, int sy, int ey, float *xResult, float *yResult);
	virtual bool calc_max_point_facet( float *pScoreMap, int width, int height, int sx, int ex, int sy, int ey, float *xResult, float *yResult);
	virtual bool cale_bilinear_interpolation( float *pMap, int width, int height, float x, float y, float *resultValue );
	virtual bool cale_bilinear_interpolation( BYTE *pMap, int width, int height, float x, float y, float *resultValue );
	virtual bool cale_bicubic_interpolation( float *pMap, int width, int height, float x, float y, float *resultValue );
	virtual bool cale_bicubic_interpolation( BYTE *pMap, int width, int height, float x, float y, float *resultValue );

	//
	virtual bool make_image_float_to_byte(float *pIn, BYTE *pOut, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey );
	virtual bool make_image_float_to_byte(float *pIn, BYTE *pOut, UINT width, UINT height, UINT sx, UINT ex, UINT sy, UINT ey, float fRangeMin, float fRangeMax );
	virtual void paint_boundary_region( BYTE *pImage, UINT width, UINT height, int range, int paint_value );
	virtual void paint_boundary_region( BYTE *pImage, UINT width, UINT height, int sx, int ex, int sy, int ey, int range, int paint_value );
	virtual BYTE* pyramid_image( BYTE *pImageIn, int widthIn, int heightIn, int level, int *widthOut, int* heightOut );



	//////////////////////////////////////////////////////////////////////////
	//
	// Template matching : shape based
	//
	//////////////////////////////////////////////////////////////////////////
// 	vector<list<FPoint>> m_contourVector;					// 임시 모델
// 	vector<list<pointShape>> m_contourVector_shapeModel;	// contour of shape model
	virtual bool edit_shape_model( int szMin , int szMax );
	virtual bool create_shape_model( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey, int szSmooth );
	virtual bool find_shape_model( BYTE *pImage, int width, int height, UINT sx, UINT ex, UINT sy, UINT ey, float *x_pos, float *y_pos );
	virtual bool find_shape_model( BYTE *pImage, int width, int height, UINT sx, UINT ex, UINT sy, UINT ey, float *x_pos, float *y_pos, int pyramidLevel, int method );
	virtual bool save_shape_model(char *pFileName);
	virtual bool load_shape_model(char *pFileName);


	//////////////////////////////////////////////////////////////////////////
	//
	// Template matching : NCC model
	//
	//////////////////////////////////////////////////////////////////////////
// 	NCC_MODEL m_nccModel;
	virtual bool create_ncc_model( BYTE *pImage, int width, int height, int sx, int ex, int sy, int ey, int iColor=0 );
	virtual bool create_ncc_model( BYTE *pImage, int width, int height, int sx, int ex, int sy, int ey, int lowThreshold, int highThreshold );
	virtual bool save_ncc_model(char *pFileName){return false;};
	virtual bool load_ncc_model(char *pFileName){return false;};
	virtual bool find_ncc_model( BYTE *pImage, int width, int height, int sx, int ex, int sy, int ey, float *resultX, float *resultY );
	virtual bool find_ncc_model( BYTE *pImage, int width, int height, int sx, int ex, int sy, int ey, float *resultX, float *resultY, int pyramid );




	//////////////////////////////////////////////////////////////////////////
	//
	// Focus
	//
	//////////////////////////////////////////////////////////////////////////
	virtual bool insp_focus( BYTE* pImage, int width, int height, int sx, int ex, int sy, int ey, double limit ){return false;};
	virtual double getFocusValue( BYTE* pImage, int width, int height, int sx, int ex, int sy, int ey ){return 0.0;};


	//////////////////////////////////////////////////////////////////////////
	//
	// Wave
	//
	//////////////////////////////////////////////////////////////////////////
	virtual bool insp_wave( float *pDataIn, int width, int height, int sx, int sy, int ex, int ey, int bin_x, int bin_y, float rangeMin, float rangeMax, float fThreshold ){return false;};
	virtual bool insp_wave_bais( float *pDataIn, int width, int height, int sx, int sy, int ex, int ey, int bin_x, int bin_y, float fLowThreshold, float fHighThreshold, int iMinSize ){return false;};
	virtual bool find_guides( float *pDataIn, int width, int height, int pitchX, int pitchY, float fThresholdHGuide, float fThresholdVGuide, float fLimitHGuideRatio, float fLimitVGuideRatio ){return false;};


	//////////////////////////////////////////////////////////////////////////
	//
	// Align 관련
	//
	//////////////////////////////////////////////////////////////////////////
	virtual bool set_align_parameter( float fRes, float fXPosMaskLT, float fYPosMaskLT, float fXPosMaskRB, float fYPosMaskRB, float fXPosGlassLT, float fYPosGlassLT, float fXPosGlassRB, float fYPosGlassRB ) {return false;};
	virtual bool calc_align( float *dx, float *dy, float *theta );
	virtual bool find_main_pattern( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey, int iLowThreshold, int iHighThreshold, int index, float *xResult, float *yResult ) {return false;};
	virtual bool find_main_pattern_ncc( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey, int index, float *xResult, float *yResult, int iLevel ) {return false;};
	virtual bool find_sub_pattern( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey, int iLowThreshold, int iHighThreshold, int index, float *xResult, float *yResult ) {return false;};

	virtual bool set_align_parameter( float fRes, float x1_world, float y1_world, float x2_world, float y2_world );
	virtual bool set_frame_pattern( float x_image, float y_image, float x_world, float y_world, int index );
	virtual bool set_glass_pattern( float x_image, float y_image, float x_world, float y_world, int index );

	virtual bool create_main_pattern( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey, int lowThreshold, int highThreshold );
	virtual bool create_sub_pattern( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey );

	virtual bool find_main_pattern( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey, int index, float *xResult, float *yResult );
	virtual bool find_sub_pattern( BYTE *pImage, int width, int height, int sx, int sy, int ex, int ey, int index, float *xResult, float *yResult );

	virtual bool copy_shape_model( vector<list<pointShape>> *scrModel, vector<list<pointShape>> *resultModel );

// 	vector<list<pointShape>> m_contourVector_shapeModel_main_pattern;	// contour of main pattern
// 	vector<list<pointShape>> m_contourVector_shapeModel_sub_pattern;	// contour of sub pattern
// 	float m_fResolution;			// pixel resolution of align camera
// 	FPoint m_ptMainPattern1_world;	// Main pattern #1의 좌표
// 	FPoint m_ptMainPattern2_world;	// Main pattern #2의 좌표
// 	FPoint m_ptMainPattern1_image;	// Main pattern #1의 이미지 좌표
// 	FPoint m_ptMainPattern2_image; 	// Main pattern #2의 이미지 좌표
// 	FPoint m_ptSubPattern1_world;	// Sub pattern #1의 좌표
// 	FPoint m_ptSubPattern2_world;	// Sub pattern #2의 좌표
// 	FPoint m_ptSubPattern1_image; 	// Sub pattern #1의 이미지 좌표
// 	FPoint m_ptSubPattern2_image; 	// Sub pattern #2의 이미지 좌표


	//////////////////////////////////////////////////////////////////////////
	// PPA 관련
	//////////////////////////////////////////////////////////////////////////
	int m_nPpaResult;
	FPoint m_aPpaResult[_IP_PPA_];
	int m_nOffsetResult;

	virtual bool insp_offset( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, int szMask );
	virtual bool insp_offset( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, int lowThrashold_glass, int highThreshold_glass, int minArea_glass, int maxArea_glass, int lowThreshold_mask, int highThreshold_mask, int minArea_mask, int maxArea_mask, int szMask, float *cx_glass, float *cy_glass, float *szGalss, float *cx_mask, float *cy_mask, float *width_mask );
	virtual bool insp_offset_multi( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, int lowThreshold_glass, int highThreshold_glass, int width_glass, int height_glass, float fMargin_glass, int lowThreshold_mask, int highThreshold_mask, int width_mask, int height_mask, float fMargin_mask, double fPitchX, double fPitchY, int nByn, double *fDx, double *fDy  ) {return false;};

	virtual bool insp_ppa( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, int low_threshold, int high_threshold, UINT low_area, UINT high_area, double *cx_result, double *cy_result, int *width_result, int *height_result );
	virtual bool insp_ppa( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, MASK_INFO maskInfo, int nByn, PPA_INFO *pPpaInfo, int method=0 ) {return false;};
	virtual bool insp_ppa_pos( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, MASK_INFO maskInfo, int iPos, int xTarget, int yTarget, int nByn, PPA_INFO *pPpaInfo, int method=0 ) {return false;};
	virtual bool insp_ppa_multi( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, int low_threshold, int high_threshold, int target_width, int target_hight, float fMargin_ratio, double fPitchX, double fPitchY, int nByn, double *fDx, double *fDy, int method=0 ) {return false;};

	virtual FPoint* get_ppa_result( int *nResult ) {return NULL;};
	virtual OFFSET_FMM* get_offset_result( int *nResult ) {return NULL;};

	virtual bool create_mask_pattern_ppa( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, int lowThreshold, int highThreshold );
	virtual bool find_mask_pattern_ppa( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey, float *cx, float *cy );




	//////////////////////////////////////////////////////////////////////////
	//
	// 이물검사 관련
	//
	//////////////////////////////////////////////////////////////////////////
protected:

public:
	// 검사 영역
	virtual bool AddActiveRegion( DRect rect );	// 검사영역 추가

	// 좌표계 설정 파라메터 ( Motor 좌표 -> 도면좌표 )
	virtual bool SetParam_coordinate( double dTx, double dTy, double dTheta, double xScale=1.0, double yScale=1.0 );	// Rotation & translation
	virtual bool SetParam_coordinate( double *xSrc, double *ySrc, int nSrc, double *xDst, double *yDst, int nDst );		// Affine transform
	virtual bool Calculate_coordinate( double xIn, double yIn, double *xOut, double *yOut );							// 설정된 좌표 변환 matrix에 따른 좌표 변환 함수

	// 검사 파라메터
	virtual bool setParam_surfaceInspection( PARAM_FMM_SURFACE_INSPECTIOM *pParam );

	// 검사 함수
	virtual bool inspectParticleAtFMM( BYTE* pImage, int width, int height, int sx, int sy, int ex, int ey );	// 테스트용 검사 함수
	virtual bool inspectParticleDefect( BYTE* pImage, int width, int height, double sxCoord, double syCoord, int iDirection, DEFECT_INFO *pDefects, int *nDefect );
	virtual bool mergeParticleDefect( list<DEFECT_INFO> *pDefectList, list<DEFECT_INFO> *pListMergedDefect );	// 검출 불량 merge 함수
};

