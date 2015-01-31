// GraphView.cpp : implementation file
//

#include "stdafx.h"

#include "GraphView.h"
#include "2DGraph.h"
#include ".\graphview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CSize ScrollSize;
/////////////////////////////////////////////////////////////////////////////
// CGraphView

IMPLEMENT_DYNCREATE(CGraphView, CScrollView)

CGraphView::CGraphView()
: CtrlPressed(false)
, mouse_click(0)
{
	//sizeTotal.cx=sizeTotal.cy=100;
	ScrollSize.cx=50;
	ScrollSize.cy=50;

	GraphYSpace = 30;
	GraphXSpace = 30;


	t_mergin = 10;
	b_mergin = 10;
	l_mergin = 10;
	r_mergin = 5;
	space = 10;
	m_bViewShow = m_bTCShow = true;

	PSP_graph = new C2DGraph;

	PSP_graph->sLabel = "Membrane potential - <mV,ms>";
	PSP_graph->xFrom = 0.;
	PSP_graph->xTo = 0.05;
	PSP_graph->yFrom = -150.;
	PSP_graph->yTo = 50.;
	PSP_graph->yGrid = 0.1;
	PSP_graph->xGrid = 10;

	EPSC_graph = new C2DGraph;

	EPSC_graph->sLabel = "Excitatory current - <pA,ms>";
	EPSC_graph->xFrom = 0.;
	EPSC_graph->xTo =  0.05;
	EPSC_graph->yFrom = -10.;
	EPSC_graph->yTo = 300.;
	EPSC_graph->yGrid = 0.1;
	EPSC_graph->xGrid = 10;

	CtrlPressed = false;
}

CGraphView::~CGraphView()
{
	CleanUp();
	delete PSP_graph; PSP_graph = NULL;
	delete EPSC_graph; EPSC_graph = NULL;
}


BEGIN_MESSAGE_MAP(CGraphView, CScrollView)
	//{{AFX_MSG_MAP(CGraphView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_TC_TO_FILE, OnFile)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_BN_CLICKED( BTN_VIEW, OnBN_View )
	ON_BN_CLICKED( BTN_TC, OnBN_TC )



	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphView drawing

void CGraphView::OnBN_View()
{

	m_bViewShow = !m_bViewShow;

	//m_btn_show_traces.SetState(m_bViewShow );

}
void CGraphView::OnBN_TC()
{
	m_bTCShow = !m_bTCShow;
	
	//m_btn_show_tc.SetState(m_bTCShow);
}

int CGraphView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	((CLensDoc*)GetDocument())->graph_view = this;
	
/*
	m_btn_show_traces.Create("View",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON ,CRect(5,5,40,20), this, BTN_VIEW);
	m_btn_show_tc.Create("TC",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON ,CRect(45,5,80,20), this, BTN_TC);
	
	RECT r; 
	GetClientRect(&r);
	r.top = r.bottom-50;
	VERIFY(m_ScrollBar.Create(SBS_HORZ | SBS_TOPALIGN | WS_CHILD, r, this, 100));
    m_ScrollBar.ShowScrollBar();
*/	

	return 0;
}

void CGraphView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	
	
	// TODO: calculate the total size of this view
	
	SetScrollSizes(MM_TEXT, CSize(500,100));
}

/////////////////////////////////////////////////////////////////////////////
// CFVProfileView printing

BOOL CGraphView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(1);
	return DoPreparePrinting(pInfo);
}

void CGraphView::OnBeginPrinting(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	m_nPageHeight=pDC->GetDeviceCaps(VERTRES);
}

void CGraphView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


void CGraphView::OnDraw(CDC* pDC)
{
//	CFVProfileDoc* pDoc = GetDocument();
	CMemDC DC(pDC); 
	if (!DC.m_hDC) return;
	
	
	CFont InfoFont,GridFont;
	GridFont.CreatePointFont(70, "Arial", DC);
	InfoFont.CreatePointFont(70, "Arial", DC);
	DC.SetMapMode(MM_TEXT);
	DC.SetBkMode( TRANSPARENT );
	DC.SetStretchBltMode(HALFTONE);
	
	::SetROP2(DC.GetSafeHdc(),R2_COPYPEN);
	
	//CString	strChan; strChan.Format("Total ROI : XXX");
	//DC.SelectObject(&InfoFont);
	//DC.TextOut(0, 0,strChan);
				
	CPen penAxis;
	penAxis.CreatePen(PS_SOLID, 1, RGB(0, 0, 100));
	CPen* pOldPen = DC.SelectObject(&penAxis);							
	
			
	InfoFont.DeleteObject();
	GridFont.DeleteObject();

	SetScrollSizes(MM_TEXT, ScrollSize);	
	
	PSP_graph->Draw(DC);
	EPSC_graph->Draw(DC);
	

	CLensDoc* pDoc  = (CLensDoc*)GetDocument();	
	//m_ScrollBar.SetScrollRange(0,pDoc->Ticks);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphView diagnostics

#ifdef _DEBUG
void CGraphView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGraphView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphView message handlers

BOOL CGraphView::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
	//return CScrollView::OnEraseBkgnd(pDC);
}

