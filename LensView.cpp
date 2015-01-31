// LensView.cpp : implementation of the CLensView class
//

#include "stdafx.h"
#include "Lens.h"

#include "LensDoc.h"
#include "CntrItem.h"
#include "LensView.h"
#include "math.h"
#include ".\lensview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum {TEST_NO_COMMAND,TEST_KILL_COMMAND,TEST_SHOW_WEIGHTS,TEST_SHOW_DELAYS};

/////////////////////////////////////////////////////////////////////////////
// CLensView

IMPLEMENT_DYNCREATE(CLensView, CGLEnabledView)

BEGIN_MESSAGE_MAP(CLensView, CGLEnabledView)
	//{{AFX_MSG_MAP(CLensView)
	ON_WM_DESTROY()
	//ON_WM_SETFOCUS()
	//ON_COMMAND(ID_OLE_INSERT_NEW, OnInsertObject)
	//ON_COMMAND(ID_CANCEL_EDIT_CNTR, OnCancelEditCntr)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLensView construction/destruction

CLensView::CLensView()
: TestCmd(0)
, CtrlPressed(false)
{
	m_pSelection = NULL;
	// TODO: add construction code here
	pDoc = NULL;
	HCURSOR f = AfxGetApp()->LoadCursor(IDC_CURSOR1);
	SetMouseCursor(f );
  	px=py=0;
}

CLensView::~CLensView()
{
}

void CLensView::VideoMode(ColorsNumber& c, ZAccuracy& z, BOOL& dbuf)
{
// ask for 65355 colors...
        c=THOUSANDS;
// ...16 bit Z-buffer...
        z=NORMAL;
// ...double-buffering
        dbuf=TRUE;
}

void CLensView::OnInitialUpdate()
{
	//ooo CGLEnabledView::OnInitialUpdate();
		RECT r;
	   GetClientRect(&r);
	   if(pDoc) 
	   {
			pDoc->zoom_x = float(r.right)/pDoc->field->X;
			pDoc->zoom_y = float(r.bottom)/pDoc->field->Y;
			glPixelZoom(pDoc->zoom_x,pDoc->zoom_y);	
			glViewport(0,0,r.right,r.bottom);
			
	   }

	pDoc->glView = this;

	m_pSelection = NULL;    // initialize selection
	//SetTimer(0,50,0);
}

/////////////////////////////////////////////////////////////////////////////
// CLensView printing

BOOL CLensView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLensView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLensView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLensView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
	
	CView::OnDestroy();

   //KillTimer(0);
}


/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

BOOL CLensView::IsSelected(const CObject* pDocItem) const
{
	// The implementation below is adequate if your selection consists of
	//  only CLensCntrItem objects.  To handle different selection
	//  mechanisms, the implementation here should be replaced.

	// TODO: implement this function that tests for a selected OLE client item

	return pDocItem == m_pSelection;
}


/////////////////////////////////////////////////////////////////////////////
// CLensView diagnostics

#ifdef _DEBUG
void CLensView::AssertValid() const
{
	CView::AssertValid();
}

