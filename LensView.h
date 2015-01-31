// LensView.h : interface of the CLensView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LENSVIEW_H__FC2B2934_0744_495C_9DB2_F7F5FDA8E2F4__INCLUDED_)
#define AFX_LENSVIEW_H__FC2B2934_0744_495C_9DB2_F7F5FDA8E2F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLensCntrItem;
class CLensDoc;
#include "GLEnabledView.h"
class CLensView : public CGLEnabledView

{
protected: // create from serialization only
	CLensView();
	DECLARE_DYNCREATE(CLensView)

// Attributes
public:
	
	CLensDoc* GetDocument();
	CLensDoc* pDoc;
	int px,py;
	// m_pSelection holds the selection to the current CLensCntrItem.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not CLensCntrItem objects.  This selection
	//  mechanism is provided just to help you get started.

	// TODO: replace this selection mechanism with one appropriate to your app.
	CLensCntrItem* m_pSelection;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLensView)
	public:
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddWall(int x, int y);
	bool RB;
	bool LB;
	virtual void OnCreateGL();
	void OnDrawGL();
	void OnSizeGL(int cx, int cy); 
	void VideoMode(ColorsNumber& c, ZAccuracy& z, BOOL& dbuf);
	virtual ~CLensView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLensView)
	afx_msg void OnDestroy();
	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	// Encodes test action to be performed when the user presses given key combination
	int TestCmd;
	bool CtrlPressed;
};

#ifndef _DEBUG  // debug version in LensView.cpp
inline CLensDoc* CLensView::GetDocument()
   { return (CLensDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LENSVIEW_H__FC2B2934_0744_495C_9DB2_F7F5FDA8E2F4__INCLUDED_)
