// LensDoc.h : interface of the CLensDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LENSDOC_H__58698727_B1B0_4091_84CD_4F9D0EAB9F4F__INCLUDED_)
#define AFX_LENSDOC_H__58698727_B1B0_4091_84CD_4F9D0EAB9F4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LensView.h"
#include "GLToMovie.h"
#include "Lens.h"
#include "statdlg.h"
#include "ChildFrameGraph.h"
#include "GraphView.h"
#include "Field.h"



class CGraphView;
class CLensDoc : public CDocument
{
protected: // create from serialization only
	CLensDoc();
	DECLARE_DYNCREATE(CLensDoc)
	
	LPCRITICAL_SECTION CriticalSection;

// Attributes
public:
	
	// SIMULATION RELATED
	
	CFieldSimulator * field;

	// DOCUMENT RELATED

	CGLToMovie*			recorder;							// AVI file recorder

	float				zoom_x,zoom_y;						// zoom factors of the simulatied field in view
	bool				bRecordAVI;							// just some flags
	int					nAVIFileIndex;						// index of newly created AVI files
	CPtrArray			AVIList;							// list of AVI files created during simulation
	
	volatile CLensApp *			app;								// pointer to the application class
	
	CStatDlg *			stat_dlg;							// control panel window
	
	CChildFrameGraph*	graph_frame;
	CGraphView*			graph_view, *graph_view0;
	CLensView *			glView;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLensDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	int			RestartSimulation();						// Called for reinitialization of the numeric core
	virtual		~CLensDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLensDoc)
	afx_msg void OnWriteavi();
	afx_msg void OnSimpause();
	afx_msg void OnSimresume();
	afx_msg void OnRestartsim();
	afx_msg void OnSetwall();
	afx_msg void OnTonicinput();
	afx_msg void OnInitialvalue();
	afx_msg void OnOpenDoc();
	afx_msg void OnSaveDoc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
	// // makes initializations of the environment
	int Initialize(void);
	bool OnceOpen;
	int AddPointEvolutionGraph(UINT x, UINT y);
	int RemoveEvolution(int i);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LENSDOC_H__58698727_B1B0_4091_84CD_4F9D0EAB9F4F__INCLUDED_)
