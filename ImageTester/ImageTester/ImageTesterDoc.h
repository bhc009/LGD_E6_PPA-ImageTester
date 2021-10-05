
// ImageTesterDoc.h : CImageTesterDoc Ŭ������ �������̽�
//


#pragma once

#include "..\common\BhImage.h"
#include "..\common\ImageProcess.h"
#include "..\common\Labeling.h"
// #include "..\ImageProcess_C\ImageProcess_C.h"

#include "ImageProcess_C.h"
#include "ImageProcess_M.h"
#include "SimpleDll.h"

// extern "C" double PASCAL IMPORT getFocusValue( BYTE* pImage, int width, int height, int sx, int ex, int sy, int ey );
// extern "C" double __declspec(dllimport) getFocusValue( BYTE* pImage, int width, int height, int sx, int ex, int sy, int ey );

// library loading
#ifdef USE_HALCON
	#include "ImageProcess_h.h"
	#pragma comment(lib, "imageprocess_h.lib")
#endif

#ifdef USE_OPENCV
	#include "ImageProcess_Open.h"
	#pragma comment(lib, "ImageProcess_Open.lib")
#endif

#include <list>
#include <vector>
using namespace std;

#include "define.h"
#include "common/dxf/Dxf.h"

#include "atf_lib_exp.h"

#define DOC_DEFAULT	-1
#define FILE_PATH_SHAPE_MODEL	"D:\\Shape model.csv"
#define FILE_PATH_NCC_MODEL		"D:\\a\\NCC model.csv"

class CImageViewer;
class CCommandView;
class CSubImageView;
class CMessageView;
class CImageTesterDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CImageTesterDoc();
	DECLARE_DYNCREATE(CImageTesterDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

	// Viewers
	CImageViewer *m_pViewImage;
	CCommandView *m_pViewCommand;
	CSubImageView *m_pViewSubImage;
	CMessageView *m_pViewMessage;

	CString m_strFileName;


	//////////////////////////////////////////////////////////////////////////
	// Images(main)
protected:

	IT_IMAGE m_pImage[2];		// image

	SELECT_IMAGE m_indexSelect;	// ���� �����ִ� ���� ���� �ε���
	
	BOOL m_bRoi;		// ROI ���� ����
	CRect m_rtROI[2];	// ROI
	
	BOOL m_bDisplayProcessingImage;

	list<CPoint> m_listPoint[2];

 	CLabeling m_objLabel;
	list<DEFECT_INFO> m_listDefect;

public:
	//
	void SetDisplayMode( SELECT_IMAGE index );
	void SetDisplayModeFlip();

	//
	IT_IMAGE *get_image();
	BYTE *get_image_buffer();
	int get_width();
	int get_width_step();
	int get_height();

	//
	int get_next_image_index();
	BYTE *get_next_image_buffer();
	IT_IMAGE *get_next_image();
	BOOL PrepareNextImage();
// 	IplImage *Get_NextIplImage();

	//
	BOOL valid_Roi( BOOL bValid );	// ROI ���� ���� ����
	BOOL SetROI( CRect roi );	//  
	BOOL GetROI( CRect *roi );	// 
	BOOL displayProcessingImage( BOOL bValid );	// ROI ���� ���� ����
	BOOL GetInspectionRegion( int *sx, int *ex, int *sy, int *ey );
	void DeleteROI();
	int GetPointNum();
	void AddPoint( CPoint pt );
	BOOL GetPoint( CPoint *pt );

	//
	list<DEFECT_INFO>* GetDefect() 
	{ 
		return &m_listDefect; 
	};
		

	// data
protected:
	int m_nListFPoint[2];
	list<FPoint> m_listLine[2][999];
	list<CIRCLED> m_listCircle[2];

	vector<stResutltData> m_listResultData;	// ��� ������

public:
	int Get_line_num() { return m_nListFPoint[m_indexSelect]; };
	list<FPoint> * Get_line() { return m_listLine[m_indexSelect]; };

	int get_display_circle_num(); 
	void add_display_circle(double cx, double cy, double radius);
	void clear_display_circle();
	list<CIRCLED>* get_display_circle();

	void add_result_data( stResutltData resultData );
	void clear_result_data();
	void check_result_data();


	//////////////////////////////////////////////////////////////////////////
	// sub-image list
protected:
	list<IT_IMAGE*> m_listImage;	// Image
	int m_iSelectedImage;			// ���õ� �̹����� ���� �ε���

public:
	IT_IMAGE* ImageList_GetImage(int n);
	IT_IMAGE* ImageList_GetSelectedImage();
	int ImageList_GetNum();
	void ImageList_Push();		// list ����
	void ImageList_Pop();		// list ���� 
	void ImageList_Delete();		// list ���� ���� 
	BOOL ImageList_GetSelectedIndex(int *index);
	void ImageList_Select(int i);


	//////////////////////////////////////////////////////////////////////////
	//
protected:
	CImageProcess *m_pIPModule;
// 	CImageProcess_C clsIP;

public:
	void set_libray( LIBRARY_TYPE type );


public:
	void threshold( UINT method, int nLowThreshold, int nHighThreshold );
	void smooth( UINT method );
	void derivative_filter( UINT method );
	void edge( UINT method, int param );
	void PerformMorphology( UINT method, int param );
	void PerformPitchCompare( UINT uiPitch );
	void PerformLabeling();


public:
	//////////////////////////////////////////////////////////////////////////
	//
	void create_shape_model();
	void find_shape_model();
	void save_shape_model();
	void load_shape_model();


	//////////////////////////////////////////////////////////////////////////
	//
	void create_ncc_model();
	void find_ncc_model();
	void save_ncc_model();
	void load_ncc_model();


	//////////////////////////////////////////////////////////////////////////
	// 
	void insp_ppa(int NByN);
	void insp_ppa_pos(int NByN);
	void insp_offset();
	void insp_offset_n();
	void insp_focus();
	void InspectParticle(int iType);

	//////////////////////////////////////////////////////////////////////////
	// align
	void create_main_mark();
	void create_sub_mark();
	void find_main_mark(int index);
	void find_sub_mark(int index);
	void calc_align();


	//////////////////////////////////////////////////////////////////////////
	//
protected:
	// ���� �ð� üũ��
	LARGE_INTEGER m_globalStartCounter, m_globalEndCounter, m_counter_Freq;
	LARGE_INTEGER m_localStartCounter, m_localEndCounter;
	void start_global_counter();
	void end_global_counter(CString strMsg);
	void start_local_counter();
	void end_local_counter(CString strMsg);


	//////////////////////////////////////////////////////////////////////////
	//
	//
	void Save_image( BYTE *pImage, int width, int height, CString strFileName );
	void write_message(CString strMsg);

public:
	void Test(void);
	void Test2(void);

// �������Դϴ�.
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CImageTesterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnGaussian3x3();
	afx_msg void OnGaussian5x5();
	afx_msg void OnGaussian7x7();
	afx_msg void OnBlur3x3();
	afx_msg void OnBox3x3();
	afx_msg void OnMedian3x3();
	afx_msg void OnFilterSobel();
	afx_msg void OnFilterLaplacian();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};
