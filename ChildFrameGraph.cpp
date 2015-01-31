// ChildFrameGraph.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h" 
#include "ChildFrameGraph.h"
#include ".\childframegraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrameGraph

IMPLEMENT_DYNCREATE(CChildFrameGraph, CMDIChildWnd)

CChildFrameGraph::CChildFrameGraph()
{
}

CChildFrameGraph::~CChildFrameGraph()
{

}


BEGIN_MESSAGE_MAP(CChildFrameGraph, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrameGraph)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrameGraph message handlers

int CChildFrameGraph::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

if (!m_toolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_toolbar.LoadToolBar(IDR_TC_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_toolbar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_toolbar);

	return 0;
}
