
// Labeling.cpp: implementation of the CLabeling class.
//
//////////////////////////////////////////////////////////////////////
//#include "..\stdafx.h"
#include "stdafx.h"
#include "Labeling.h"
#include <math.h>

#define	PI	3.14159265358979323846

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
CLabeling::CLabeling(int nAlgType)
{
	m_nAlgType = nAlgType;

	m_vecPosition.resize(SEEDNUM+9); //Member 변수로 추가(FindBlob의 변수를) 

	m_pGrabedImage = NULL;
	m_pResultData  = NULL;
	m_szImage      = CSize(0, 0);

	//해상도 변수
	m_dResolution  = 40.0;
	m_dResolutionX = 40.0;					// Camera Resolution X
	m_dResolutionY = 40.0;					// Camera Resolution Y

	m_nLiftingMask   = 1;

	//BLOB Condition
	m_bManualMode  = FALSE;

	m_nMaxNumberOfDefects = LIMIT_BLOBS ;   // Max. blob count

    m_nArea        = 1;                     // Limit blob count
    m_nPeak        = 0;                     //Limit gray;

	m_dWhiteLimitSize   = 1;					// Defect Limit Size
	m_dWhiteFR     = 1;
	m_dBlackLimitSize   = 1;					// Defect Limit Size
	m_dBlackFR     = 1;
	m_dCompact     = 0.3;

	//Merge 
	m_nConvexType    = 0;
	m_nMergeDistance = 10;

	m_pBlobList = NULL;
	m_pBlobList = new LABELING[LIMIT_BLOBS];
}

CLabeling::~CLabeling()
{
	if ( m_pBlobList )
	{
		delete[] m_pBlobList;
		m_pBlobList = NULL;
	}
}


/******************************************************************************************
//	함수명	: CalculateMeanGray(int width, int height, BYTE* pImage)
//	입력변수: 
//	기능	: 평균 Gray 구한다
//	최종변경: 2005. 05. 18 Cho Chul-Hyun
//  이    력: 
/******************************************************************************************/ 
int CLabeling::CalculateMeanGray(int width, int height, BYTE* pImage)
{
	int nCount  = width*height;
	int nMeanGray = 0;
	double fSum  = 0;
	double fMean = 0;

	for (int y=0; y<height; y++)
	{
		int nYpos = y*width;
		for (int x=0; x<width; x++)
		{
			fSum += (float)pImage[nYpos+x];
		}
	}

	fMean = (float)(fSum/nCount);

	nMeanGray = (int)(fMean + 0.5);

	return nMeanGray;
}

/******************************************************************************************
//	함수명	: PerformLabeling(BYTE* pGrabedImage,BYTE* pResultData, CSize szImage, int nLiftingMask, BOOL bManualMode, int nMaxNumberOfDefects)
//	입력변수: 
//	기능	: Image Processing을 진행한다
//	최종변경: 2009. 05. 11 Cho Chul-Hyun
//  이    력: 
/******************************************************************************************/  
BOOL CLabeling::PerformLabeling(BYTE* pGrabedImage,BYTE* pResultData, CSize szImage, int nLiftingMask, BOOL bManualMode, int nMaxNumberOfDefects)
{
	m_bBlobExist = FALSE;
	m_nLiftingMask = nLiftingMask;
	m_bManualMode  = bManualMode;
	m_nMaxNumberOfDefects = nMaxNumberOfDefects;

	m_pResultData  = pResultData;
	m_pGrabedImage = pGrabedImage;

	m_szImage      = szImage;

	if(m_nLiftingMask != 2) m_nLiftingMask = 1;

	if(m_nMaxNumberOfDefects >= LIMIT_BLOBS) m_nMaxNumberOfDefects = LIMIT_BLOBS;

	// original image 생성
	if( m_szImage.cx < m_nLiftingMask || m_szImage.cy < m_nLiftingMask ) return m_bBlobExist;

	// Labeling 실행...
	m_bBlobExist = AnalysisBlob();

	return m_bBlobExist;
}


