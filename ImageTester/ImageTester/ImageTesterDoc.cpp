
// ImageTesterDoc.cpp : CImageTesterDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageTester.h"
#endif

#include "ImageTesterDoc.h"
#include "ImageViewer.h"
#include "MessageView.h"

#include <propkey.h>

#include <iostream> 
#include <fstream> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageTesterDoc

IMPLEMENT_DYNCREATE(CImageTesterDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageTesterDoc, CDocument)
	ON_COMMAND(ID_GAUSSIAN_3X3, &CImageTesterDoc::OnGaussian3x3)
	ON_COMMAND(ID_GAUSSIAN_5X5, &CImageTesterDoc::OnGaussian5x5)
	ON_COMMAND(ID_GAUSSIAN_7X7, &CImageTesterDoc::OnGaussian7x7)
	ON_COMMAND(ID_BLUR_3X3, &CImageTesterDoc::OnBlur3x3)
	ON_COMMAND(ID_BOX_3X3, &CImageTesterDoc::OnBox3x3)
	ON_COMMAND(ID_MEDIAN_3X3, &CImageTesterDoc::OnMedian3x3)
	ON_COMMAND(ID_FILTER_SOBEL, &CImageTesterDoc::OnFilterSobel)
	ON_COMMAND(ID_FILTER_LAPLACIAN, &CImageTesterDoc::OnFilterLaplacian)
END_MESSAGE_MAP()


// CImageTesterDoc 생성/소멸

CImageTesterDoc::CImageTesterDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_rtROI[0] = CRect( 0,0,0,0 );
	m_rtROI[1] = CRect( 0,0,0,0 );

	m_indexSelect = SELECT_FIRST;

	m_iSelectedImage = DOC_DEFAULT;

	m_nListFPoint[0] = 0;
	m_nListFPoint[1] = 0;

	m_bRoi = FALSE;

	QueryPerformanceFrequency(&m_counter_Freq);

	m_pIPModule = new CImageProcess_C;

	m_bDisplayProcessingImage = FALSE;

// 	CImageProcess_C ic;
// 
// 	char a[999];
// 	memset(a, 0, 999);
// 	CString str;
// 	str.Format("D:\\a\\");
// 	memcpy(a, str.GetBuffer(), str.GetLength());
// 
// 	ic.setFilePath(a,999);
}

CImageTesterDoc::~CImageTesterDoc()
{
	for( list<IT_IMAGE*>::iterator ite = m_listImage.begin() ; ite!=m_listImage.end() ; ite++ )
	{
		IT_IMAGE* pImg = *ite;
		delete[] pImg;
	}
	m_listImage.clear();

	if( m_pIPModule )
	{
		delete[] m_pIPModule;
	}
	m_pIPModule = NULL;

// 	char a[999];
// 	memset(a, 0, 999);
// 	CString str;
// 	str.Format("D:\\a\\");
// 	memcpy(a, str.GetBuffer(), str.GetLength());
// 
// 	CString str2;
// 	str2 = a;
// 	int b = 0;
}

BOOL CImageTesterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CImageTesterDoc serialization

void CImageTesterDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageTesterDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CImageTesterDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageTesterDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageTesterDoc 진단

#ifdef _DEBUG
void CImageTesterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageTesterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageTesterDoc 명령


void CImageTesterDoc::Test(void)
{
	//
	// AF dll 테스트
	//
/*	int err;
	CString strPort;
	strPort.Format("COM3");

 	err = Atf_OpenConnection(strPort.GetBuffer(), 9600);

	err = Atf_MoveZ(2000);

	err = Atf_MoveZ(-2000);


	Atf_CloseConnection();

	if( get_image() )
	{
		start_global_counter();

		// ROI
		int sx, ex, sy, ey;
		GetInspectionRegion( &sx, &ex, &sy, &ey );

		//
// 		double dValue = m_pIPModule->getFocusValue(	get_image_buffer(), get_width_step(), get_height(), 
// 													sx, ex, sy, ey );

		double dValue = getFocusValue(	get_image_buffer(), get_width_step(), get_height(), 
			sx, ex, sy, ey );

		end_global_counter("=> insp_focus - OK");

		
		CString strMessage;
		strMessage.Format("Focus value = %f\n", dValue );
		write_message(strMessage);
	}*/


	//
	// Defocus 제거 테스트
	//
/*	if( get_image() )
	{

		CBhImage img;
		img.Create(get_width(), get_height(), 8, 0 );
		memset(img.GetImage(), 0, get_width()*get_height());

		int iSize = 5;
		int iSum=0;

		if( 1 )
		{
			// defocus 제거
			for( int y=iSize ; y<get_height() - iSize ; y++ )
			{
				for( int x=iSize ; x<get_width() - iSize ; x++ )
				{
					iSum = 0;
					for( int k=-iSize ; k<=iSize ; k++ )
					{
						iSum += (int)(get_image_buffer()[ (x+k) + y*get_width() ]) - 128;
					}

					img.GetImage()[ x + y*get_width() ] = min( 255, max( 0, 128 + iSum ) );
				}
			}
		}

		img.Save("D:\\result.BMP");
	}
	*/


	/*
	focus 값 계산
	if( get_image() )
	{
		start_global_counter();

		double fValue = getFocusValue(	get_image_buffer(), 
										get_width(), 
										get_height(), 
										0, get_width()-1, 
										0, get_height()-1 );


		end_global_counter("=> insp_focus - OK");


		CString strMessage;
		strMessage.Format("Focus value = %f", fValue);
		write_message(strMessage);
	}
	*/


/*
	// Derectory 생성 테스트
	CString strFile;
	strFile.Format("d:\\processing\\%d_insp_ppa_result", GetTickCount());

	CString tmp = strFile;
	CString tok;
	while(0 <= tmp.Find("\\"))
	{
		tok += tmp.Left(tmp.Find("\\")+1);
		tmp = tmp.Right(tmp.GetLength() - (tmp.Find("\\")+1));

		CreateDirectory(tok, NULL);
	}

	CreateDirectory(strFile, NULL);
*/

/*
	m_pIPModule->displayImage((bool)m_bDisplayProcessingImage);

	// template matching
	m_pIPModule->load_ncc_model( "d:\\images\\templateimage.bmp");

	float fX = 0.0;
	float fY = 0.0;
	m_pIPModule->find_ncc_model( m_pImage->GetImage(), 
		m_pImage->GetWidth(), 
		m_pImage->GetHeight(), 
		0, m_pImage->GetWidth() - 1,
		0, m_pImage->GetHeight() - 1,
		&fX, &fY);
*/



	CImageProcess_H *ip = new CImageProcess_H;

	ip->displayImage((bool)m_bDisplayProcessingImage);

	double dx, dy;
	ip->insp_ppa_image_based(	"d:\\images\\templateimage.bmp", 
								m_pImage->GetImage(), 
								m_pImage->GetWidth(), 
								m_pImage->GetHeight(), 
								0, m_pImage->GetWidth() - 1,
								0, m_pImage->GetHeight() - 1,
								0.8,	// score
						// 		m_pImage->GetWidth()/2, m_pImage->GetHeight()/2, 
								0, 0, 
								&dx, &dy );

	delete ip;

}

//////////////////////////////////////////////////////////////////////////
//
// Read CAD file
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::Test2(void)
{
	CDxf dxfReader;

	dxfReader.LoadFile("D:\\image\\E_Panel-1.dxf");

	write_message("Read CAD file...");


	CString strMessage;
	strMessage.Format("  #Line = %d", dxfReader.m_LineCount );
	write_message(strMessage);

	strMessage.Format("  #Arc = %d", dxfReader.m_ArcCount );
	write_message(strMessage);

	strMessage.Format("  #Circle = %d", dxfReader.m_CircleCount );
	write_message(strMessage);

	strMessage.Format("  #PolyLine = %d", dxfReader.m_PolyLineCount );
	write_message(strMessage);

	strMessage.Format("  #Spline = %d", dxfReader.m_SplineCount );
	write_message(strMessage);
}


BOOL CImageTesterDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	m_strFileName = lpszPathName;
	
	CString strFile;
	strFile = lpszPathName;

	char *pfile = new char[strFile.GetLength()+1];
	strcpy_s( pfile, strFile.GetLength()+1, strFile);	
	

	// open cv version
// 	CImageProcess_Open clsIP;
// 	int width, height;
// 
// 	BYTE *pImg = clsIP.load_image( pfile, &width, &height );
// 
// 
// 	if( pImg )
// 	{
// 		get_image()->Create(width, height, 8, 1);
// 		memcpy( get_image_buffer(), pImg, width*height );
// 		get_image()->Invalidate();
// 
// 		IT_IMAGE img;
// 		img.Create(width, height, 8, 1 );
// 		memcpy( img.GetImage(), pImg, width*height );
// 		img.Save("d:\\save.bmp");
// 	}


	//
	get_image()->Open(strFile);

	m_pViewImage->Invalidate(FALSE);

	delete[] pfile;

	return TRUE;
}


//
// display할 영상 선택
//	- source or result
//
void CImageTesterDoc::SetDisplayMode( SELECT_IMAGE index )
{
	m_indexSelect = index;
}


//
// display할 영상 변경
//
void CImageTesterDoc::SetDisplayModeFlip()
{
	switch( m_indexSelect )
	{
	case SELECT_FIRST:
		m_indexSelect = SELECT_SECOND;
		break;

	case SELECT_SECOND:
		m_indexSelect = SELECT_FIRST;
		break;
	}

	m_pViewImage->ResizeView();

	UpdateAllViews(FALSE);
}


//
// display할 영상 변경
//
int CImageTesterDoc::get_next_image_index()
{
	switch( m_indexSelect )
	{
	case SELECT_FIRST:
		return SELECT_SECOND;
		break;

	case SELECT_SECOND:
		return SELECT_FIRST;
		break;
	}

	return 0;
}


IT_IMAGE *CImageTesterDoc::get_next_image()
{
	return &m_pImage[get_next_image_index()];
}