void CGraphView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	
	
	point.x+= GetScrollPos(SB_HORZ);
	point.y+= GetScrollPos(SB_VERT);

	

	//Invalidate(FALSE);	
	
	CScrollView::OnLButtonUp(nFlags, point);
}


void CGraphView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	
	point.x+= GetScrollPos(SB_HORZ);
	point.y+= GetScrollPos(SB_VERT);

	

	//Invalidate(FALSE);	
	
		

	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CGraphView::CleanUp()
{
	
}



void CGraphView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	
	CScrollView::OnPrepareDC(pDC, pInfo);

	 if(pDC->IsPrinting()) // Is the DC a printer DC.
   {
      CRect rect;
      GetClientRect (&rect);
	
      pDC->SetViewportOrg(0, m_nPageHeight);
   }

}

void CGraphView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	RECT cr,cr1;
	GetClientRect(&cr);
	GetClientRect(&cr1);
	//m_ScrollBar.MoveWindow(cr.left,cr.bottom-b_mergin,cr.right,b_mergin);
	cr.top += t_mergin;
	cr.bottom /= 2;
	//cr.bottom -= b_mergin/4;
	cr.left += l_mergin;
	cr.right -= r_mergin;
	
	PSP_graph->SetCanvas(NULL, cr);
	
	cr1.top = cr1.bottom / 2;
	//cr1.top += t_mergin/4;
	cr1.bottom -= b_mergin;
	cr1.left += l_mergin;
	cr1.right -= r_mergin;

	EPSC_graph->SetCanvas(NULL, cr1);

	
}

void CGraphView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CLensDoc* pDoc  = (CLensDoc*)GetDocument();
	float range = PSP_graph->xTo - PSP_graph->xFrom;
	float dt = CtrlPressed? range/2. : 2.*range/float(PSP_graph->Area.right-PSP_graph->Area.left);
	switch(nChar)
	{
		case 17: CtrlPressed = true;
		case 37: //LEFT
			{
				if (PSP_graph->xFrom - dt>=0)
				{
					PSP_graph->xFrom -=dt; 
					PSP_graph->xTo -=dt;   
					
					EPSC_graph->xFrom -=dt; 
					EPSC_graph->xTo -=dt;   
				}
				Invalidate();
			}
			break;
		case 39: //RIGHT
			{
				//if (PSP_graph->xTo + dt<= float( (pDoc->Ticks*int(pDoc->dT*1000000.0))/1000000 ))
				{
					PSP_graph->xFrom +=dt;
					PSP_graph->xTo +=dt;

					EPSC_graph->xFrom +=dt;
					EPSC_graph->xTo +=dt;
				}
				Invalidate();
			}
			break;
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGraphView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 17) CtrlPressed = false;
	
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CGraphView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CScrollView::OnMouseWheel(nFlags, zDelta, pt);
		CLensDoc* pDoc  = (CLensDoc*)GetDocument();	
	ScreenToClient( &pt );

	bool side =  (pt.y < PSP_graph->Area.bottom);
	
	if (nFlags==MK_RBUTTON)
	{
		float t = side ? PSP_graph->yTo - PSP_graph->yFrom : EPSC_graph->yTo - EPSC_graph->yFrom;
		float w = side ? PSP_graph->Area.bottom - PSP_graph->Area.top : EPSC_graph->Area.bottom - EPSC_graph->Area.top;
		float w_t = w/t;
		
		float tt = side ? float(pt.y - PSP_graph->Area.top)/w_t : float(pt.y - EPSC_graph->Area.top)/w_t;
		float bt = side ? float(PSP_graph->Area.bottom - pt.y)/w_t : float(EPSC_graph->Area.bottom - pt.y)/w_t;
		float sn = zDelta < 0? -1.0: 1.0;
	
		if (side)
		{
			PSP_graph->yFrom += sn*bt/10.0; 
			PSP_graph->yTo -= sn*tt/10.0;
		}
		else
		{
			EPSC_graph->yFrom += sn*bt/10.0; 
			EPSC_graph->yTo -= sn*tt/10.0;
		}
	}
	else
	{
		float t = PSP_graph->xTo - PSP_graph->xFrom;	
		float w = PSP_graph->Area.right - PSP_graph->Area.left;
		float w_t = w/t;
		
		float lt = float(pt.x - PSP_graph->Area.left)/w_t;
		float rt = float(PSP_graph->Area.right - pt.x)/w_t;
		float sn = zDelta < 0? -1.0: 1.0;
		PSP_graph->xFrom += sn*lt/10.0; if (PSP_graph->xFrom < 0) PSP_graph->xFrom=0;
		PSP_graph->xTo -= sn*rt/10.0;
		EPSC_graph->xFrom += sn*lt/10.0; if (PSP_graph->xFrom < 0) PSP_graph->xFrom=0;
		EPSC_graph->xTo -= sn*rt/10.0;
	}

	Invalidate();
	return TRUE;
}

