// 2DGraph.h: interface for the C2DGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_2DGRAPH_H__2935AE1C_EB81_40B5_B8BF_91E239AE137C__INCLUDED_)
#define AFX_2DGRAPH_H__2935AE1C_EB81_40B5_B8BF_91E239AE137C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LensDoc.h"
#include <afxtempl.h>

enum {	// line types in 2DGraph class
		GT_DOT,		// Data points only
		GT_LINE		// Lines connecting data points
};
typedef struct
	{
		__int64		nTick;				// starting time of timecourse
		UINT		X;
		UINT		Y;
		char		c_type;
		COLORREF	x_col,x_colsel;
		char		c_sym;
		char		n_LineWidth;
		CArray <float,float>*	data;
		CString		sLabel;
		union
		{
			float		fTime;
			float		fMicrons;
		};
		float		dt;
		bool		b_visible;
		bool		microns;
		bool		MouseOver;
		bool		flag_writing;

	} _2DGRAPH_PARAM_STRUCT;


class C2DGraph  
{
public:
	bool OnPoint(CPoint &p);
	int AddXMark(float X, CString &Label);
	void Draw(CDC* DC);
	RECT Area;
	CPtrArray	Series;
	float		xZoom,yZoom;
	float		xFrom,xTo,yFrom,yTo;
	float		xGrid,yGrid;
	
	COLORREF	m_cTrialCol,m_cAverCol,m_cTrialColSel,m_cAverColSel;
	int			m_nTrialStyle, m_nAverStyle,m_nTrialLineWidth, m_nAverLineWidth;

	CString		sLabel;
	// sets attributes of serie pointed by idx
	void SetSerieAttributes(int idx, _2DGRAPH_PARAM_STRUCT * s);
	// returns index of added serie
	int AddSerie( _2DGRAPH_PARAM_STRUCT * s);

	void SetCanvas(CDC *dc,RECT &r);
	
	C2DGraph();
	virtual ~C2DGraph();

};

#endif // !defined(AFX_2DGRAPH_H__2935AE1C_EB81_40B5_B8BF_91E239AE137C__INCLUDED_)
