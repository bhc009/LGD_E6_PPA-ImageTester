

#pragma once

#define  MSG_TEMPLATE_MATCHING (WM_USER)
#define  MSG_CREATE_TEMPLATE (WM_USER+1)
#define  MSG_FIND_TEMPLATE (WM_USER+2)
#define  MSG_ALIGN (WM_USER+3)
#define  MSG_PPA_INSPECTION (WM_USER+4)
#define  MSG_OFFSET_INSPECTION (WM_USER+5)
#define  MSG_INSPECT_FOCUS (WM_USER+6)
#define  MSG_SAVE_TEMPLATE (WM_USER+7)
#define  MSG_LOAD_TEMPLATE (WM_USER+8)
#define  MSG_THRESHOLD (WM_USER+9)
#define  MSG_SMOOTH (WM_USER+10)
#define  MSG_DERIVATIVE (WM_USER+11)
#define  MSG_EDGE (WM_USER+12)
#define  MSG_MORPHOLOGY (WM_USER+13)
#define  MSG_PITCH_COMPARE (WM_USER+14)
#define  MSG_LABELING (WM_USER+15)
#define  MSG_INSPECT_PARTICLE (WM_USER+16)

#define ITP_NCC	0
#define ITP_SHAPE_BASED	1

#define MSG_ALIGN_CREATE	0
#define MSG_ALIGN_FIND		1
#define MSG_ALIGN_CALCULATE	2

#define MSG_MAIN_MARK	0
#define MSG_SUB_MARK	1

#define MSG_MAIN_MARK_1	0
#define MSG_MAIN_MARK_2	1
#define MSG_SUB_MARK_1	3
#define MSG_SUB_MARK_2	4

// #define  MSG_THRESHOLD (WM_USER+9)
#define MSG_THRESHOLD_MANUAL	0
#define MSG_THRESHOLD_OTSU	1


// #define  MSG_SMOOTH (WM_USER+10)
#define MSG_SMOOTH_BLUR		0
#define MSG_SMOOTH_BOX		1
#define MSG_SMOOTH_MEDIAN	2
#define MSG_SMOOTH_GAUSSIAN	3

// #define  MSG_DERIVATIVE (WM_USER+11)
#define MSG_DERIVATIVE_SOBEL		0
#define MSG_DERIVATIVE_SCHARR		1
#define MSG_DERIVATIVE_LAPLACE		2
#define MSG_DERIVATIVE_PREWITT		3

// #define  MSG_EDGE (WM_USER+12)
#define MSG_EDGE_CANNY		0


// #define  MSG_MORPHOLOGY (WM_USER+12)
#define MSG_MORPHOLOGY_ERODE		0
#define MSG_MORPHOLOGY_DILATION		1

typedef enum librayType 
{
	LIB_C,	// SHOW ORIGINAL IMAGE
	LIB_HALCON,	// SHOW ORIGINAL IMAGE
	LIB_OPENCV
}LIBRARY_TYPE;


typedef enum SelectImage 
{
	SELECT_FIRST,	// SHOW ORIGINAL IMAGE
	SELECT_SECOND	// SHOW ORIGINAL IMAGE
}SELECT_IMAGE;


#ifndef _FLOAT_POINT_
struct FPoint
{
	float x;
	float y;
};
#define _FLOAT_POINT_
#endif


#ifndef _CIRCLED_
typedef struct CircleD
{
	double cx;
	double cy;
	double radius;
} CIRCLED;
#define _CIRCLED_
#endif



#ifndef _RESULT_DATA_
struct stResutltData
{
	stResutltData() 
	{
		x = 0.f;
		y = 0.f;
	};

	float x;
	float y;
};
#define _RESULT_DATA_
#endif


#define INSPECT_PARTICLE_GENERAL		0
#define INSPECT_PARTICLE_SIMULATION_1	1
#define INSPECT_PARTICLE_SIMULATION_N	2


#define PPA_ABSOLUTE	0
#define PPA_RELATIVE	1
#define PPA_POSITION	2