/******************************************************************************************
//	함수명	: AnalysisBlob()
//	입력변수: 
//	기능	: 
//	최종변경: 2008. 03. 17 Cho Chul-Hyun
//  이    력: 
/******************************************************************************************/  
BOOL CLabeling::AnalysisBlob()
{	
	unsigned int x, y;
	int   nFindBlob = -1;
	int   nTabsize  = 1;	// 1 pixel dilation 한다
	BYTE  nDefectColor = BLACKDEFECT;

	unsigned int  nBlobs = 0;

	memset(m_pBlobList, 0, sizeof(LABELING)*LIMIT_BLOBS);

	for ( y = 1; y < m_szImage.cy-1; y++ )
	{
		int iwidth = y*m_szImage.cx;
		for ( x = 1; x < m_szImage.cx-1; x++ )
		{
			int nPosition = iwidth+x;
			if(m_pResultData[nPosition] == BACKGROUND) 
			{
				m_pResultData[nPosition] = DONE;
				continue;
			}

			if ((m_pResultData[nPosition] == MIDDLE) || (m_pResultData[nPosition] == FOREGROUND))
			{

				m_pBlobList[nBlobs].color  = m_pResultData[nPosition];
				m_pBlobList[nBlobs].nIndex = nBlobs;

				nDefectColor = BLACKDEFECT;

				if(m_pResultData[nPosition] == FOREGROUND)
				{
					nDefectColor = WHITEDEFECT;
				}

				// 1. blob connection
				nFindBlob = FindBlob(m_pBlobList,nBlobs,m_pResultData,nDefectColor, x, y, m_pResultData[nPosition], m_nArea);

				if( nFindBlob == 0 ) 
				{
					DeleteMBR(m_pResultData,nDefectColor);
					continue;
				}					
				else if(nFindBlob == -1)
				{
					m_bBlobExist = FALSE;
					return m_bBlobExist;
				}

				// 2. find outter boundary. (boundary mean gray 계산 포함)
				OutterBoundaryTracing(m_pBlobList,nBlobs);

				// 3. No two path thinning. (target mean gray, peak, g/d 계산 포함)
				NoTwoPassThinning(m_pBlobList,nBlobs);

				if(m_pBlobList[nBlobs].nPeak < m_nPeak)
				{
					DeleteMBR(m_pResultData,nDefectColor);
					continue;
				}

				// 4. now, compute mbr information.
				ComputeMBRInformation(&m_pBlobList[nBlobs],nTabsize);

				// 5. caculation of the size
				if (m_pBlobList[nBlobs].dCompact <= m_dCompact)
				{
					m_pBlobList[nBlobs].nType  = THREAD_DEFECT;
					m_pBlobList[nBlobs].dSize = m_pBlobList[nBlobs].dMajor;
				}
				else
				{
					m_pBlobList[nBlobs].nType  = DOT_DEFECT;
					m_pBlobList[nBlobs].dSize = (m_pBlobList[nBlobs].dMajor + m_pBlobList[nBlobs].dMinor) / 2.0;
				}

				m_pBlobList[nBlobs].nAlgType = m_nAlgType;
				m_pBlobList[nBlobs].nExtType = 0;
				
				if(m_pBlobList[nBlobs].color == MIDDLE) //암점
				{
					if ( m_pBlobList[nBlobs].dSize < m_dBlackLimitSize)
					{
						DeleteMBR(m_pResultData,nDefectColor);
						continue;
					}

					if ( fabs(m_pBlobList[nBlobs].dGrayDiff) < m_dBlackFR)
					{
						DeleteMBR(m_pResultData,nDefectColor);
						continue;
					}
				}
				else
				{
					if ( m_pBlobList[nBlobs].dSize < m_dWhiteLimitSize )
					{
						DeleteMBR(m_pResultData,nDefectColor);
						continue;
					}

					if ( fabs(m_pBlobList[nBlobs].dGrayDiff) < m_dWhiteFR)
					{
						DeleteMBR(m_pResultData,nDefectColor);
						continue;
					}
				}

				// 6. Defect merge 
				if(MakeExtendDefectType(m_pBlobList, nBlobs) == TRUE)
				{
					DeleteMBR(m_pResultData,nDefectColor);
					continue;
				}

				SaveMBR(m_pBlobList, nBlobs);

				if((m_bManualMode == FALSE) && (nBlobs >= m_nMaxNumberOfDefects))
				{
					break;
				}

				// increase color & blob index
				nBlobs++;
			}
		}

		if((m_bManualMode == FALSE) && (nBlobs >= m_nMaxNumberOfDefects))
		{
			break;
		}
	}

	m_nbBlob = nBlobs;

	m_bBlobExist = TRUE;

	return m_bBlobExist;
}


