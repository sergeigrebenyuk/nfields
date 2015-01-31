// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Lens.h"

#include "MainFrm.h"
#include "LensDoc.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	TBBUTTONINFO bi;
    m_wndToolBar.GetToolBarCtrl().GetButtonInfo(ID_WRITEAVI, &bi);
    bi.cbSize = sizeof(TBBUTTONINFO);
    bi.dwMask = TBIF_STYLE;
    bi.fsStyle = TBSTYLE_CHECK;
    m_wndToolBar.GetToolBarCtrl().SetButtonInfo(ID_WRITEAVI, &bi);
	//m_wndToolBar.GetToolBarCtrl().SetButtonInfo(ID_TONICINPUT, &bi);
	//m_wndToolBar.GetToolBarCtrl().SetButtonInfo(ID_INITIALVALUE, &bi);
	//m_wndToolBar.GetToolBarCtrl().SetButtonInfo(ID_SETWALL, &bi);


    
    
    WINDOWPLACEMENT wp;
    wp.length = sizeof(wp);
    wp.showCmd = SW_SHOWMAXIMIZED;
    SetWindowPlacement( &wp ); 


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

/*
void CMainFrame::OnWriteavi() 
{
	//CLensApp * app = (CLensApp *)AfxGetApp();
	//app->glView->pDoc->bRecordAVI = m_wndToolBar.GetToolBarCtrl().IsButtonChecked(ID_WRITEAVI);     
	//app->glView->pDoc->OnWriteavi();
	
}
*/

int CMainFrame::CreateGraphView(void)
{
        CMDIChildWnd* pActiveChild = MDIGetActive();    
    //CFVProfileDoc* pDocument = ((CFVProfileApp*)AfxGetApp())->pDoc;
    CLensDoc* pDocument = NULL;  
        if (pActiveChild == NULL ||
          (pDocument = (CLensDoc*)pActiveChild->GetActiveDocument()) == NULL)
        {
                TRACE0("Warning: No active document for WindowNew command.\n");
                AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
                return 0;     // command failed
        }
       
    
    CDocTemplate* pTemplate = pDocument->GetDocTemplate();

    CCreateContext context;

    context.m_pCurrentDoc = pDocument;
    context.m_pCurrentFrame = pDocument->graph_frame; 
    context.m_pNewViewClass = pDocument->graph_view->GetRuntimeClass( );
    context.m_pNewDocTemplate = pTemplate;
        
        if (!pDocument->graph_frame->m_hWnd) 
                pDocument->graph_frame->Create( 0, _T("Profile analysis"), WS_CAPTION|WS_CHILD| WS_VISIBLE |WS_THICKFRAME , CRect(0,0,200,100), this, &context );

        // otherwise we have a new frame !

    pTemplate = pDocument->GetDocTemplate();
        ASSERT_VALID(pTemplate);
        CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDocument, pDocument->graph_frame);
        if (pFrame == NULL)
        {
                TRACE0("Warning: failed to create new frame.\n");
                return 0;     // command failed
        }
   
        pTemplate->InitialUpdateFrame(pDocument->graph_frame, pDocument);
        pDocument->graph_frame->ShowWindow(SW_SHOW);
        pDocument->graph_frame->SetWindowPos(&wndTop,200, 0, 500, 400, SWP_SHOWWINDOW);   

	return 0;
}