void CLensView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLensDoc* CLensView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLensDoc)));
	return (CLensDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLensView message handlers

void CLensView::OnDrawGL()
{

		
	 glDrawPixels(	pDoc->field->X,	pDoc->field->Y, GL_RGB,	GL_UNSIGNED_SHORT,	pDoc->field->surf); 
		
				  
	RECT r; GetClientRect(&r);	
	
	float OX = 2.0*float(px - r.right / 2) / float(r.right);
	float OY = 2.0*float(r.bottom/2 - py) / float(r.bottom);
	float radius = pDoc->field->ChangeSize/pDoc->field->X;
	float vectorY1=OY+radius;
	float vectorX1=OX;
	glBegin(GL_LINE_STRIP);		
		glColor3f(1.0f,1.0f,1.0f);
		for(float angle=0.0f;angle<=(2.0f*3.14159);angle+=0.01f)
		{		
		float vectorX=OX +(radius*(float)sin((double)angle));
		float vectorY=OY +(radius*(float)cos((double)angle));		
		glVertex2f(vectorX1,vectorY1);
		vectorY1=vectorY;
		vectorX1=vectorX;			
		}
	glEnd();
	glPixelZoom(pDoc->zoom_x,pDoc->zoom_y);

	glBegin(GL_TRIANGLES );		
		for (int i=0; i< pDoc->graph_view->PSP_graph->Series.GetSize(); i++)
		{
			_2DGRAPH_PARAM_STRUCT* ps = (_2DGRAPH_PARAM_STRUCT*) pDoc->graph_view->PSP_graph->Series[i];
			float ox = 2.0*float(ps->X*pDoc->zoom_x - r.right / 2) / float(r.right);
			float oy = 2.0*float(r.bottom/2 - (pDoc->field->Y - ps->Y)*pDoc->zoom_y) / float(r.bottom);
			float zx = 2./pDoc->field->X;
			float zy = 2./pDoc->field->Y;
			glVertex2f(ox,oy);
			glVertex2f(ox+zx,oy);
			glVertex2f(ox+zx,oy+zy);
			//glRasterPos2f(0,0);
			//Text2D(ps->sLabel);
		}
	glEnd();
	
	glFlush();	
		
	 
}

void CLensView::OnSizeGL(int cx, int cy) 
{
   
	pDoc->zoom_x = float(cx)/pDoc->field->X;
	pDoc->zoom_y = float(cy)/pDoc->field->Y;
	glPixelZoom(pDoc->zoom_x,pDoc->zoom_y);	
	
	//	CGLEnabledView::OnSizeGL(cx,cy);
}



void CLensView::OnCreateGL()
{
	CGLEnabledView::OnCreateGL();
	PrepareCharset2D("Arial",12,false,false);
	

	pDoc = (CLensDoc*)m_pDocument;     	  
		   
}


void CLensView::OnMouseMove(UINT nFlags, CPoint point) 
{
	px = point.x;
	py = point.y;
	
	if (TestCmd != TEST_NO_COMMAND)
	{
		pDoc->field->TestShowInternalData(float(point.x)/pDoc->zoom_x, float(point.y)/pDoc->zoom_y, &TestCmd);
	}
	if (nFlags==MK_LBUTTON)
	{
		pDoc->field->UpdateValues(float(point.x)/pDoc->zoom_x, float(point.y)/pDoc->zoom_y);
		
	}
	
	_2DGRAPH_PARAM_STRUCT *ps1=NULL,*ps2=NULL;
	int i=0;
	int epsx = float(pDoc->field->X)/100.0;
	int epsy = float(pDoc->field->Y)/100.0;
	for (i=0; i< pDoc->graph_view->PSP_graph->Series.GetSize(); i++)
		{
			ps1 = (_2DGRAPH_PARAM_STRUCT*) pDoc->graph_view->PSP_graph->Series[i];
			ps2 = (_2DGRAPH_PARAM_STRUCT*) pDoc->graph_view->EPSC_graph->Series[i];
			if (
				(abs(int(ps1->X - int(float(point.x)/pDoc->zoom_x))) <= epsx)	&&
				(abs(int(ps1->Y - int(float(pDoc->field->Y) - float(point.y)/pDoc->zoom_y))) <= epsy )
				)
			{
				ps1->MouseOver = true;
				ps2->MouseOver = true;
			}
			else
			{
				ps1->MouseOver = false;
				ps2->MouseOver = false;
			}
		}
	Invalidate();
	pDoc->graph_view->Invalidate();
	CGLEnabledView::OnMouseMove(nFlags, point);
}

void CLensView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	
	pDoc->field->UpdateValues(float(point.x)/pDoc->zoom_x, float(point.y)/pDoc->zoom_y);
	
	CGLEnabledView::OnLButtonUp(nFlags, point);
	Invalidate(FALSE);
}

void CLensView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pDoc->field->oldx = float(point.x)/pDoc->zoom_x;
	pDoc->field->oldy = float(pDoc->field->Y) - float(point.y)/pDoc->zoom_y;

	CGLEnabledView::OnLButtonDown(nFlags, point);
}

BOOL CLensView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CGLEnabledView::OnMouseWheel(nFlags, zDelta, pt);
}

void CLensView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	bool found=false;
	_2DGRAPH_PARAM_STRUCT* ps=NULL;
	int i=0;
	int epsx = float(pDoc->field->X)/100.0;
	int epsy = float(pDoc->field->Y)/100.0;
	for (i=0; i< pDoc->graph_view->PSP_graph->Series.GetSize(); i++)
		{
			ps = (_2DGRAPH_PARAM_STRUCT*) pDoc->graph_view->PSP_graph->Series[i];
			if ((abs(int(ps->X - int(float(point.x)/pDoc->zoom_x))) <= epsx)	&&
				(abs(int(ps->Y - int(float(pDoc->field->Y) - float(point.y)/pDoc->zoom_y))) <= epsy ))
			{found = true; break;}
		}
	if (!found)
		pDoc->AddPointEvolutionGraph(float(point.x)/pDoc->zoom_x,float(pDoc->field->Y) - float(point.y)/pDoc->zoom_y);
	else
	{	
		pDoc->RemoveEvolution(i);
	}
	
	CGLEnabledView::OnRButtonUp(nFlags, point);
}

void CLensView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CGLEnabledView::OnRButtonDown(nFlags, point);
}

void CLensView::AddWall(int x, int y)
{
	
}

void CLensView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGLEnabledView::OnKeyDown(nChar, nRepCnt, nFlags);

	nFlags = nFlags & 0xFF;
	if (nFlags == 29) { CtrlPressed = true; return;}

	switch (nFlags)
	{
		case 17: // Ctrl-W pressed
			{TestCmd = TEST_SHOW_WEIGHTS;} break;
		case 32: // Ctrl-D pressed
			{TestCmd = TEST_SHOW_DELAYS;} break;
	}
	CtrlPressed = false;
	pDoc->field->oldx=	float(px)/pDoc->zoom_x;
	pDoc->field->oldy = float(pDoc->field->Y) - float(py)/pDoc->zoom_y;
	pDoc->field->TestShowInternalData(float(px)/pDoc->zoom_x, float(py)/pDoc->zoom_y, &TestCmd);
	Invalidate();
}

void CLensView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGLEnabledView::OnKeyUp(nChar, nRepCnt, nFlags);
	CtrlPressed = false;
	TestCmd = TEST_NO_COMMAND;
		
	pDoc->field->TestShowInternalData(float(px)/pDoc->zoom_x, float(py)/pDoc->zoom_y, &TestCmd);
	Invalidate();
	
}
