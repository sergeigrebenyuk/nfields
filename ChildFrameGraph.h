#if !defined(AFX_CHILDFRAMEGRAPH_H__9684E454_CE70_49F8_A693_325DA2C2A5FC__INCLUDED_)
#define AFX_CHILDFRAMEGRAPH_H__9684E454_CE70_49F8_A693_325DA2C2A5FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildFrameGraph.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildFrameGraph frame

class CChildFrameGraph : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrameGraph)
public:
//protected:
	CChildFrameGraph();           // protected constructor used by dynamic creation
	CToolBar m_toolbar;
// Attributes


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrameGraph)
	//}}AFX_VIRTUAL

// Implementation
//protected:
	virtual ~CChildFrameGraph();

	// Generated message map functions
	//{{AFX_MSG(CChildFrameGraph)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRAMEGRAPH_H__9684E454_CE70_49F8_A693_325DA2C2A5FC__INCLUDED_)