//
// 현재 displasy 중인 영상에 대한 pointer
//
// IplImage* CImageTesterDoc::GetIplImage()
// {
// 	if( m_pImage[m_indexSelect] )
// 	{
// 		return m_pImage[m_indexSelect];
// 	}
// 
// 	return NULL;
// }

BOOL CImageTesterDoc::PrepareNextImage()
{
	int width = get_width_step();
	int height = get_height();

	if( width<1 || height<1 )
	{
		return FALSE;
	}

	get_next_image()->Create( width, height, 8, 0 );

	// 초기화
	memset( get_next_image_buffer(), 0, width*height );

	return TRUE;
}

//
// 현재 displasy 중인 영상에 대한 pointer
//
BYTE* CImageTesterDoc::get_image_buffer()
{
// 	if( m_pImage[m_indexSelect] )
	{
		return m_pImage[m_indexSelect].GetImage();
	}

	return NULL;
}


IT_IMAGE* CImageTesterDoc::get_image()
{
// 	if( m_pImage[m_indexSelect] )
// 	{
		return &m_pImage[m_indexSelect];
// 	}

	return NULL;
}


int CImageTesterDoc::get_width()
{
// 	if( m_pImage[m_indexSelect] )
	{
		return m_pImage[m_indexSelect].GetWidth();
	}

	return 0;
}


int CImageTesterDoc::get_width_step()
{
// 	if( m_pImage[m_indexSelect] )
	{
		return m_pImage[m_indexSelect].GetWidthStep();
	}

	return 0;
}


int CImageTesterDoc::get_height()
{
// 	if( m_pImage[m_indexSelect] )
	{
		return m_pImage[m_indexSelect].GetHeight();
	}

	return 0;

}

//
// 현재 displasy 중인 영상에 대한 pointer
//
// IplImage* CImageTesterDoc::Get_NextIplImage()
// {
// 	if( m_pImage[Get_NextImageIndex()] )
// 	{
// 		return m_pImage[Get_NextImageIndex()];
// 	}
// 
// 	return NULL;
// }

//
// 현재 displasy 중인 영상에 대한 pointer
//
BYTE* CImageTesterDoc::get_next_image_buffer()
{
// 	if( m_pImage[Get_NextImageIndex()] )
	{
		return m_pImage[get_next_image_index()].GetImage();
	}

	return NULL;
}

BOOL CImageTesterDoc::SetROI( CRect roi )
{
	m_rtROI[m_indexSelect] = roi;

	UpdateAllViews(FALSE);

	return TRUE;
}

BOOL CImageTesterDoc::valid_Roi( BOOL bValid )
{
	m_bRoi = bValid;

	return TRUE;
}

BOOL CImageTesterDoc::GetROI( CRect *roi )
{
	*roi = m_rtROI[m_indexSelect];

	return TRUE;
}


BOOL CImageTesterDoc::displayProcessingImage( BOOL bValid )
{
	m_bDisplayProcessingImage = bValid;

	return TRUE;
}

BOOL CImageTesterDoc::GetInspectionRegion( int *sx, int *ex, int *sy, int *ey )
{
	if( m_bRoi )
	{
		CRect roi;
		GetROI(&roi);

		*sx = roi.left;
		*ex = roi.right - 1;
		*sy = roi.top;
		*ey = roi.bottom -1;
	} else {
		*sx = 0;
		*ex = get_width() - 1;
		*sy = 0;
		*ey = get_height() - 1;
	}

	return TRUE;
}

void CImageTesterDoc::DeleteROI()
{

}

int CImageTesterDoc::GetPointNum()
{
	return (int)m_listPoint->size();
}

void CImageTesterDoc::AddPoint( CPoint pt )
{
	m_listPoint->push_back(pt);
}

BOOL CImageTesterDoc::GetPoint( CPoint *pt )
{
	return TRUE;
}


int CImageTesterDoc::get_display_circle_num()
{
	return (int)get_display_circle()->size();
}

void CImageTesterDoc::add_display_circle( double cx, double cy, double radius )
{
	CIRCLED circle;
	circle.cx = cx;
	circle.cy = cy;
	circle.radius = radius;

	get_display_circle()->push_back(circle);
}


void CImageTesterDoc::clear_display_circle()
{
	get_display_circle()->clear();
}


list<CIRCLED>* CImageTesterDoc::get_display_circle()
{
	return &m_listCircle[m_indexSelect];
}


void CImageTesterDoc::add_result_data( stResutltData resultData )
{
	m_listResultData.push_back(resultData);
}

void CImageTesterDoc::clear_result_data()
{
	m_listResultData.clear();
}