static char BASED_CODE szFilter[] = "ASCII Files (*.txt)|*.txt|All Files (*.*)|*.*||";
void CGraphView::OnFile()
{
	CLensDoc* pDoc  = (CLensDoc*)GetDocument();\
		_2DGRAPH_PARAM_STRUCT* ps=NULL;
		FILE* f=NULL,*f2=NULL;
        int cnt=0;
		CFileDialog dlg(FALSE,"TXT",0,0,szFilter,AfxGetMainWnd(),0);
		if (dlg.DoModal()== IDCANCEL) return;

		CString fn = dlg.GetPathName();
        
        bool goahead=false;
        
        f = fopen(fn,"r");
        f2 = fopen(fn,"a+");

        if (f && f2)
        {
                goahead = (AfxMessageBox("Overwrite existing?",MB_YESNO|MB_ICONWARNING)==IDYES) ? true: false;
        } 
        else 
        {
                goahead=true;
                
        }
        if (f) fclose(f);
        if (f2) fclose(f2);

        if (!goahead) {fclose(f); return ;}
                
        f = fopen(fn,"wt");
                

        if (!f) {AfxMessageBox("Error opening file! Action cancelled.");return ;}

        CString strHeader;
        CString file_content;
		bool bHeader = true;

        fseek(f,0,SEEK_SET);
		strHeader = "Time, s\t";
        for(int i=0; i < PSP_graph->Series.GetSize(); i++)
		{
			ps = (_2DGRAPH_PARAM_STRUCT*) PSP_graph->Series[i];
			CString s;
			s.Format("<%i,%i>\t",ps->X,ps->Y);
			strHeader += s;
		
		}
        fprintf(f,"%s\n",strHeader);
		
		bool go = true;
		unsigned __int32 nTick=0;
		int finished_graphs=0;
		while (go)
		{
			CString line = "";
			CString s = "";
			
			line.Format("%f\t",float(nTick)*pDoc->field->dT);
			int cnt=PSP_graph->Series.GetSize();
			
			for(int i=0; i < PSP_graph->Series.GetSize(); i++)
			{
				ps = (_2DGRAPH_PARAM_STRUCT*) PSP_graph->Series[i];
				if (( nTick >= ps->nTick)&&(nTick - ps->nTick < ps->data->GetSize()))
				{				
					s.Format("%f\t",(*ps->data)[nTick - ps->nTick]);
					if (nTick - ps->nTick+1 == ps->data->GetSize())
					finished_graphs ++;
				}
				else
				{
					s = "\t";
					cnt--;
				}
				line += s;			
			}
			nTick++;
			fprintf(f,"%s\n",line);
			if ((!cnt)&&(finished_graphs==PSP_graph->Series.GetSize())) go = false;
		}
        
		


        fflush(f);
        fclose(f);

 return;
}

void CGraphView::OnMouseMove(UINT nFlags, CPoint point)
{
	bool side = (point.y < PSP_graph->Area.bottom);
	if (nFlags ==MK_LBUTTON)
	{
		float yt = side ? PSP_graph->yTo - PSP_graph->yFrom : EPSC_graph->yTo - EPSC_graph->yFrom;	
		float yw = side ? PSP_graph->Area.bottom - PSP_graph->Area.top : EPSC_graph->Area.bottom - EPSC_graph->Area.top;
		float yw_t = yw/yt;
		
		float xt = PSP_graph->xTo - PSP_graph->xFrom;	
		float xw = PSP_graph->Area.right - PSP_graph->Area.left;
		float xw_t = xw/xt;

		
		float dx = float(point.x-mouse_click.x)/xw_t;
		float dy = float(point.y-mouse_click.y)/yw_t;

		PSP_graph->xFrom -= dx;
		PSP_graph->xTo -= dx;
		EPSC_graph->xFrom -= dx;
		EPSC_graph->xTo -= dx;
		
	if (side)
	{	
		PSP_graph->yFrom += dy;
		PSP_graph->yTo += dy;
	}
	else
	{
		EPSC_graph->yFrom += dy;
		EPSC_graph->yTo += dy;
	}
		mouse_click = point;
		Invalidate();
	}
	CScrollView::OnMouseMove(nFlags, point);
}

void CGraphView::OnLButtonDown(UINT nFlags, CPoint point)
{
	
mouse_click = point;
	CScrollView::OnLButtonDown(nFlags, point);
}