/******************************************************************************************
//	함수명	: FindBlob(LABELING * mbr, int k, BYTE *pResultData,BYTE nDefectColor, unsigned int x, unsigned int y, BYTE OBJ, unsigned int nMinNumOfPixsInBlob)
//	입력변수: mbr: LABELING 배열 포인터, k: mbr index,  x,y : 시작 위치,OBJ : blob color type
//	OUTPUT  : blob을 찾으면 true를 찾지 못하면 false를 반환
//	기능	: x,y에서부터 시작하여 blob을 찾는 함수
//	최종변경: 2009. 03. 17 Cho Chul-Hyun
//  이    력: 
//  REMARK  : 8 connection
//		주어진 좌표로부터 시작하여 blob을 찾으며 찾은 결과는 mbr의 left, top, right, bottom, ptsBlob에 저장됨.
//		위의 결과는 이 후 연산에서 필요함.
/******************************************************************************************/ 
int CLabeling::FindBlob(LABELING * mbr, int k, BYTE *pResultData,BYTE nDefectColor, unsigned int x, unsigned int y, BYTE OBJ, unsigned int nMinNumOfPixsInBlob)
{
	int i, j;
	unsigned int left,top,right,bottom;	// mean blob rectangle
	unsigned int Xp, Yp, Xc, Yc;
	unsigned int sxWidth;
	unsigned int check, followNo;

	// set the initial value of top,bottom,left,right
	left = right = x;
	top = bottom = y;

	check = followNo = 0;
	pResultData[y * m_szImage.cx + x] = nDefectColor;

	m_vecPosition[0].x    = x;
	m_vecPosition[0].y    = y;
	m_vecPosition[0].gray = m_pGrabedImage[y * m_szImage.cx + x];
	
	do 
	{
		Xp = m_vecPosition[followNo].x; 
		Yp = m_vecPosition[followNo].y;

		for ( i = -1; i <= 1; i++ )
		{
			Yc = Yp + i;
			if ( Yc < 0 || Yc >= m_szImage.cy ) continue;

			sxWidth = Yc * m_szImage.cx;

			for ( j = -1; j <= 1; j++ )
			{
				Xc = Xp + j;
				if ( Xc < 0 || Xc >= m_szImage.cx) continue;

				if (pResultData[sxWidth+Xc] == OBJ)
				{
					check++;

					// update left,right,top,bottom
					if ( top    >= Yc )    top = Yc;
					if ( bottom <= Yc ) bottom = Yc;
					if ( left   >= Xc )   left = Xc;
					if ( right  <= Xc )  right = Xc;

					// Add a pixel to the blob
					pResultData[sxWidth+Xc]   = nDefectColor;
					m_vecPosition[check].x    = Xc; 
					m_vecPosition[check].y    = Yc;
					m_vecPosition[check].gray = m_pGrabedImage[sxWidth+Xc];
				}
			}
		}

		followNo++;

		if((check+1 >= SEEDNUM) || (followNo+1 >= SEEDNUM)) break;
		
	} while(followNo <= check ) ;

	if ((check+1 >= SEEDNUM) || (followNo+1 >= SEEDNUM))
	{
		return -1;
	}

	// save mbr
	mbr[k].color  = OBJ;
	mbr[k].left   = left;
	mbr[k].right  = right;
	mbr[k].top    = top;
	mbr[k].bottom = bottom;

	m_ptsBlob.resize(followNo);

	for ( i = 0; i < followNo; i++ )
	{
		m_ptsBlob[i].x = m_vecPosition[i].x;
		m_ptsBlob[i].y = m_vecPosition[i].y;

		m_ptsBlob[i].gray = m_vecPosition[i].gray;
	}

	if((followNo * m_nLiftingMask * m_nLiftingMask) < nMinNumOfPixsInBlob)
	{
		return 0;
	}

	return 1;
}



