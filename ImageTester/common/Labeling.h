// Labeling.h: interface for the CLabeling class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_LALELING_H__25352E53_EEB8_4EC5_8C36_8358536BFFC8__INCLUDED_)
//#define AFX_LALELING_H__25352E53_EEB8_4EC5_8C36_8358536BFFC8__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlobDef.h"
/*
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
*/

#include <vector>

// class  AFX_EXT_CLASS CLabeling 
class CLabeling 
{
public:
	CLabeling(int nAlgType = STD_ALG_TYPE);
	virtual ~CLabeling();

protected:
	int m_nAlgType;

protected:
	std::vector<PIXEL> m_vecPosition;

	std::vector<PIXEL> m_ptsBlob;		   // blob pixels
	std::vector<PIXEL> m_ptsBoundary;	   // boundary pixels

protected:
	BYTE *	m_pGrabedImage;				   // 해석할 이미지
	BYTE *  m_pResultData;				   // 결과 이미지(Result Buffer)
	CSize	m_szImage;

protected:
	//해상도
	double	m_dResolution;					// Camera Resolution
	double 	m_dResolutionX;					// Camera Resolution X
	double	m_dResolutionY;					// Camera Resolution Y

protected:
	int     m_nLiftingMask;

// Labeling Condition
protected:
	BOOL    m_bManualMode;
	unsigned int m_nMaxNumberOfDefects;    // Max. blob count

    unsigned int m_nArea;                  // Limit blob count
    unsigned int m_nPeak;                  //Limit gray;

	double	m_dCompact;

	double	m_dWhiteLimitSize;
	double	m_dWhiteFR;
	double	m_dBlackLimitSize;
	double	m_dBlackFR;

	//Merge 
	int m_nConvexType;
	int m_nMergeDistance;

protected:
	LABELING * m_pBlobList;
	int        m_nbBlob;				   // Blob counter
	BOOL       m_bBlobExist;			   // blob exist

public:
	LABELING * GetBlobList() const     { return m_pBlobList; }
	int	       GetBlobNum() const      { return m_nbBlob; }
	BOOL       IsBlobExist()           { return m_bBlobExist; }

public:
	CSize      GetImageSize() const    { return m_szImage; }

public:	
	int  CalculateMeanGray(int width, int height, BYTE* pImage);

public:
	void SetCameraResolution(double dResolutionX, double dResolutionY);       //해상도 설정 

	void SetBlobCondition(unsigned int nArea,unsigned int nPeak, double dBlackLimitSize,double dBlackFR,double dWhiteLimitSize,double dWhiteFR,double dCompact = 0.0f) 
		{ m_nArea = nArea; m_nPeak = nPeak; m_dBlackLimitSize = dBlackLimitSize; m_dBlackFR = dBlackFR; m_dWhiteLimitSize = dWhiteLimitSize; m_dWhiteFR = dWhiteFR; m_dCompact = dCompact;}

	void SetMergeCondition(int nConvexType = 0,int nMergeDistance = 10)
		{m_nConvexType = nConvexType; m_nMergeDistance = nMergeDistance;}

public:	
	BOOL PerformLabeling(BYTE* pGrabedImage,BYTE* pResultData, CSize szImage, int nLiftingMask, BOOL bManualMode, int nMaxNumberOfDefects);

	// Image Processing(blob analysis)
protected:
	BOOL AnalysisBlob();	                // blob analysis를 진행한다.

protected:
	// x,y에서부터 시작하여 blob을 찾는 함수
	int 	FindBlob(LABELING * mbr, int k, BYTE *pResultData,BYTE nDefectColor, unsigned int x, unsigned int y, BYTE OBJ, unsigned int nMinNumOfPixsInBlob);
	// blob의 외곽 boundary를 search하는 함수
	unsigned int OutterBoundaryTracing(LABELING *mbr, int k);

	void	NoTwoPassThinning(LABELING * mbr, int k);
	
	void    SaveMBR(LABELING *mbr, int k);
	void	DeleteMBR(BYTE *pResultData, BYTE nDefectColor);
	// blob 정보 계산 함수
	void	ComputeMBRInformation(LABELING *mbr, int tab);
	BOOL    MakeExtendDefectType(LABELING *mbr, int nBlobs);
};

inline void CLabeling::SetCameraResolution(double dResolutionX, double dResolutionY) 
{ 
	m_dResolutionX = dResolutionX; 
	m_dResolutionY = dResolutionY; 
	m_dResolution = (m_dResolutionX + m_dResolutionY)/2;
}
//#endif // !defined(AFX_LALELING_H__25352E53_EEB8_4EC5_8C36_8358536BFFC8__INCLUDED_)