void CImageTesterDoc::check_result_data()
{
	vector<stResutltData>::iterator ite;

	float average_x=0, average_y=0, std_x=0, std_y=0;

	CString strAll, strTemp;
	for( ite = m_listResultData.begin() ; ite!=m_listResultData.end() ; ite++ )
	{
		average_x += ite->x;
		average_y += ite->y;

		strTemp.Format("x=%f, y=%f\n", ite->x, ite->y );
		strAll += strTemp;
	}

	//
	average_x /= m_listResultData.size();
	average_y /= m_listResultData.size();

	// anaysis
	int nData = (int)m_listResultData.size();
	float fX_Min=FLT_MAX, fX_Max=-FLT_MIN;
	float fY_Min=FLT_MAX, fY_Max=-FLT_MAX;

	for( ite = m_listResultData.begin() ; ite!=m_listResultData.end() ; ite++ )
	{
		std_x += pow( ite->x - average_x, 2.f );
		std_y += pow( ite->y - average_y, 2.f );

		fX_Min = min(fX_Min, ite->x );
		fX_Max = max(fX_Max, ite->x );

		fY_Min = min(fY_Min, ite->y );
		fY_Max = max(fY_Max, ite->y );
	}

	std_x = sqrt( std_x )/m_listResultData.size();
	std_y = sqrt( std_y )/m_listResultData.size();

	strTemp.Format("x_min=%f, x_max=%f, x_diff=%f\n", fX_Min, fX_Max, fX_Max-fX_Min );
	strAll += strTemp;

	strTemp.Format("y_min=%f, y_max=%f, y_diff=%f\n", fY_Min, fY_Max, fY_Max-fY_Min );
	strAll += strTemp;

	strTemp.Format("std_x=%f, std_y=%f\n", std_x, std_y );
	strAll += strTemp;


	AfxMessageBox(strAll);
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::ImageList_Push()
{
	if( get_image_buffer() )
	{
		IT_IMAGE *img = new IT_IMAGE;
 		img->Copy(get_image());

		m_listImage.push_back( img );

// 		IT_IMAGE img;
// 		img.Copy(GetImage());
// 		img.Open("d:\\a.bmp");

// 		m_listImage.push_back( img );

		UpdateAllViews(FALSE);
	}

}

//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::ImageList_Pop()
{
	IT_IMAGE *pSrcImage = ImageList_GetSelectedImage();

	if( pSrcImage )
	{
		m_pImage[m_indexSelect].Copy(pSrcImage);
// 		IplImage *pImg = GetImage();
// 		pImg = cvCloneImage(pSrcImage);
	}

	UpdateAllViews(FALSE);
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::ImageList_Delete()
{
	int index;
	if( ImageList_GetSelectedIndex(&index) )
	{
		list<IT_IMAGE*>::iterator ite = m_listImage.begin();

		for( int i=0 ; i<index ; i++ )
		{
			ite++;
		}
		
		delete[] *ite;
		m_listImage.erase(ite);

		m_iSelectedImage = DOC_DEFAULT;

		UpdateAllViews(FALSE);
	} 
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
BOOL CImageTesterDoc::ImageList_GetSelectedIndex(int *index)
{
	if( m_iSelectedImage>=0 )
	{
		*index = m_iSelectedImage;
		return TRUE;
	}

	*index = -1;
	return FALSE;
}

IT_IMAGE* CImageTesterDoc::ImageList_GetImage(int n)
{
	int nImage = (int)m_listImage.size();
	IT_IMAGE *pImage = NULL;

	if( n<nImage )
	{
		list<IT_IMAGE*>::iterator ite = m_listImage.begin();

		for( int i=0 ; i<n ; i++ )
		{
			ite++;
		}

		pImage = *ite;
	} 

	return pImage;
}

IT_IMAGE* CImageTesterDoc::ImageList_GetSelectedImage()
{
	int nImage = (int)m_listImage.size();

	IT_IMAGE *pImage = NULL;

	int index;
	if( ImageList_GetSelectedIndex(&index) )
	{
		list<IT_IMAGE*>::iterator ite = m_listImage.begin();

		for( int i=0 ; i<index ; i++ )
		{
			ite++;
		}

		pImage = *ite;
	} 

	return pImage;
}

int CImageTesterDoc::ImageList_GetNum()
{
	return (int)m_listImage.size();
}

void CImageTesterDoc::ImageList_Select(int i)
{
	int nImage = (int)m_listImage.size();

	if( i>=0 && i < nImage )
	{
		m_iSelectedImage = i;

		UpdateAllViews(FALSE);
	}
}


//////////////////////////////////////////////////////////////////////////
//
// Create shape model template
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::save_shape_model()
{
	start_global_counter();


	CString str;
	str.Format(FILE_PATH_SHAPE_MODEL);

	char *pfile = new char[str.GetLength()+1];
	strcpy_s( pfile, str.GetLength()+1, str );

	m_pIPModule->save_shape_model( pfile );

	delete[] pfile;


	end_global_counter("save_shape_model");
}


//////////////////////////////////////////////////////////////////////////
//
// Create shape model template
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::load_shape_model()
{
	start_global_counter();


	CString str;
	str.Format(FILE_PATH_SHAPE_MODEL);

	char *pfile = new char[str.GetLength()+1];
	strcpy_s( pfile, str.GetLength()+1, str);

	m_pIPModule->load_shape_model( pfile );

	delete[] pfile;


	end_global_counter("load_shape_model");
}

//////////////////////////////////////////////////////////////////////////
//
// Create shape model template
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::save_ncc_model()
{
	start_global_counter();

	CString str;
	str.Format(FILE_PATH_NCC_MODEL);

	char *pfile = new char[str.GetLength()+1];
	strcpy_s( pfile, str.GetLength()+1, str );

	m_pIPModule->save_ncc_model( pfile );

	delete[] pfile;


	end_global_counter("save_ncc_model");
}


//////////////////////////////////////////////////////////////////////////
//
// Create shape model template
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::load_ncc_model()
{
	start_global_counter();

	CString str;
	str.Format(FILE_PATH_NCC_MODEL);

	char *pfile = new char[str.GetLength()+1];
	strcpy_s( pfile, str.GetLength()+1, str);

	m_pIPModule->load_ncc_model( pfile );

	end_global_counter("load_ncc_model");

	delete[] pfile;
}

//////////////////////////////////////////////////////////////////////////
//
// Create shape model template
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::create_shape_model()
{
// 	if( !get_image() )
// 	{
// 		return;
// 	}
// 
// 
// 	// Roi 정보
// 	int sx, ex, sy, ey;
// 	GetInspectionRegion( &sx, &ex, &sy, &ey );
// 
// 
// 	//
// 	start_global_counter();
// 		
// 	m_pIPModule->create_shape_model( get_image_buffer(), get_width(), get_height(), sx, sy, ex, ey, 3 );
// 
// 	end_global_counter("=> Create shape model");
// 
// 
// 	// overflow 검토
// 	for( UINT i=0 ; i<m_pIPModule->m_contourVector.size() ; i++ )
// 	{
// 		m_listLine[m_indexSelect][i].clear();
// 		list<FPoint>::iterator ite;
// 		for( ite = m_pIPModule->m_contourVector.at(i).begin() ; ite!=m_pIPModule->m_contourVector.at(i).end() ; ite++ )
// 		{
// 			FPoint pt;
// 			pt.x = ite->x;
// 			pt.y = ite->y;
// 
// 			m_listLine[m_indexSelect][i].push_back(pt);
// 		}
// 	}
// 	m_nListFPoint[m_indexSelect] = (int)m_pIPModule->m_contourVector.size();
// 
// 
// 	m_pViewImage->Invalidate(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//
// Create shape model template
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::create_ncc_model()
{
	write_message("------- Create NCC model -------");

	if( get_image() )
	{
		// Roi 정보
		int sx, ex, sy, ey;
		GetInspectionRegion( &sx, &ex, &sy, &ey );


		start_global_counter();
		

		//
		//
		//
		m_pIPModule->create_ncc_model( get_image_buffer(), get_width(), get_height(), sx, ex, sy, ey, _IP_DARK_ );

		end_global_counter("=> Create NCC model");
	}

	m_pViewImage->Invalidate(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//
// find shape model
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::find_shape_model()
{
// 	if( get_image() )
// 	{
// 		float cx, cy;
// 
// 		// ROI
// 		int sx, ex, sy, ey;
// 		GetInspectionRegion( &sx, &ex, &sy, &ey );
// 
// 
// 		//
// 		start_global_counter();
// 
// 		bool bSucess = m_pIPModule->find_shape_model(	get_image_buffer(), get_width_step(), get_height(), 
// 												sx, ex, sy, ey,	// ROI
// 												&cx, &cy,		// 
// 												3, 				// Pyramid level
// 												_IP_FACET_);
// 		if( bSucess )
// 		{
// 			end_global_counter("=> Matching(OK)");
// 		} else {
// 			end_global_counter("=> Matching(Fail)");
// 		}
// 
// 
// 		if( bSucess )
// 		{
// 			FPoint ptResult;
// 
// 			for( UINT i=0 ; i<m_pIPModule->m_contourVector_shapeModel.size() ; i++ )
// 			{
// 				m_listLine[m_indexSelect][i].clear();
// 
// 				list<pointShape>::iterator ite;
// 				for( ite = m_pIPModule->m_contourVector_shapeModel.at(i).begin() ; ite!=m_pIPModule->m_contourVector_shapeModel.at(i).end() ; ite++ )
// 				{
// 					ptResult.x = cx + ite->x;
// 					ptResult.y = cy + ite->y;
// 
// 					m_listLine[m_indexSelect][i].push_back(ptResult);
// 				}
// 			}
// 			m_nListFPoint[m_indexSelect] = (int)m_pIPModule->m_contourVector_shapeModel.size();
// 
// 
// 			stResutltData resultData;
// 
// 			resultData.x = cx;
// 			resultData.y = cy;
// 
// 			add_result_data( resultData );
// 
// 		}
// 	}
// 
// 	m_pViewImage->Invalidate(FALSE);

}


//////////////////////////////////////////////////////////////////////////
//
// find shape model
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::find_ncc_model()
{
	if( get_image() )
	{
		float xResult, yResult;

		int sx, ex, sy, ey;
		GetInspectionRegion( &sx, &ex, &sy, &ey );


		start_global_counter();
		//
// 		bool bSucess = clsIP.find_ncc_model( GetImage(), GetWidthStep(), GetHeight(), sx, ex, sy, ey, &xResult, &yResult );
		bool bSucess = m_pIPModule->find_ncc_model( get_image_buffer(), get_width_step(), get_height(), sx, ex, sy, ey, &xResult, &yResult, 2 );

		if( bSucess )
		{
			end_global_counter("=> Matching(NCC)");

			stResutltData resultData;

			resultData.x = xResult;
			resultData.y = yResult;

			add_result_data( resultData );

		} else {
			end_global_counter("=> Matching(NCC) - Fail -");
		}


		//
		// overflow 검토
		if(bSucess)
		{
			clear_display_circle();

			add_display_circle( xResult, yResult, 10 );
		}
	}

	m_pViewImage->Invalidate(FALSE);

}


//////////////////////////////////////////////////////////////////////////
//
//	create_main_mark
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::create_main_mark()
{
/*
	if( get_image() )
	{
		int sx, ex, sy, ey;
		GetInspectionRegion( &sx, &ex, &sy, &ey );


		start_global_counter();

		bool bSucess = m_pIPModule->create_main_pattern( get_image_buffer(), get_width_step(), get_height(), sx, sy, ex, ey, 0, 60 );	// Align 용 param

		if( bSucess )
		{
			end_global_counter("=> create main align mark");
		} else {
			end_global_counter("=> create main align mark - Fail -");
		}


		//
		// Display : shape model일 때
		//
// 		for( int i=0 ; i<clsIP.m_contourVector.size() ; i++ )
// 		{
// 			m_listFPoint[m_indexSelect][i].clear();
// 			list<FPoint>::iterator ite;
// 			for( ite = clsIP.m_contourVector.at(i).begin() ; ite!=clsIP.m_contourVector.at(i).end() ; ite++ )
// 			{
// 				FPoint pt;
// 				pt.x = ite->x;
// 				pt.y = ite->y;
// 
// 				m_listFPoint[m_indexSelect][i].push_back(pt);
// 			}
// 		}
// 		m_nListFPoint[m_indexSelect] = clsIP.m_contourVector.size();


		//
		// Display NCC model일 때
		// overflow 검토
		m_listLine[m_indexSelect][0].clear();
		FPoint tempPoint;
		float fRadius = (ex-sx)/2.f;
		for( int angle = 0 ; angle<360 ; angle++ )
		{
			tempPoint.x = (float)( sx + m_pIPModule->m_nccModel.cx + fRadius*cos(angle*_IP_PI_/180.f) );
			tempPoint.y = (float)( sy + m_pIPModule->m_nccModel.cy + fRadius*sin(angle*_IP_PI_/180.f) );

			m_listLine[m_indexSelect][0].push_back(tempPoint);
		}
		m_nListFPoint[m_indexSelect] = 1;

	}

	m_pViewImage->Invalidate(FALSE);*/
}


//////////////////////////////////////////////////////////////////////////
//
//	create_sub_mark
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::create_sub_mark()
{
/*
	if( get_image() )
	{
		int sx, ex, sy, ey;
		GetInspectionRegion( &sx, &ex, &sy, &ey );


		start_global_counter();

		bool bSucess = m_pIPModule->create_sub_pattern( get_image_buffer(), get_width_step(), get_height(), sx, sy, ex, ey );

		if( bSucess )
		{
			end_global_counter("=> create main align mark");
		} else {
			end_global_counter("=> create main align mark - Fail -");
		}

		//
		for( UINT i=0 ; i<m_pIPModule->m_contourVector.size() ; i++ )
		{
			m_listLine[m_indexSelect][i].clear();
			list<FPoint>::iterator ite;
			for( ite = m_pIPModule->m_contourVector.at(i).begin() ; ite!=m_pIPModule->m_contourVector.at(i).end() ; ite++ )
			{
				FPoint pt;
				pt.x = ite->x;
				pt.y = ite->y;

				m_listLine[m_indexSelect][i].push_back(pt);
			}
		}
		m_nListFPoint[m_indexSelect] = (int)m_pIPModule->m_contourVector.size();
	}

	m_pViewImage->Invalidate(FALSE);*/
}


//////////////////////////////////////////////////////////////////////////
//
//	find_main_mark
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::find_main_mark(int index)
{
	if( !get_image_buffer())
	{
		return;
	}

	CImageProcess_C clsIp;

	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	
	start_global_counter();
	
	float x, y;
// 	bool bSucess = clsIp.find_main_pattern( GetImageBuffer(), GetWidthStep(), GetHeight(),
// 								sx, sy, ex, ey, 0, 70, index,
// 								&x, &y );
	bool bSucess = m_pIPModule->find_main_pattern_ncc( get_image_buffer(), get_width_step(), get_height(),
														sx, sy, ex, ey,
														index,
														&x, &y,
														2);


	if( bSucess==false )
	{
		end_global_counter("=> find_main_mark - Fail");
		return;
	}
	end_global_counter("=> find_main_mark - OK");


	m_listLine[m_indexSelect][0].clear();
	FPoint tempPoint;
	float fRadius = 10/2;
	for( int angle = 0 ; angle<360 ; angle++ )
	{
		tempPoint.x = x + fRadius*(float)cos(angle*_IP_PI_/180.f);
		tempPoint.y = y + fRadius*(float)sin(angle*_IP_PI_/180.f);

		m_listLine[m_indexSelect][0].push_back(tempPoint);
	}

	m_nListFPoint[m_indexSelect] = 1;

	m_pViewImage->Invalidate(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//
//	find_sub_mark
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::find_sub_mark(int index)
{
	if( !get_image_buffer())
	{
		return;
	}

	CImageProcess_C clsIp;

	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );


	start_global_counter();

	float x, y;
	bool bSucess = clsIp.find_sub_pattern( get_image_buffer(), get_width_step(), get_height(),
		sx, sy, ex, ey, 250, 255, index,
		&x, &y );

	if( bSucess==false )
	{
		end_global_counter("=> find_main_mark - Fail");
		return;
	}
	end_global_counter("=> find_main_mark - OK");

	m_listLine[m_indexSelect][0].clear();
	FPoint tempPoint;
	float fRadius = 50/2;
	for( int angle = 0 ; angle<360 ; angle++ )
	{
		tempPoint.x = x + fRadius*(float)cos(angle*_IP_PI_/180.f);
		tempPoint.y = y + fRadius*(float)sin(angle*_IP_PI_/180.f);

		m_listLine[m_indexSelect][0].push_back(tempPoint);
	}

	m_nListFPoint[m_indexSelect] = 1;

	m_pViewImage->Invalidate(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::calc_align()
{
	float dx, dy, theta;

	m_pIPModule->calc_align( &dx, &dy, &theta);

	CString str;
	str.Format( "dx=%f, dy=%f, theta = %f", dx, dy, theta );
	AfxMessageBox(str);
}

void CImageTesterDoc::write_message(CString strMsg)
{
	m_pViewMessage->m_listBox_message.AddString(strMsg);
}

void CImageTesterDoc::start_global_counter()
{
	QueryPerformanceCounter(&m_globalStartCounter);
}

void CImageTesterDoc::end_global_counter(CString strMsg)
{
	QueryPerformanceCounter(&m_globalEndCounter);

	CString str;
	str.Format(" = %f sec", (double)(m_globalEndCounter.QuadPart - m_globalStartCounter.QuadPart) / (double)m_counter_Freq.QuadPart );

	write_message(strMsg+str);
}

void CImageTesterDoc::start_local_counter()
{
	QueryPerformanceCounter(&m_localStartCounter);
}

void CImageTesterDoc::end_local_counter(CString strMsg)
{
	QueryPerformanceCounter(&m_localEndCounter);

	CString str;
	str.Format(" = %f sec", (double)(m_localEndCounter.QuadPart - m_localStartCounter.QuadPart) / (double)m_counter_Freq.QuadPart );

	write_message(strMsg+str);
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::Save_image( BYTE *pImage, int width, int height, CString strFileName )
{
	IT_IMAGE img;
	img.Create(width, height, 8, 1 );
	memcpy( img.GetImage(), pImage, width*height );
	img.Save(strFileName);
}



//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::insp_offset()
{
	//
	// Offset
	//
	if( get_image() )
	{
		// ROI
		int sx, ex, sy, ey;
		GetInspectionRegion( &sx, &ex, &sy, &ey );

		//
		start_global_counter();

		float cxGlass, cyGlass, cxMask, cyMask, widthGlass, widthMask;
		bool bSucess = m_pIPModule->insp_offset(	get_image_buffer(), get_width_step(), get_height(), 
											sx, sy, ex, ey,
											200, 255,
											(int)(2300*0.5), (int)(2300*1.5),
											70, 255,
											(int)(7300*0.8), (int)(7300*1.2),
											100,
											&cxGlass, &cyGlass, &widthGlass,
											&cxMask, &cyMask, &widthMask );


		if( bSucess==false )
		{
			end_global_counter("=> insp_offset - find glass pattern(Fail)");
			return;
		}
		end_global_counter("=> insp_offset - find glass pattern(OK)");


		m_listLine[m_indexSelect][0].clear();
		FPoint tempPoint;
		float fRadius = widthGlass/2;
		for( int angle = 0 ; angle<360 ; angle++ )
		{
			tempPoint.x = cxGlass + fRadius*(float)cos(angle*_IP_PI_/180.f);
			tempPoint.y = cyGlass + fRadius*(float)sin(angle*_IP_PI_/180.f);

			m_listLine[m_indexSelect][0].push_back(tempPoint);
		}

		m_listLine[m_indexSelect][1].clear();
		fRadius = widthMask/2;
		for( int angle = 0 ; angle<360 ; angle++ )
		{
			tempPoint.x = cxMask + fRadius*(float)cos(angle*_IP_PI_/180.f);
			tempPoint.y = cyMask + fRadius*(float)sin(angle*_IP_PI_/180.f);

			m_listLine[m_indexSelect][1].push_back(tempPoint);
		}

		m_nListFPoint[m_indexSelect] = 2;
	}

	m_pViewImage->Invalidate(FALSE);
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::insp_offset_n()
{
	if( get_image()==NULL )
	{
		return;
	}
	//
	// Offset
	//

	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	//
	start_global_counter();

	double dDx, dDy;
	bool bSucess = m_pIPModule->insp_offset_multi(	get_image_buffer(), get_width_step(), get_height(), 
											sx, sy, ex, ey,
											200, 255, 50, 50, 0.2f,
											100, 255, 119, 130, 0.2f,
											230, 227, 5,
											&dDx, &dDy );


	if( bSucess==false )
	{
		end_global_counter("=> insp_offset_multi - NG");
		return;
	}
	end_global_counter("=> insp_offset_multi - OK");


	int nResult;
	OFFSET_FMM *pResult = m_pIPModule->get_offset_result(&nResult);

	clear_display_circle();

	for( int i=0 ; i<nResult ; i++ )
	{
		add_display_circle( pResult[i].mask_x, pResult[i].mask_y, 25 );
		add_display_circle( pResult[i].glass_x, pResult[i].glass_y, 15 );
	}

	m_pViewImage->Invalidate(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::insp_focus()
{
	if( get_image() )
	{

		// ROI
		int sx, ex, sy, ey;
		GetInspectionRegion( &sx, &ex, &sy, &ey );


		//
		// 방법 1
		//
// 		start_global_counter();
// 
// 		bool bSucess = m_pIPModule->insp_focus(	get_image_buffer(), get_width_step(), get_height(), 
// 											sx, ex, sy, ey,
// 											40.0 );
// 
// 
// 		if( bSucess==false )
// 		{
// 			end_global_counter("=> insp_focus - Fail");
// 			return;
// 		}
// 		end_global_counter("=> insp_focus - OK");



		//
		// 방법 2
		//
		start_global_counter();
double fValue=0;
		for( int i=0 ; i<1 ; i++ )
		{
			fValue = getFocusValue(	get_image_buffer(), 
				get_width(), 
				get_height(), 
				sx, ex, 
				sy, ey );
		}


		end_global_counter("=> insp_focus - OK");


		CString strMessage;
		strMessage.Format("Focus value = %f", fValue);
		write_message(strMessage);

	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::InspectParticle(int iType)
{
	CString strMsg;

	if( get_image() )
	{
		// ROI
		int sx, ex, sy, ey;
		GetInspectionRegion( &sx, &ex, &sy, &ey );

		if( iType==INSPECT_PARTICLE_GENERAL)
		{
			//
			// 1. Parameter 설정
			//
			// 좌표
			// 좌상, 우상, 좌하, 우하
			double xSrc[4] = { -439.08202, 433.91348, -439.98549, 433.02801 };	// Align hole x좌표( 모터 )
			double ySrc[4] = { -195.69821, -195.74921, -248.20116, -248.24616 };	// Align hole y좌표( 모터 )
			double xDst[4] = { -436.5, 436.5, -436.5, 436.5 };		// Align hole x좌표( 도면 ) align mark의 도면 좌표
			double yDst[4] = { 35.0,	35.0,	-35.0,	-35.0 };	// Align hole y좌표( 도면 )	
			m_pIPModule->SetParam_coordinate( xSrc, ySrc, 4, xDst, yDst, 4 );

			// 검사 정보
			PARAM_FMM_SURFACE_INSPECTIOM param;
			param.uiPitch = 88;			// 구멍간 간격(pixel)
			param.x_resolution = 0.0015;		// x방향 해상도(mm)
			param.y_resolution = 0.0015;		// x방향 해상도(mm)
			param.xRealPitch = 42 * 0.0015;
			param.yRealPitch = 42 * 0.0015;
			param.xRealMergeRange = 0.01;
			param.yRealMergeRange = 0.01;
			param.uiDarkThreshold = 30;	// threshold
			param.uiBrightThreshold = 30;	// threshold
			param.dMinSize = 0.01;
			m_pIPModule->setParam_surfaceInspection( &param );

			// Active region
			DRect activeRegion;				// Active 영역의 도면 좌표(mm 아래는 임의로 입력한 예 )
			// 1st
			activeRegion.sx = -423.9904121;			// 도면 별 실제 검사 영역
			activeRegion.ex = -298.2451119;
			activeRegion.sy = -35.3797841;
			activeRegion.ey = 35.3797841;
			m_pIPModule->AddActiveRegion(activeRegion);

			// 2nd
			activeRegion.sx = -290.3577441;			// 도면 별 실제 검사 영역
			activeRegion.ex = -164.6118439;
			activeRegion.sy = -35.3797841;
			activeRegion.ey = 35.3797841;
			m_pIPModule->AddActiveRegion(activeRegion);

			// 3rd
			activeRegion.sx = -156.7238761;			// 도면 별 실제 검사 영역
			activeRegion.ex = -30.9785759;
			activeRegion.sy = -35.3797841;
			activeRegion.ey = 35.3797841;
			m_pIPModule->AddActiveRegion(activeRegion);

			// 4th
			activeRegion.sx = 35.9175879;			// 도면 별 실제 검사 영역
			activeRegion.ex = 161.6628881;
			activeRegion.sy = -35.3797841;
			activeRegion.ey = 35.3797841;
			m_pIPModule->AddActiveRegion(activeRegion);

			// 5th
			activeRegion.sx = 169.5508559;			// 도면 별 실제 검사 영역
			activeRegion.ex = 295.2961561;
			activeRegion.sy = -35.3797841;
			activeRegion.ey = 35.3797841;
			m_pIPModule->AddActiveRegion(activeRegion);

			// 6th
			activeRegion.sx = 303.1841239;			// 도면 별 실제 검사 영역
			activeRegion.ex = 428.9294241;
			activeRegion.sy = -35.3797841;
			activeRegion.ey = 35.3797841;
			m_pIPModule->AddActiveRegion(activeRegion);


	// 		m_pIPModule->m_bSaveImage = false;

			m_listDefect.clear();

			DEFECT_INFO *pDefectList = new DEFECT_INFO[500];
			int nDefectList = 0;

			//
			// 2. Grab 영상 대상 불량 검사
			//
			m_pIPModule->saveOrigialImage(true);

	// 		for( int i=0 ; i<90 ; i++ )
	// 		{
	// 			CString strFileName;
	// 			strFileName.Format("d:\\images\\AOI\\%d.bmp", i+1 );
	// 			CBhImage img;
	// 			img.Open(strFileName);
	// 
	// 			start_global_counter();
	// 
	// 			bool bSucess = m_pIPModule->inspectParticleDefect(	img.GetImage(), img.GetWidth(), img.GetHeight(), 
	// 																0, i*1000,	//	motor 좌표
	// 																1,			// 검사 방향
	// 																&m_listDefect );
	// 
	// 			end_global_counter("=> InspectParticle - OK");
	// 		}

			start_global_counter();
				bool bSucess = m_pIPModule->inspectParticleDefect(	get_image_buffer(), get_width_step(), get_height(), 
																	-418.00001, -176.99998,	//	motor 좌표
																	-1,			// 검사 방향
	// 																-291.99998, -182.48921,	//	motor 좌표
	// 																-1,			// 검사 방향
	// 																432.99998, -182.48921,	//	motor 좌표
	// 																-1,			// 검사 방향
	// 																434.00001, -236.48916,	//	motor 좌표
	// 																1,			// 검사 방향
	// 																-438.99999, -236.48916,	//	motor 좌표
	// 																1,			// 검사 방향
																	pDefectList,
																	&nDefectList);
			end_global_counter("=> InspectParticle - OK");
		
			//
			// 3. 
			//
	// 		list<DEFECT_INFO> listResultDefect;	// 최종 불량 버퍼
	// 
	// 		m_pIPModule->mergeParticleDefect(&m_listDefect, &listResultDefect);
	// 
	// 		strMsg.Format("Merge defect : %d ea => %d ea\n", m_listDefect.size(), listResultDefect.size() );
	// 		write_message(strMsg);


			delete[] pDefectList;

			//
			//	전체 영상 대상 검사
			//
	// 
	// 		bool bSucess = m_pIPModule->inspectParticleAtFMM(	get_image_buffer(), get_width_step(), get_height(), 
	// 														sx, sy, ex, ey );
	// 
	// 
	// 		if( bSucess==false )
	// 		{
	// 			end_global_counter("=> InspectParticle - Fail");
	// 			return;
	// 		}
	// 
	// 		m_listDefect.clear();
	// 		for( list<DEFECT_INFO>::iterator ite = m_pIPModule->GetDefectList()->begin() ; ite!=m_pIPModule->GetDefectList()->end() ; ite++ )
	// 		{
	// 
	// 			m_listDefect.push_back(*ite);
	// 		}
	// 
	// 		m_pViewImage->Invalidate();
	// 
	// 		end_global_counter("=> InspectParticle - OK");
		}



		//////////////////////////////////////////////////////////////////////////
		//
		//
		if( iType==INSPECT_PARTICLE_SIMULATION_1 )
		{
			//////////////////////////////////////////////////////////////////////////
			// Scan 좌표 추출
			//
			int iPos;

			iPos = m_strFileName.Find("D");

			// Y 좌표 추출
			CString strY = m_strFileName;

			iPos = strY.Find("X=");
			strY.Delete(iPos-2, strY.GetLength());

			iPos = strY.Find("Y=");
			strY.Delete(0, iPos+2);

			double dY = atof(strY);

			// X 좌표 추출
			CString strX = m_strFileName;

			iPos = strX.ReverseFind('[');
			strX.Delete(iPos-1, strX.GetLength());

			iPos = strX.Find("X=");
			strX.Delete(0, iPos+2);

			double dX = atof(strX);

			// scan 방향 추출		dY	-9.2559631349317831e+061	double

			CString strD = m_strFileName;

			iPos = strD.ReverseFind(']');
			strD.Delete(iPos, strD.GetLength());

			iPos = strD.Find("X=");
			strD.Delete(0, iPos+2);

			iPos = strD.Find("][");
			strD.Delete(0, iPos+2);

			int iDerection = atoi(strD);
			//
			//////////////////////////////////////////////////////////////////////////


			//////////////////////////////////////////////////////////////////////////
			// Parameter 설정
			//
			// 1. Align mark 위치 좌표 : 좌상, 우상, 좌하, 우하
			double xSrc[4] = { -438.9863605, 433.9589836, -438.9820255, 433.9550764 };	// Align hole x좌표( 모터 )
			double ySrc[4] = { -160.4029703, -160.4045747, -226.0414429, -226.0448005 };	// Align hole y좌표( 모터 )
			double xDst[4] = { -436.5000000, 436.5000000, -436.5000000, 436.5000000 };		// Align hole x좌표( 도면 ) align mark의 도면 좌표
			double yDst[4] = { 32.8200000,	32.8200000,	-32.8200000,	-32.8200000 };	// Align hole y좌표( 도면 )	
			m_pIPModule->SetParam_coordinate( xSrc, ySrc, 4, xDst, yDst, 4 );

			// 2. 검사 정보
			PARAM_FMM_SURFACE_INSPECTIOM param;
			param.uiPitch = 31;			// 구멍간 간격(pixel)
			param.x_resolution = 0.0015;		// x방향 해상도(mm)
			param.y_resolution = 0.0015;		// x방향 해상도(mm)
			param.xRealPitch = 0.0472423;//42 * 0.0015;
			param.yRealPitch = 0.0472513;//42 * 0.0015;
			param.xRealMergeRange = 0.01;
			param.yRealMergeRange = 0.01;
			param.uiDarkThreshold = 45;	// threshold
			param.uiBrightThreshold = 45;	// threshold
			param.dMinSize = 0.01;
			m_pIPModule->setParam_surfaceInspection( &param );

			// 3. Active region
			DRect activeRegion;				// Active 영역의 도면 좌표(mm 아래는 임의로 입력한 예 )
			// 1st
			activeRegion.sx = -381.2061600;			// 도면 별 실제 검사 영역
			activeRegion.ex = -244.9744900;
			activeRegion.sy = -34.1078800;
			activeRegion.ey = 34.1078800;
			m_pIPModule->AddActiveRegion(activeRegion);

			// 2nd
			activeRegion.sx = -224.6491900;			// 도면 별 실제 검사 영역
			activeRegion.ex = -88.4175200;
			activeRegion.sy = -34.1078800;
			activeRegion.ey = 34.1078800;
			m_pIPModule->AddActiveRegion(activeRegion);

			// 3rd
			activeRegion.sx = -68.0922100;			// 도면 별 실제 검사 영역
			activeRegion.ex = 68.1394600;
			activeRegion.sy = -34.1078800;
			activeRegion.ey = 34.1078800;
			m_pIPModule->AddActiveRegion(activeRegion);

			// 4th
			activeRegion.sx = 88.4647600;			// 도면 별 실제 검사 영역
			activeRegion.ex = 224.6964300;
			activeRegion.sy = -34.1078800;
			activeRegion.ey = 34.1078800;
			m_pIPModule->AddActiveRegion(activeRegion);

			// 5th
			activeRegion.sx = 245.0217400;			// 도면 별 실제 검사 영역
			activeRegion.ex = 381.2534100;
			activeRegion.sy = -34.1078800;
			activeRegion.ey = 34.1078800;
			m_pIPModule->AddActiveRegion(activeRegion);
			//
			//////////////////////////////////////////////////////////////////////////


			//////////////////////////////////////////////////////////////////////////
			// 검사
			//
			// 결과 데이터 저장소
			DEFECT_INFO *pDefectList = new DEFECT_INFO[500];
			int nDefectList = 0;

			// 검사
			start_global_counter();
			m_pIPModule->saveOrigialImage(false);
			m_pIPModule->saveProcessingImage(false);
			m_pIPModule->saveAlignImage(false);
			m_pIPModule->inspectParticleDefect( get_image_buffer(), 
												get_width_step(), get_height(), 
												dX, dY,				//	motor 좌표
												iDerection,			// 검사 방향
												pDefectList,
												&nDefectList);
			end_global_counter("이물검사(1장) : ");

			//
			delete[] pDefectList;
			//
			//////////////////////////////////////////////////////////////////////////
		}
		//
		//////////////////////////////////////////////////////////////////////////



		//////////////////////////////////////////////////////////////////////////
		//
		//
		if( iType==INSPECT_PARTICLE_SIMULATION_N )
		{
			char path[] = "d:\\processing\\input\\*.bmp"; // 해당 경로에서 txt 파일만 찾기. 

			HANDLE hSrch;  
			WIN32_FIND_DATA wfd;  
			BOOL bResult = TRUE;  

			hSrch = FindFirstFile(path, &wfd);

			if(hSrch == INVALID_HANDLE_VALUE)  
			{  
				write_message("no image file");
				bResult = FALSE;
			}  

			while(bResult)  
			{  
				CString strFile;
				strFile.Format("d:\\processing\\input\\%s",wfd.cFileName);
// 				write_message(str);

				CBhImage img;
				img.Open(strFile);


				//////////////////////////////////////////////////////////////////////////
				// Scan 좌표 추출
				//
				int iPos;

				iPos = strFile.Find("D");

				// Y 좌표 추출
				CString strY = strFile;

				iPos = strY.Find("X=");
				strY.Delete(iPos-2, strY.GetLength());

				iPos = strY.Find("Y=");
				strY.Delete(0, iPos+2);

				double dY = atof(strY);

				// X 좌표 추출
				CString strX = strFile;

				iPos = strX.ReverseFind('[');
				strX.Delete(iPos-1, strX.GetLength());

				iPos = strX.Find("X=");
				strX.Delete(0, iPos+2);

				double dX = atof(strX);

				// scan 방향 추출		dY	-9.2559631349317831e+061	double

				CString strD = strFile;

				iPos = strD.ReverseFind(']');
				strD.Delete(iPos, strD.GetLength());

				iPos = strD.Find("X=");
				strD.Delete(0, iPos+2);

				iPos = strD.Find("][");
				strD.Delete(0, iPos+2);

				int iDerection = atoi(strD);
				//
				//////////////////////////////////////////////////////////////////////////


				//////////////////////////////////////////////////////////////////////////
				// Parameter 설정
				//
				// 1. Align mark 위치 좌표 : 좌상, 우상, 좌하, 우하
				double xSrc[4] = { -438.9863605, 433.9589836, -438.9820255, 433.9550764 };	// Align hole x좌표( 모터 )
				double ySrc[4] = { -160.4029703, -160.4045747, -226.0414429, -226.0448005 };	// Align hole y좌표( 모터 )
				double xDst[4] = { -436.5000000, 436.5000000, -436.5000000, 436.5000000 };		// Align hole x좌표( 도면 ) align mark의 도면 좌표
				double yDst[4] = { 32.8200000,	32.8200000,	-32.8200000,	-32.8200000 };	// Align hole y좌표( 도면 )	
				m_pIPModule->SetParam_coordinate( xSrc, ySrc, 4, xDst, yDst, 4 );

				// 2. 검사 정보
				PARAM_FMM_SURFACE_INSPECTIOM param;
				param.uiPitch = 31;			// 구멍간 간격(pixel)
				param.x_resolution = 0.0015;		// x방향 해상도(mm)
				param.y_resolution = 0.0015;		// x방향 해상도(mm)
				param.xRealPitch = 0.0472423;//42 * 0.0015;
				param.yRealPitch = 0.0472513;//42 * 0.0015;
				param.xRealMergeRange = 0.01;
				param.yRealMergeRange = 0.01;
				param.uiDarkThreshold = 45;	// threshold
				param.uiBrightThreshold = 45;	// threshold
				param.dMinSize = 0.01;
				m_pIPModule->setParam_surfaceInspection( &param );

				// 3. Active region
				DRect activeRegion;				// Active 영역의 도면 좌표(mm 아래는 임의로 입력한 예 )
				// 1st
				activeRegion.sx = -381.2061600;			// 도면 별 실제 검사 영역
				activeRegion.ex = -244.9744900;
				activeRegion.sy = -34.1078800;
				activeRegion.ey = 34.1078800;
				m_pIPModule->AddActiveRegion(activeRegion);

				// 2nd
				activeRegion.sx = -224.6491900;			// 도면 별 실제 검사 영역
				activeRegion.ex = -88.4175200;
				activeRegion.sy = -34.1078800;
				activeRegion.ey = 34.1078800;
				m_pIPModule->AddActiveRegion(activeRegion);

				// 3rd
				activeRegion.sx = -68.0922100;			// 도면 별 실제 검사 영역
				activeRegion.ex = 68.1394600;
				activeRegion.sy = -34.1078800;
				activeRegion.ey = 34.1078800;
				m_pIPModule->AddActiveRegion(activeRegion);

				// 4th
				activeRegion.sx = 88.4647600;			// 도면 별 실제 검사 영역
				activeRegion.ex = 224.6964300;
				activeRegion.sy = -34.1078800;
				activeRegion.ey = 34.1078800;
				m_pIPModule->AddActiveRegion(activeRegion);

				// 5th
				activeRegion.sx = 245.0217400;			// 도면 별 실제 검사 영역
				activeRegion.ex = 381.2534100;
				activeRegion.sy = -34.1078800;
				activeRegion.ey = 34.1078800;
				m_pIPModule->AddActiveRegion(activeRegion);
				//
				//////////////////////////////////////////////////////////////////////////


				//////////////////////////////////////////////////////////////////////////
				// 검사
				//
				// 결과 데이터 저장소
				DEFECT_INFO *pDefectList = new DEFECT_INFO[500];
				int nDefectList = 0;

				// 검사
				m_pIPModule->saveOrigialImage(true);
				m_pIPModule->saveProcessingImage(true);
				m_pIPModule->inspectParticleDefect( img.GetImage(), 
					img.GetWidth(), img.GetHeight(), 
					dX, dY,				//	motor 좌표
					iDerection,			// 검사 방향
					pDefectList,
					&nDefectList);

				//
				delete[] pDefectList;
				//
				//////////////////////////////////////////////////////////////////////////




				bResult = FindNextFile(hSrch, &wfd);  
			}  
			FindClose(hSrch); 
		}
		//
		//////////////////////////////////////////////////////////////////////////
	}
}


//////////////////////////////////////////////////////////////////////////
//
//	PPA를 측정한다.
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::insp_ppa(int NByN)
{
	if( get_image()==NULL )
	{
		return;
	}

	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	//
	MASK_INFO maskInfo;
	// 45도 구멍 - E61 - G mask
	// 	maskInfo.dRealPitchX = 55.545;
	// 	maskInfo.dRealPitchY = 55.545;
	// 	maskInfo.dRealHoleWidth = 43.397;
	// 	maskInfo.dRealHoleHeight = 31.798;
	// 	maskInfo.dResolution = 0.345;
	// 	maskInfo.eHoleShape = HOLE_SHAPE_ELLIPSE_45;
	// 	maskInfo.eHoleArrange = HOLE_ARRANGE_RECTANGLE;
	// 	maskInfo.eHoleMeasureMethod = MEASURE_METHOD_EDGE;

	// 45도 구멍
	// 	maskInfo.dRealPitchX = 100.8;
	// 	maskInfo.dRealPitchY = 100.8;
	// 	maskInfo.dRealHoleWidth = 65.71;
	// 	maskInfo.dRealHoleHeight = 65.71;
	// 	maskInfo.dResolution = 0.345;
	// 	maskInfo.eHoleShape = HOLE_SHAPE_RECTANGLE_45;
	// 	maskInfo.eHoleArrange = HOLE_ARRANGE_CROSS;
	// 	maskInfo.eHoleMeasureMethod = MEASURE_METHOD_PAIR;

	// 일반 구멍
	maskInfo.dRealPitchX = 112.125;
	maskInfo.dRealPitchY = 112.125;
	maskInfo.dRealHoleWidth		= 52.44;
	maskInfo.dRealHoleHeight	= 52.44;
// 	maskInfo.dRealHoleWidth		= 56.44;	// E5
// 	maskInfo.dRealHoleHeight	= 56.44;	// E5
	maskInfo.dResolution = 0.345;
	maskInfo.eHoleShape = HOLE_SHAPE_DIAMOND;
	maskInfo.eHoleArrange = HOLE_ARRANGE_CROSS;
	maskInfo.eHoleMeasureMethod = MEASURE_METHOD_PAIR;


	// 일반 구멍
// 	maskInfo.dRealPitchX = 47.24227;
// 	maskInfo.dRealPitchY = 47.24227;
// 	maskInfo.dRealHoleWidth		= 32;
// 	maskInfo.dRealHoleHeight	= 32;
// 	maskInfo.dResolution = 0.342;
// 	maskInfo.eHoleShape = HOLE_SHAPE_CIRCLE;
// 	maskInfo.eHoleArrange = HOLE_ARRANGE_RECTANGLE;
// 	maskInfo.eHoleMeasureMethod = MEASURE_METHOD_PAIR;

	//
	PPA_INFO ppaInfo;

	//
	start_global_counter();

	m_pIPModule->displayImage((bool)m_bDisplayProcessingImage);

	// 	bool bSucess = m_pIPModule->insp_ppa(	get_image_buffer(), get_width_step(), get_height(), 
	// 											sx+1, sy+1, ex-1, ey-1,
	// 											maskInfo,
	// 											3,
	// 											&ppaInfo );


	// tracking 기능은 일단 halcon에만 구현
	CImageProcess_H *tmpClass = new CImageProcess_H;
	tmpClass->displayImage((bool)m_bDisplayProcessingImage);

	CString filePath = "d:\\aaa\\";
	tmpClass->setFilePath(filePath.GetBuffer(), filePath.GetLength());

	CString fileName = "kkk";
	tmpClass->setFileName(fileName.GetBuffer(), fileName.GetLength());

	bool bSucess = tmpClass->insp_ppa_tracking(	get_image_buffer(), get_width_step(), get_height(), 
												sx+1, sy+1, ex-1, ey-1,
												maskInfo,
												NByN,
												&ppaInfo,
												0,
												0);


	delete[] tmpClass;

	if( bSucess==false )
	{
		end_global_counter("=> insp_ppa - Fail");
		return;
	}
	end_global_counter("=> insp_ppa - OK");


	// display
	clear_display_circle();

	m_pViewImage->Invalidate(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//
//	PPA를 측정한다.
//	
//		- 조건 : 전체 구멍 검출 후 몇번째 구멍을 사용할 지 선택할 수 있다.
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::insp_ppa_pos(int NByN)
{
	if( get_image()==NULL )
	{
		return;
	}

	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	//
	MASK_INFO maskInfo;
	// 45도 구멍 - E61 - G mask
	// 	maskInfo.dRealPitchX = 55.545;
	// 	maskInfo.dRealPitchY = 55.545;
	// 	maskInfo.dRealHoleWidth = 43.397;
	// 	maskInfo.dRealHoleHeight = 31.798;
	// 	maskInfo.dResolution = 0.345;
	// 	maskInfo.eHoleShape = HOLE_SHAPE_ELLIPSE_45;
	// 	maskInfo.eHoleArrange = HOLE_ARRANGE_RECTANGLE;
	// 	maskInfo.eHoleMeasureMethod = MEASURE_METHOD_EDGE;

	// 45도 구멍
	// 	maskInfo.dRealPitchX = 100.8;
	// 	maskInfo.dRealPitchY = 100.8;
	// 	maskInfo.dRealHoleWidth = 65.71;
	// 	maskInfo.dRealHoleHeight = 65.71;
	// 	maskInfo.dResolution = 0.345;
	// 	maskInfo.eHoleShape = HOLE_SHAPE_RECTANGLE_45;
	// 	maskInfo.eHoleArrange = HOLE_ARRANGE_CROSS;
	// 	maskInfo.eHoleMeasureMethod = MEASURE_METHOD_PAIR;

	// 일반 구멍
	// 	maskInfo.dRealPitchX = 112.125;
	// 	maskInfo.dRealPitchY = 112.125;
	// 	maskInfo.dRealHoleWidth		= 52.44;
	// 	maskInfo.dRealHoleHeight	= 52.44;
	// 	maskInfo.dRealHoleWidth		= 56.44;	// E5
	// 	maskInfo.dRealHoleHeight	= 56.44;	// E5
	// 	maskInfo.dResolution = 0.345;
	// 	maskInfo.eHoleShape = HOLE_SHAPE_DIAMOND;
	// 	maskInfo.eHoleArrange = HOLE_ARRANGE_CROSS;
	// 	maskInfo.eHoleMeasureMethod = MEASURE_METHOD_PAIR;


	// 일반 구멍
	maskInfo.dRealPitchX = 47.24227;
	maskInfo.dRealPitchY = 47.24227;
	maskInfo.dRealHoleWidth		= 32;
	maskInfo.dRealHoleHeight	= 32;
	maskInfo.dResolution = 0.342;
	maskInfo.eHoleShape = HOLE_SHAPE_CIRCLE;
	maskInfo.eHoleArrange = HOLE_ARRANGE_RECTANGLE;
	maskInfo.eHoleMeasureMethod = MEASURE_METHOD_PAIR;

	//
	PPA_INFO ppaInfo;

	//
	start_global_counter();

	m_pIPModule->displayImage((bool)m_bDisplayProcessingImage);


	CImageProcess_H *tmpClass = new CImageProcess_H;
	tmpClass->displayImage((bool)m_bDisplayProcessingImage);

	bool bSucess = tmpClass->insp_ppa_pos( get_image_buffer(), get_width_step(), get_height(),
		sx+1, sy+1, ex-1, ey-1,
		maskInfo, 
		BHIP_POS_LT, 
		// 		BHIP_POS_RT, 
		// 		BHIP_POS_LB, 
		// 		BHIP_POS_RB, 
		1, 1, 
		1, 
		&ppaInfo );

	delete[] tmpClass;

	if( bSucess==false )
	{
		end_global_counter("=> insp_ppa_pos - Fail");
		return;
	}
	end_global_counter("=> insp_ppa_pos - OK");


	// display
	clear_display_circle();

	m_pViewImage->Invalidate(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
/*
void CImageTesterDoc::insp_ppa_n()
{
	//
	// PPA
	//
	if( get_image()==NULL )
	{
		return;
	}

	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	//
	MASK_INFO maskInfo;
	// 45도 구멍 - E61 - G mask
// 	maskInfo.dRealPitchX = 55.545;
// 	maskInfo.dRealPitchY = 55.545;
// 	maskInfo.dRealHoleWidth = 43.397;
// 	maskInfo.dRealHoleHeight = 31.798;
// 	maskInfo.dResolution = 0.345;
// 	maskInfo.eHoleShape = HOLE_SHAPE_ELLIPSE_45;
// 	maskInfo.eHoleArrange = HOLE_ARRANGE_RECTANGLE;
// 	maskInfo.eHoleMeasureMethod = MEASURE_METHOD_EDGE;

	// 45도 구멍
// 	maskInfo.dRealPitchX = 100.8;
// 	maskInfo.dRealPitchY = 100.8;
// 	maskInfo.dRealHoleWidth = 65.71;
// 	maskInfo.dRealHoleHeight = 65.71;
// 	maskInfo.dResolution = 0.345;
// 	maskInfo.eHoleShape = HOLE_SHAPE_RECTANGLE_45;
// 	maskInfo.eHoleArrange = HOLE_ARRANGE_CROSS;
// 	maskInfo.eHoleMeasureMethod = MEASURE_METHOD_PAIR;

	// 일반 구멍
// 	maskInfo.dRealPitchX = 112.125;
// 	maskInfo.dRealPitchY = 112.125;
// 	maskInfo.dRealHoleWidth		= 52.44;
// 	maskInfo.dRealHoleHeight	= 52.44;
// 	maskInfo.dRealHoleWidth		= 56.44;	// E5
// 	maskInfo.dRealHoleHeight	= 56.44;	// E5
// 	maskInfo.dResolution = 0.345;
// 	maskInfo.eHoleShape = HOLE_SHAPE_DIAMOND;
// 	maskInfo.eHoleArrange = HOLE_ARRANGE_CROSS;
// 	maskInfo.eHoleMeasureMethod = MEASURE_METHOD_PAIR;


	// 일반 구멍
	maskInfo.dRealPitchX = 47.24227;
	maskInfo.dRealPitchY = 47.24227;
	maskInfo.dRealHoleWidth		= 32;
	maskInfo.dRealHoleHeight	= 32;
	maskInfo.dResolution = 0.342;
	maskInfo.eHoleShape = HOLE_SHAPE_CIRCLE;
	maskInfo.eHoleArrange = HOLE_ARRANGE_RECTANGLE;
	maskInfo.eHoleMeasureMethod = MEASURE_METHOD_PAIR;

	//
	PPA_INFO ppaInfo;

	//
	start_global_counter();

	m_pIPModule->displayImage((bool)m_bDisplayProcessingImage);

// 	bool bSucess = m_pIPModule->insp_ppa(	get_image_buffer(), get_width_step(), get_height(), 
// 											sx+1, sy+1, ex-1, ey-1,
// 											maskInfo,
// 											3,
// 											&ppaInfo );

	CImageProcess_H *tmpClass = new CImageProcess_H;
	tmpClass->displayImage((bool)m_bDisplayProcessingImage);
// 	bool bSucess = tmpClass->insp_ppa_tracking(	get_image_buffer(), get_width_step(), get_height(), 
// 												sx+1, sy+1, ex-1, ey-1,
// 												maskInfo,
// 												3,
// 												&ppaInfo,
// 												10,
// 												10);

	bool bSucess = tmpClass->insp_ppa_pos( get_image_buffer(), get_width_step(), get_height(),
		sx+1, sy+1, ex-1, ey-1,
		maskInfo, 
		BHIP_POS_LT, 
// 		BHIP_POS_RT, 
// 		BHIP_POS_LB, 
// 		BHIP_POS_RB, 
		1, 1, 
		1, 
		&ppaInfo );

	delete[] tmpClass;

	if( bSucess==false )
	{
		end_global_counter("=> insp_ppa_multi - Fail");
		return;
	}
	end_global_counter("=> insp_ppa_multi - OK");

// 	int nResult;
// 	FPoint *pResult = m_pIPModule->get_ppa_result(&nResult);


	// display
	clear_display_circle();

// 	for( int i=0 ; i<nResult ; i++ )
// 	{
// 		add_display_circle( pResult[i].x, pResult[i].y, 25 );
// 	}
	
	m_pViewImage->Invalidate(FALSE);
}*/


//////////////////////////////////////////////////////////////////////////
//
//	사용할 DLL 연결
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::set_libray( LIBRARY_TYPE type )
{
	switch( type )
	{
	case LIB_C:
		if( m_pIPModule )
		{
			delete[] m_pIPModule;
		}
		m_pIPModule = new CImageProcess_C;
		break;

	case LIB_HALCON:
		#ifdef USE_HALCON
				if( m_pIPModule )
				{
					delete[] m_pIPModule;
				}
				m_pIPModule = new CImageProcess_H;
				break;
		#else
				AfxMessageBox("No halcon @ win64");
		#endif

	case LIB_OPENCV:
		#ifdef USE_OPENCV
			if( m_pIPModule )
			{
				delete[] m_pIPModule;
			}
			m_pIPModule = new CImageProcess_Open;
		#else
			AfxMessageBox("No opencv @ win32");
		#endif

		break;

	default:
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::threshold( UINT method, int nLowThreshold, int nHighThreshold )
{
	start_global_counter();

	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		end_global_counter("threshold fail");
		return;
	}



// 	// 생성
//  	get_next_image()->Create( width, height, 8, 0 );
// 
// 	// 초기화
// 	memset( get_next_image_buffer(), 0, width*height );

	//
	PrepareNextImage();


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );


	//
	switch( method )
	{
		case MSG_THRESHOLD_MANUAL:
			m_pIPModule->threshold( get_image_buffer(), get_next_image_buffer(), 
									width, height, 
									sx, ex, sy, ey, 
									0, nLowThreshold );
// 									nLowThreshold, nHighThreshold );

			get_next_image()->Invalidate();

// 			SetDisplayModeFlip();

			break;

		case MSG_THRESHOLD_OTSU:
			m_pIPModule->threshold( get_image_buffer(), get_next_image_buffer(), 
									width, height, 
									sx, ex, sy, ey, 
									OTSU_METHOD );

			get_next_image()->Invalidate();

// 			SetDisplayModeFlip();

			break;

		default:
			break;
	}

	end_global_counter("threshold");

}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::smooth( UINT method )
{
	start_global_counter();


	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		return;
	}


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	BYTE *pRstImg = new BYTE[width*height];
	memset( pRstImg, 0, width*height );


	//
	switch( method )
	{
		case MSG_SMOOTH_BLUR:
			m_pIPModule->blur( get_image_buffer(), pRstImg, 
									width, height, 
									sx, ex, sy, ey, 
									CSize(15,15) );
			break;

		case MSG_SMOOTH_BOX:
			m_pIPModule->box_filter( get_image_buffer(), pRstImg, 
				width, height, 
				sx, ex, sy, ey, 
				CSize(15,15) );
			break;

		case MSG_SMOOTH_MEDIAN:
			m_pIPModule->median_filter( get_image_buffer(), pRstImg, 
				width, height, 
				sx, ex, sy, ey, 
				CSize(15,15) );
			break;

		case MSG_SMOOTH_GAUSSIAN:
			m_pIPModule->gaussian_filter( get_image_buffer(), pRstImg, 
				width, height, 
				sx, ex, sy, ey, 
				CSize(15,15) );
			break;

		default:
			break;
	}


	memcpy( get_image_buffer(), pRstImg, width*height );
	get_image()->Invalidate();

	delete[] pRstImg;

	m_pViewImage->Invalidate(FALSE);


	end_global_counter("[Smooth]");
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::derivative_filter( UINT method )
{
	start_global_counter();


	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		return;
	}


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	BYTE *pRstImg = new BYTE[width*height];
	memset( pRstImg, 0, width*height );

	//
	switch( method )
	{
		case MSG_DERIVATIVE_SOBEL:
			m_pIPModule->sobel(	get_image_buffer(), pRstImg, 
								width, height, 
								sx, ex, sy, ey );
			break;

		case MSG_DERIVATIVE_SCHARR:
			m_pIPModule->scharr( get_image_buffer(), pRstImg, 
										width, height, 
										sx, ex, sy, ey );
			break;

		case MSG_DERIVATIVE_LAPLACE:
			m_pIPModule->laplacian( get_image_buffer(), pRstImg, 
									width, height, 
									sx, ex, sy, ey );
			break;

		case MSG_DERIVATIVE_PREWITT:
			m_pIPModule->prewitt(	get_image_buffer(), pRstImg, 
									width, height, 
									sx, ex, sy, ey );
			break;
	}


	memcpy( get_image_buffer(), pRstImg, width*height );
	get_image()->Invalidate();

	delete[] pRstImg;

	m_pViewImage->Invalidate(FALSE);


	end_global_counter("[derivative filter]");
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::edge( UINT method, int param )
{
	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		return;
	}


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	BYTE *pRstImg = new BYTE[width*height];
	memset( pRstImg, 0, width*height );


	//
	switch( method )
	{
		case MSG_EDGE_CANNY:
			m_pIPModule->edge(	get_image_buffer(), pRstImg, 
								width, height, 
								sx, ex, sy, ey,
								0,
								param);
			break;

	}


	memcpy( get_image_buffer(), pRstImg, width*height );
	get_image()->Invalidate();

	delete[] pRstImg;

	m_pViewImage->Invalidate(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::PerformMorphology( UINT method, int param )
{
	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		return;
	}


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	BYTE *pRstImg = new BYTE[width*height];
	memset( pRstImg, 0, width*height );


	//
	switch( method )
	{
		case MSG_MORPHOLOGY_ERODE:
			m_pIPModule->erode(	get_image_buffer(), pRstImg, 
								width, height, 
								sx, ex, sy, ey,
								param);
			break;

		case MSG_MORPHOLOGY_DILATION:
			m_pIPModule->dilation(	get_image_buffer(), pRstImg, 
									width, height, 
									sx, ex, sy, ey,
									param);
			break;

		default:
			AfxMessageBox("Morphology error");
			break;
	}


	memcpy( get_image_buffer(), pRstImg, width*height );
	get_image()->Invalidate();

	delete[] pRstImg;

	m_pViewImage->Invalidate(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::PerformPitchCompare( UINT uiPitch )
{
	start_global_counter();

	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		end_global_counter("pitch_compare fail");
		return;
	}


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

// 	BYTE *pRstImg = new BYTE[width*height];
// 	memset( pRstImg, 0, width*height );
	//
	// 결과 이미지
	//
	// 생성
	get_next_image()->Create( width, height, 8, 0 );

	// 초기화
	memset( get_next_image_buffer(), 0, width*height );


	//
	m_pIPModule->pitch_compare(	get_image_buffer(), get_next_image_buffer(), 
						width, height, 
						sx, ex, sy, ey,
						uiPitch);


// 	memcpy( get_image_buffer(), pRstImg, width*height );
	get_next_image()->Invalidate();


	m_pViewImage->Invalidate(FALSE);

	end_global_counter("pitch_compare");
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
void CImageTesterDoc::PerformLabeling()
{
	start_global_counter();

	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		end_global_counter("PerformLabeling fail");
		return;
	}


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );


	m_objLabel.PerformLabeling(	get_next_image_buffer(), 
								get_image_buffer(), 
								CSize(get_width_step(), get_height()), 
								1, 
								FALSE, 
								99999 );

	int nBlob = m_objLabel.GetBlobNum();

	CString strMsg;
	strMsg.Format("defect number = %d", nBlob);
	write_message(strMsg);

	end_global_counter("PerformLabeling");
	
	m_pViewImage->Invalidate(FALSE);
}


void CImageTesterDoc::OnGaussian3x3()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	start_global_counter();

	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		end_global_counter("OnGaussian3x3 > fail");
		return;
	}


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	//
	PrepareNextImage();


	//
	m_pIPModule->gaussian_filter(	get_image_buffer(), get_next_image_buffer(), 
									width, height, 
									sx, ex, sy, ey,
									CSize(3,3));


	get_next_image()->Invalidate();

	end_global_counter("OnGaussian3x3 > end");

	SetDisplayModeFlip();
}


void CImageTesterDoc::OnGaussian5x5()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	start_global_counter();

	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		end_global_counter("OnGaussian5x5 > fail");
		return;
	}


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	//
	PrepareNextImage();


	//
	m_pIPModule->gaussian_filter(	get_image_buffer(), get_next_image_buffer(), 
		width, height, 
		sx, ex, sy, ey,
		CSize(5,5));


	get_next_image()->Invalidate();

	end_global_counter("OnGaussian5x5 > end");

	SetDisplayModeFlip();
}


void CImageTesterDoc::OnGaussian7x7()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	start_global_counter();

	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		end_global_counter("OnGaussian7x7 > fail");
		return;
	}


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	//
	PrepareNextImage();


	//
	m_pIPModule->gaussian_filter(	get_image_buffer(), get_next_image_buffer(), 
		width, height, 
		sx, ex, sy, ey,
		CSize(7,7));


	get_next_image()->Invalidate();

	end_global_counter("OnGaussian7x7 > end");

	SetDisplayModeFlip();
}


void CImageTesterDoc::OnBlur3x3()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	start_global_counter();

	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		end_global_counter("OnBlur3x3 > fail");
		return;
	}


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	//
	PrepareNextImage();


	//
	m_pIPModule->blur(	get_image_buffer(), get_next_image_buffer(), 
		width, height, 
		sx, ex, sy, ey,
		CSize(7,7));


	get_next_image()->Invalidate();

	end_global_counter("OnBlur3x3 > end");

	SetDisplayModeFlip();
}


void CImageTesterDoc::OnBox3x3()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	start_global_counter();

	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		end_global_counter("OnBox3x3 > fail");
		return;
	}


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	//
	PrepareNextImage();


	//
	m_pIPModule->box_filter(	get_image_buffer(), get_next_image_buffer(), 
		width, height, 
		sx, ex, sy, ey,
		CSize(3,3));


	get_next_image()->Invalidate();

	end_global_counter("OnBox3x3 > end");

	SetDisplayModeFlip();
}


void CImageTesterDoc::OnMedian3x3()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	start_global_counter();

	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		end_global_counter("OnMedian3x3 > fail");
		return;
	}


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	//
	PrepareNextImage();


	//
	m_pIPModule->median_filter(	get_image_buffer(), get_next_image_buffer(), 
		width, height, 
		sx, ex, sy, ey,
		CSize(3,3));


	get_next_image()->Invalidate();

	end_global_counter("OnMedian3x3 > end");

	SetDisplayModeFlip();
}


void CImageTesterDoc::OnFilterSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	start_global_counter();

	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		end_global_counter("OnFilterSobel > fail");
		return;
	}


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );


	//
	PrepareNextImage();


	//
	m_pIPModule->sobel(	get_image_buffer(), get_next_image_buffer(), 
		width, height, 
		sx, ex, sy, ey);

	get_next_image()->Invalidate();

	end_global_counter("OnFilterSobel > end");

	//
	SetDisplayModeFlip();
}


void CImageTesterDoc::OnFilterLaplacian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	start_global_counter();

	int width = get_width_step();
	int height = get_height();


	if( width<0 || height<0 )
	{
		end_global_counter("OnFilterSobel > fail");
		return;
	}


	// ROI
	int sx, ex, sy, ey;
	GetInspectionRegion( &sx, &ex, &sy, &ey );

	//
	PrepareNextImage();


	//
	m_pIPModule->laplacian(	get_image_buffer(), get_next_image_buffer(), 
		width, height, 
		sx, ex, sy, ey);


	get_next_image()->Invalidate();

	end_global_counter("OnFilterSobel > end");

	SetDisplayModeFlip();
}


BOOL CImageTesterDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	get_image()->Save("d:\\test.bmp");

// 	return CDocument::OnSaveDocument(lpszPathName);
	return TRUE;
}
