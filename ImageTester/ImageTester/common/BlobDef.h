
#ifndef BLOBDEF_H
#define BLOBDEF_H

//Preprocessing
#define _PREPROCESSING_NONE_		    0
#define _PREPROCESSING_HORIZONTAL_		1
#define _PREPROCESSING_VERTICAL_		2
#define _PREPROCESSING_ALL_				3


//Labeling(Blob) 
#define	LIMIT_BLOBS		       40000

#define	SEEDNUM			       1000000

#define	BACKGROUND			   0
#define	MIDDLE				   128
#define	FOREGROUND			   255

#define	UNDONE			       0
#define	DONE			       1
#define	FALSEDONE		       2

#define	BLACKDEFECT			   125
#define	WHITEDEFECT		       250


//////////////////////////////////////////////////////////////////////////
// Algorithm Type
//////////////////////////////////////////////////////////////////////////
#define	STD_ALG_TYPE			1
#define	EDGE_ALG_TYPE			2
#define	CMA_ALG_TYPE			3
#define	MURA1_ALG_TYPE			4
#define	MURA2_ALG_TYPE			5

//////////////////////////////////////////////////////////////////////////
// Defect Basic Type
//////////////////////////////////////////////////////////////////////////
#define	DOT_DEFECT				0x0001
#define	SSDOT_DEFECT			0x0002
#define	THREAD_DEFECT			0x0004
#define	MURA_DEFECT				0x0008

//////////////////////////////////////////////////////////////////////////
// Defect Extend Type
//////////////////////////////////////////////////////////////////////////
#define	CONVEX_TYPE				0x0001
#define	CONCAVE_TYPE			0x0002
#define	MERGE_TYPE			    0x0004
#define	ALG_MERGE			    0x0008  //ALGORITHM°£ MERGE


// Labeled Blob
typedef struct tagPixel
{
	unsigned int x;
	unsigned int y;

	BYTE		 gray;
} PIXEL;



typedef struct tagLabels
{
	int          nAlgType;
	BYTE         color;
	unsigned int nIndex;

	unsigned int nType;
	unsigned int nExtType;
	int          nResultType;

	bool         overlaped;
	bool         bIsDefect;

	unsigned int left;		    // unit: pixel
	unsigned int top;		    // unit: pixel
	unsigned int right;		    // unit: pixel
	unsigned int bottom;		// unit: pixel

	unsigned int cx;
	unsigned int cy;

	double dCx;
	double dCy;

	double		 dCenx;		    // unit: mm
	double		 dCeny;		    // unit: mm

    unsigned int nArea;         // blob count;
    unsigned int nPeak;         // unit: gray;

    unsigned int nDx;           // unit: pixel; (right - left + 1)
    unsigned int nDy;           // unit: pixel; (bottom - top + 1)

 	double       dSize;		    // unit: um
	double       dDir;		    // unit: radian
	double       dPerimeter;	// unit: um

	double       dMajor;		// unit: um
	double       dMinor;		// unit: um

	double       dMean;         // unit: gray (target mean gray)
	double       dBgMean;       // unit: gray (mean of the background surrounding the corresponding blob)
	double       dGrayDiff;	    // unit: gray

	double       dCompact;	    // unit: none
	double       dThickness;	// unit: none
	double       dCurvature;	// unit: none
	double       dArea;	        // unit: um2
} LABELING;



#endif