/******************************************************************************************
//	함수명	: OutterBoundaryTracing(LABELING *mbr, int k)
//	입력변수: mbr: LABELING 배열 포인터, k: mbr index
//	OUTPUT  : boundary pixel 개수
//	기능	: blob의 외곽 boundary를 search하는 함수(Dilation 미 수행시 적용)
//	최종변경: 2009. 03. 17 Cho Chul-Hyun
//  이    력: 
//  REMARK  : 
//		FindBlob에서 찾은 blob에 대해 boundary search 후에 찾은 결과 ptsBoundary에 저장
/******************************************************************************************/ 
unsigned int CLabeling::OutterBoundaryTracing(LABELING *mbr, int k)
{
	unsigned int x, y;
	unsigned int bIndex, dir, m;
	unsigned int yN;
	unsigned int Xc, Yc;
	unsigned int sx, sy;
	BOOL bBoundaryStart = FALSE;

	long nPosition;

	double dMeanGray = 0.0f;		// mean gray

	// create the patch image for this blob
	unsigned int patchWidth  = (mbr[k].right - mbr[k].left + 1) + 4;
	unsigned int patchHeight = (mbr[k].bottom - mbr[k].top + 1) + 4;


	BYTE * patchImage = new BYTE[patchWidth*patchHeight];
	memset(patchImage, 0, sizeof(BYTE)*patchWidth*patchHeight);

	for ( std::vector<PIXEL>::iterator it = m_ptsBlob.begin(); it != m_ptsBlob.end(); it++ )
	{
		nPosition = (it->y - mbr[k].top+1) * patchWidth + it->x- mbr[k].left+2;
		patchImage[nPosition-1] = 1;
		patchImage[nPosition  ] = 1;
		patchImage[nPosition+1] = 1;

		nPosition = (it->y - mbr[k].top+2) * patchWidth + it->x- mbr[k].left+2;
		patchImage[nPosition-1] = 1;
		patchImage[nPosition  ] = 1;
		patchImage[nPosition+1] = 1;

		nPosition = (it->y - mbr[k].top+3) * patchWidth + it->x- mbr[k].left+2;
		patchImage[nPosition-1] = 1;
		patchImage[nPosition  ] = 1;
		patchImage[nPosition+1] = 1;
	}

	// Initialize the boundary vector
	int nBoundaryResize = (patchWidth*patchHeight) > (SEEDNUM*2+1000)? (SEEDNUM*2+1000): (patchWidth*patchHeight);
	m_ptsBoundary.resize(nBoundaryResize);

	for ( y = 1; y < patchHeight-1; y++ )
	{
		yN = y * patchWidth;
		for ( x = 1; x < patchWidth -1; x++ )
		{
			if ( patchImage[yN+x] == 1)
			{
				bBoundaryStart = TRUE;
				break;
			}
		}

		if(bBoundaryStart == TRUE) break;
	}

	Xc = mbr[k].left - 2 + x; Yc = mbr[k].top - 2 + y;

	if(Xc <= 0) Xc = 0;
	if(Xc >= m_szImage.cx) Xc = m_szImage.cx-1;
	if(Yc <= 0) Yc = 0;
	if(Yc >= m_szImage.cy) Yc = m_szImage.cy-1;

	m_ptsBoundary[0].x    = Xc;
	m_ptsBoundary[0].y    = Yc;
	m_ptsBoundary[0].gray = m_pGrabedImage[Yc*m_szImage.cx + Xc];

	dMeanGray = m_ptsBoundary[0].gray;

	std::vector<PIXEL> P(8);

	bIndex = 0; dir = 7;

	//Process Step 2 and 3 and 4
	//Set start position
	do 
	{
		// 8 neighborhood positions at the current position
		P[0].x = x+1; P[0].y = y;
		P[1].x = x+1; P[1].y = y-1;
		P[2].x = x  ; P[2].y = y-1;
		P[3].x = x-1; P[3].y = y-1;
		P[4].x = x-1; P[4].y = y;
		P[5].x = x-1; P[5].y = y+1;
		P[6].x = x  ; P[6].y = y+1;
		P[7].x = x+1; P[7].y = y+1;

		// Determine the search direction
		if ( (dir % 2) == 0 ) dir = (dir+7) % 8;
		else                  dir = (dir+6) % 8;

		m = dir;

		// Find new border element among the 8 neightborhood at (x,y)
		do 
		{
			//Get the x,y position corresponding the searching direction "dir"
			sx=P[m].x; sy=P[m].y; 

			//If the current pixel is not a predetermined blob color, escape this loop.
			if (patchImage[sy*patchWidth + sx] == 1) break;

			//Increase m
			m = (m+1) % 8;
			
		} while(1);

		//Update the "dir" value
		dir = m;

		//Update the border position
		x = sx; y = sy;
		Xc = mbr[k].left - 2 + x; Yc = mbr[k].top - 2 + y;

		//Increase the number of border element
		bIndex++;

		//Add the current border element into the border vector
		if(Xc <= 0) Xc = 0;
		if(Xc >= m_szImage.cx) Xc = m_szImage.cx-1;
		if(Yc <= 0) Yc = 0;
		if(Yc >= m_szImage.cy) Yc = m_szImage.cy-1;

		m_ptsBoundary[bIndex].x    = Xc;
		m_ptsBoundary[bIndex].y    = Yc;
		m_ptsBoundary[bIndex].gray = m_pGrabedImage[Yc*m_szImage.cx+Xc];

		dMeanGray += m_ptsBoundary[bIndex].gray;

		//Check whether the border is terminated or Not.
		//If terminated, then escape this do loop
		if ( (bIndex >= 2) && (m_ptsBoundary[bIndex].x   == m_ptsBoundary[1].x)
					       && (m_ptsBoundary[bIndex].y   == m_ptsBoundary[1].y)
						   && (m_ptsBoundary[bIndex-1].x == m_ptsBoundary[0].x)
						   && (m_ptsBoundary[bIndex-1].y == m_ptsBoundary[0].y) ) break;

	} while(1);

	m_ptsBoundary.resize(bIndex);

	mbr[k].dBgMean = dMeanGray/bIndex;

	delete [] patchImage;

	return bIndex;
}


