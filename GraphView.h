#if !defined(AFX_GRAPHVIEW_H__E9CAC8A2_588D_4FA7_81A6_77A51F07BE9E__INCLUDED_)
#define AFX_GRAPHVIEW_H__E9CAC8A2_588D_4FA7_81A6_77A51F07BE9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphView view
#include  "memdc.h"
#include  "lensDoc.h"

#define BTN_VIEW 1
#define BTN_TC	 2

#include "2DGraph.h"
#include "atltypes.h"
class C2DGraph;
class CGraphView : public CScrollView
{
protected:
	
	DECLARE_DYNCREATE(CGraphView)
   
// Attributes
public:
   	CGraphView();           // protected constructor used by dynamic creation
	CDC MemDC;
	C2DGraph* PSP_graph;
	C2DGraph* EPSC_graph;
	
	//IDR_TC_TOOLBAR
	//int GraphXSize;
	//int GraphYSize;
	int GraphYSpace;
	int GraphXSpace;
	long m_nPageHeight;
	int t_mergin;
	int b_mergin;
	int l_mergin;
	int r_mergin;
	int space;
	CButton m_btn_show_traces,m_btn_show_tc;
	CScrollBar m_ScrollBar;
	bool m_bViewShow,m_bTCShow;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphView)
	public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
		virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void CleanUp();
	virtual ~CGraphView();

protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGraphView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBN_View();
	afx_msg void OnBN_TC();
	afx_msg void OnFile();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	bool CtrlPressed;
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CPoint mouse_click;
};
#ifndef _DEBUG  // debug version in FVProfileView.cpp
//inline CFVProfileDoc* CFVProfileView::GetDocument()
 //  { return (CFVProfileDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHVIEW_H__E9CAC8A2_588D_4FA7_81A6_77A51F07BE9E__INCLUDED_)