/******************************************************************************************
//	함수명	: NoTwoPassThinning(LABELING * mbr, int k)
//	입력변수: 
//	OUTPUT  : 없음
//	기능	: blob no thinning 함수(이물 검사시 사용, Dilation 안한 것에 사용)
//	최종변경: 2009. 03. 17 Cho Chul-Hyun
//  이    력: 
//  REMARK  : 
/******************************************************************************************/ 
void CLabeling::NoTwoPassThinning(LABELING * mbr, int k)
{
	double dMeanGray = 0.0f;		// mean gray
	int    nBoundaryGray = 0; 
	int    nMaxGray = 0;
	int    nCurGray = 0;

	unsigned int target_count = 0;
	target_count = m_ptsBlob.size();

	nBoundaryGray = (int)(mbr[k].dBgMean+0.5);

	for(int i = 0 ; i < target_count ; i++)
	{
		dMeanGray += m_ptsBlob[i].gray;
		nCurGray = abs(m_ptsBlob[i].gray - nBoundaryGray);
		if ( nCurGray > nMaxGray ) nMaxGray = nCurGray;
	}

	mbr[k].left   = mbr[k].left * m_nLiftingMask;
	mbr[k].right  = mbr[k].right * m_nLiftingMask;
	mbr[k].top    = mbr[k].top * m_nLiftingMask;
	mbr[k].bottom = mbr[k].bottom * m_nLiftingMask;

	mbr[k].dMean = dMeanGray/target_count;
	mbr[k].nArea = target_count * m_nLiftingMask * m_nLiftingMask;
	mbr[k].nPeak = nMaxGray;

	mbr[k].nDx = abs( (long)(mbr[k].right  - mbr[k].left + 1) );
	mbr[k].nDy = abs( (long)(mbr[k].bottom - mbr[k].top + 1) );
}


/******************************************************************************************
//	함수명	: ComputeMBRInformation(LABELING * mbr, int tab)
//	입력변수: mbr: THREADLABLES 배열 포인터, k: mbr index
//	OUTPUT  : 없음
//	기능	: blob 정보 계산 함수
//	최종변경: 2009. 03. 17 Cho Chul-Hyun
//  이    력: 
//  REMARK  : 
//		blob pixels(ptsBlob), boundary pixels(ptsBoundary), target pixels(ptsTarget)의 정보를 이용하여 
//      다음의 blob 정보를 계산하는 함수이다.
//		1. target (실선화된 blob)의 mean gray & std
//		2. background (실제 blob에서 2pixel 떨어진 지점)의 mean gray & std
//		3. center 좌표, blob 회전 방향, blob 크기(major, minor)
//		4. blob 외곽 둘레, compactness
//		5. curvature
//		compactness = blob pixel 개수 / (PI/4 * major^2)
//		curvature = sum of the total distance of the target pixels / target pixel counts
/******************************************************************************************/ 
void CLabeling::ComputeMBRInformation(LABELING * mbr, int tab)
{
	std::vector<PIXEL>::iterator it;

	int nTabSize = tab*2;
	double cenx, ceny;
	double sum;
	double Ixy, Ixx, Iyy;
	double dir;

	//calculate cenx, ceny
	cenx=0.; ceny=0.;
	for ( it = m_ptsBlob.begin(); it != m_ptsBlob.end(); it++ )
	{
		cenx += it->x;
		ceny += it->y;
	}

	cenx /= m_ptsBlob.size();
	ceny /= m_ptsBlob.size();

	mbr->cx = (int)(cenx * m_nLiftingMask);
	mbr->cy = (int)(ceny * m_nLiftingMask);

	mbr->dCx = cenx * m_nLiftingMask;
	mbr->dCy = ceny * m_nLiftingMask;

	mbr->dCenx = (cenx * m_dResolutionX / 1000.0) * m_nLiftingMask;
	mbr->dCeny = (ceny * m_dResolutionY / 1000.0) * m_nLiftingMask;

	//Here, You must consider that the origin of the coordinates is left-bottom.
	Ixx = 0.; Iyy = 0.; Ixy = 0.;
	for ( it = m_ptsBlob.begin(); it != m_ptsBlob.end(); it++ )
	{
		Ixy += (it->x - cenx) * (it->y - ceny);
		Ixx += (it->x - cenx) * (it->x - cenx);
		Iyy += (it->y - ceny) * (it->y - ceny);
	}

	//compute the direction of major axis
	dir = 0.5 * atan2(2.*Ixy, Ixx - Iyy);
	mbr->dDir = dir ;// * 180.0 / PI;

	//compute the lengths of major and minor axes of a blob.
	//alpha(x,y)=x*cos(dir)+y*sin(dir);
	//beta(x,y)=-x*sin(dir)+y*cos(dir);
	double alpha, beta;
	double alphaMin = 10000000., alphaMax = -10000000.;
	double betaMin  = 10000000., betaMax  = -10000000.;
	double cosdir = cos(dir);
	double sindir = sin(dir);
	unsigned int aminX, aminY, amaxX, amaxY, bminX, bminY, bmaxX, bmaxY;

	for ( it = m_ptsBoundary.begin(); it != m_ptsBoundary.end(); it++ )
	{
		alpha =  (it->x * cosdir) + (it->y * sindir);
		beta  = -(it->x * sindir) + (it->y * cosdir);
		if ( alphaMin > alpha )
		{
			alphaMin = alpha;
			aminX = it->x; aminY = it->y;
		}
		if ( alphaMax < alpha )
		{
			alphaMax = alpha;
			amaxX = it->x; amaxY = it->y;
		}
		if ( betaMin > beta )
		{
			betaMin = beta;
			bminX = it->x; bminY = it->y;
		}
		if ( betaMax < beta )
		{
			betaMax = beta;
			bmaxX = it->x; bmaxY = it->y;
		}
	}

	mbr->dMajor = (alphaMax - alphaMin - nTabSize) * m_dResolution * m_nLiftingMask; //um
	mbr->dMinor = (betaMax  - betaMin - nTabSize) * m_dResolution * m_nLiftingMask;  //um

	if(mbr->dMajor <= 0) mbr->dMajor = 1;
	if(mbr->dMinor <= 0) mbr->dMinor = 1;

	//compute boundary length (perimeter)
	double x1, y1, x2, y2;
	sum = 0.;
	for ( it = m_ptsBoundary.begin(); it != m_ptsBoundary.end()-1; it++ )
	{
		x1 = it->x; y1 = it->y;
		x2 = (it+1)->x; y2 = (it+1)->y;
		sum += sqrt((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
	}

	mbr->dPerimeter = sum * m_nLiftingMask * m_nLiftingMask;	// mm2


	// gray-difference
	mbr->dGrayDiff = fabs(mbr->dMean - mbr->dBgMean);

	// target pixel count
	mbr->nArea = m_ptsBlob.size() * m_nLiftingMask * m_nLiftingMask; 
	
	// area (um2)
	mbr->dArea = mbr->nArea * m_dResolutionX * m_dResolutionY ;
	
	mbr->dCompact = (mbr->dArea * 4.) / (PI * mbr->dMajor * mbr->dMajor);
	// 이미지의 특성상 2x2 defect 는 정확한 원과 4/3.141592 의 오차 발생하여 1이상 값이 나옴
	if (mbr->dCompact > 1) mbr->dCompact = 1;

	//	dThickness = nArea/ long size(pixel)
	double dLongSize = (mbr->dMajor/m_dResolution);
	if( dLongSize == 0.0 ) 
	{
		mbr->dThickness = mbr->nArea;
	}
	else 
	{
		mbr->dThickness = mbr->nArea / dLongSize;
	}

	//compute the curvature of the blob
	//curvature = sum of the total distance / pixel counts
	if ( m_ptsBlob.size() > 2 )
	{
		//linear equation: ax + bx + c = 0
		//where,
		//	a = y1 - y2
		//	b = x2 - x1
		//	c = x1*y2 - x2*y1
		double x1 = m_ptsBlob.front().x; 
		double y1 = m_ptsBlob.front().y;
		double x2 = m_ptsBlob.back().x;  
		double y2 = m_ptsBlob.back().y;

		double line_a = y1 - y2;
		double line_b = x2 - x1;
		double line_c = x1*y2 - x2*y1;

		//distance: |ax1+by1+c|/sqrt(a^2+b^2)
		double dist = sqrt(line_a*line_a + line_b*line_b);
		sum = 0.;
		for ( it = m_ptsBlob.begin()+1; it != m_ptsBlob.end()-1; it++ )
		{
			sum += fabs(line_a * it->x + line_b * it->y + line_c)/dist;
		}
		mbr->dCurvature = sum/(m_ptsBlob.size() - 2);
		mbr->dCurvature = mbr->dCurvature * m_nLiftingMask * m_nLiftingMask;
	}
	else
	{
		mbr->dCurvature = 0.;
	}
}



/******************************************************************************************
//	함수명	: MakeExtendDefectType(LABELING *mbr, int nBlobs)
//	입력변수: 
//	기능	: defect merge(CONVEX_TYPE, CONCAVE_TYPE, MERGE_TYPE)
//	최종변경: 2009. 03. 17 Cho Chul-Hyun
//  이    력: 
/******************************************************************************************/  
BOOL CLabeling::MakeExtendDefectType(LABELING *mbr, int nBlobs)
{
	int nLeft, nRight, nTop, nBottom;
	int ul, ur, ut, ub;

	if(nBlobs == 0) return FALSE;

	// set merge range
	ul = mbr[nBlobs].left   - m_nMergeDistance; 
	ur = mbr[nBlobs].right  + m_nMergeDistance; 
	ut = mbr[nBlobs].top    - m_nMergeDistance; 
	ub = mbr[nBlobs].bottom + m_nMergeDistance; 

	if ( ul < 0 ) ul = 0;
	if ( ut < 0 ) ut = 0;

	for(int nIndex = 0; nIndex < nBlobs; nIndex++)
	{
		nLeft   = max(ul, mbr[nIndex].left);
		nRight  = min(ur, mbr[nIndex].right);
		nTop    = max(ut, mbr[nIndex].top);
		nBottom = min(ub, mbr[nIndex].bottom);
		if((nRight >= nLeft) && (nBottom >= nTop))
		{
			if(mbr[nIndex].color > mbr[nBlobs].color)
			{
				if ( m_nConvexType == 0 )	// white -> black
				{
					mbr[nIndex].nExtType = (mbr[nIndex].cy < mbr[nBlobs].cy) ? CONVEX_TYPE : CONCAVE_TYPE;
				}
				else	// black -> white
				{
					mbr[nIndex].nExtType = (mbr[nIndex].cy < mbr[nBlobs].cy) ? CONCAVE_TYPE : CONVEX_TYPE;
				}
			}
			else if(mbr[nIndex].color < mbr[nBlobs].color)
			{
				if ( m_nConvexType == 0 )	// white -> black
				{
					mbr[nIndex].nExtType = (mbr[nIndex].cy > mbr[nBlobs].cy) ? CONVEX_TYPE : CONCAVE_TYPE;
				}
				else	// black -> white
				{
					mbr[nIndex].nExtType = (mbr[nIndex].cy > mbr[nBlobs].cy) ? CONCAVE_TYPE : CONVEX_TYPE;
				}
			}
			else
			{
				if(mbr[nIndex].nExtType == 0)
				{
					mbr[nIndex].nExtType = MERGE_TYPE;
				}
			}

			// mbr 확장
			mbr[nIndex].left   = min(mbr[nIndex].left, mbr[nBlobs].left);
			mbr[nIndex].top    = min(mbr[nIndex].top, mbr[nBlobs].top);
			mbr[nIndex].right  = max(mbr[nIndex].right, mbr[nBlobs].right);
			mbr[nIndex].bottom = max(mbr[nIndex].bottom, mbr[nBlobs].bottom);
			mbr[nIndex].dSize += mbr[nBlobs].dSize;

			mbr[nIndex].nDx    = mbr[nIndex].right - mbr[nIndex].left + 1;
			mbr[nIndex].nDy    = mbr[nIndex].bottom - mbr[nIndex].top + 1;

			mbr[nIndex].nArea += mbr[nBlobs].nArea;
			mbr[nIndex].nPeak  = max(mbr[nIndex].nPeak, mbr[nBlobs].nPeak);

			mbr[nIndex].dGrayDiff = max(mbr[nIndex].dGrayDiff, mbr[nBlobs].dGrayDiff);

			mbr[nIndex].cx     = mbr[nIndex].left+abs( (int)(mbr[nIndex].right-mbr[nIndex].left) )/2;
			mbr[nIndex].cy     = mbr[nIndex].top+abs(  (int)(mbr[nIndex].bottom-mbr[nIndex].top) )/2;
			mbr[nIndex].dCenx  = mbr[nIndex].cx * m_dResolutionX / 1000.0;
			mbr[nIndex].dCeny  = mbr[nIndex].cy * m_dResolutionY / 1000.0;

			mbr[nIndex].dCompact = max(mbr[nIndex].dCompact, mbr[nBlobs].dCompact);
			mbr[nIndex].dMajor   = sqrt( (double)(mbr[nIndex].nDx*mbr[nIndex].nDx*m_dResolutionX*m_dResolutionX) + 
										 	   (double)(mbr[nIndex].nDy*mbr[nIndex].nDy*m_dResolutionY*m_dResolutionY) );
			mbr[nIndex].dMinor   = max(mbr[nIndex].dMinor, mbr[nBlobs].dMinor);
			
			return TRUE;
		}
	}

	return FALSE;
}

/******************************************************************************************
//	함수명	: DeleteMBR(BYTE *pResultData,BYTE nDefectColor)  
//	입력변수: 
//	기능	: mbr제거 함수 
//	최종변경: 2009. 03. 17 Cho Chul-Hyun
//  이    력: 
/******************************************************************************************/ 
void CLabeling::DeleteMBR(BYTE *pResultData,BYTE nDefectColor)
{
	for ( std::vector<PIXEL>::iterator it = m_ptsBlob.begin(); it != m_ptsBlob.end(); it++ )
	{
		if(pResultData[it->y*m_szImage.cx+it->x] == nDefectColor)
		{
			pResultData[it->y*m_szImage.cx+it->x] = FALSEDONE;	   // data 삭제
		}
	}
}


/******************************************************************************************
//	함수명	: SaveMBR(LABELING *mbr, int k)
//	입력변수: 
//	기능	: mbr 저장 함수 
//	최종변경: 2009. 03. 17 Cho Chul-Hyun
//  이    력: 
/******************************************************************************************/  
void CLabeling::SaveMBR(LABELING *mbr, int k)
{
	mbr[k].overlaped = false;
}

