// LensDoc.cpp : implementation of the CLensDoc class
//

#include "stdafx.h"
#include "Lens.h"

#include "LensDoc.h"
#include "CntrItem.h"
#include "math.h"
#include "process.h"
#include ".\lensdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CLensDoc

IMPLEMENT_DYNCREATE(CLensDoc, CDocument)

BEGIN_MESSAGE_MAP(CLensDoc, CDocument)
	//{{AFX_MSG_MAP(CLensDoc)
	ON_COMMAND(ID_WRITEAVI, OnWriteavi)
	ON_COMMAND(ID_SIMPAUSE, OnSimpause)
	ON_COMMAND(ID_SIMRESUME, OnSimresume)
	ON_COMMAND(ID_RESTARTSIM, OnRestartsim)
	ON_COMMAND(ID_SETWALL, OnSetwall)
	ON_COMMAND(ID_TONICINPUT, OnTonicinput)
	ON_COMMAND(ID_INITIALVALUE, OnInitialvalue)
	ON_COMMAND(ID_FILE_CUSTOM_OPEN, OnOpenDoc)
	ON_COMMAND(ID_FILE_CUSTOM_SAVE, OnSaveDoc)

	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	//ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, COleDocument::OnUpdatePasteMenu)
	//ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, COleDocument::OnUpdatePasteLinkMenu)
	//ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_CONVERT, COleDocument::OnUpdateObjectVerbMenu)
	//ON_COMMAND(ID_OLE_EDIT_CONVERT, COleDocument::OnEditConvert)
	//ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, COleDocument::OnUpdateEditLinksMenu)
	//ON_COMMAND(ID_OLE_EDIT_LINKS, COleDocument::OnEditLinks)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, COleDocument::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLensDoc construction/destruction

CLensDoc::CLensDoc()
{
	// Use OLE compound files
	//EnableCompoundFile();
	
		OnceOpen = false;
		glView = NULL;
		field = new CFieldSimulator;
		graph_frame = (CChildFrameGraph*)new CChildFrameGraph;
        graph_view0 = graph_view = (CGraphView*)new CGraphView;
		
}

CLensDoc::~CLensDoc()
{
	// delete NumCore;
	delete field; 
	delete graph_view0;	 
	 //delete [] mask; mask = NULL;
}





#ifdef _DEBUG
void CLensDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLensDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLensDoc commands






BOOL CLensDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	if (Initialize()) return FALSE;
	
	RestartSimulation();  
	  
	  return TRUE;
}

void CLensDoc::OnCloseDocument() 
{
	
	field->TerminateCore();

	for(int i=0; i< AVIList.GetSize(); i++)
		delete AVIList[i];
	AVIList.RemoveAll();
	
	
	
	stat_dlg->DestroyWindow( );
	delete stat_dlg;
	
	CDocument::OnCloseDocument();
}

void CLensDoc::OnWriteavi() 
{
	
	bRecordAVI = !bRecordAVI;
	
	if (bRecordAVI)
	{
		RECT r;
	   glView->GetClientRect(&r);
	   //bDeleteAVI = false;
	   
	   	CString s;
		s.Format("Simulation%i.avi", nAVIFileIndex++);
		//if (!recorder) 
			recorder = new  CGLToMovie(s, r.right,r.bottom,24,/*mmioFOURCC('M','P','G','4')*/ 0,1);
		AVIList.Add(recorder);
	}
	else
	{
	   //bDeleteAVI = true;
	  
	   
	}

	

}


void CLensDoc::OnSimpause() 
{
	
	field->SuspendCore();

}

void CLensDoc::OnSimresume() 
{
	field->ResumeCore();
	
}

void CLensDoc::OnRestartsim() 
{
	RestartSimulation();
	
}


void CLensDoc::OnSetwall() 
{
	field->SetSimulationState(STAT_WALLS);
	stat_dlg->SetStatus();
}

void CLensDoc::OnTonicinput() 
{
	field->SetSimulationState(STAT_TONIC);
	stat_dlg->SetStatus();
	
}

void CLensDoc::OnInitialvalue() 
{
	field->SetSimulationState(STAT_INIT);
	stat_dlg->SetStatus();
	
}


int CLensDoc::RestartSimulation()
{
	field->RestartCore();
		
	// zeroing all point evolution series
	for (int i=0; i < graph_view->PSP_graph->Series.GetSize(); i++)
		{
			_2DGRAPH_PARAM_STRUCT * ps = (_2DGRAPH_PARAM_STRUCT *)graph_view->PSP_graph->Series[i];
			ps->data->RemoveAll();
			ps->nTick = 0;
			ps->dt = field->dT;
		}
	for ( i=0; i < graph_view->EPSC_graph->Series.GetSize(); i++)
		{
			_2DGRAPH_PARAM_STRUCT * ps = (_2DGRAPH_PARAM_STRUCT *)graph_view->EPSC_graph->Series[i];
			ps->data->RemoveAll();
			ps->nTick = 0;
			ps->dt = field->dT;
		}

	field->ResumeCore();
	if (glView)
	{
		RECT r;
		glView->GetClientRect(&r);

		zoom_x = float(r.right)/field->X;
		zoom_y = float(r.bottom)/field->Y;
		glPixelZoom(zoom_x,zoom_y);	
	}
return 0;

}



static char BASED_CODE szFilter[] = "Lens Files (*.lns)|*.lns|All Files (*.*)|*.*||";

void CLensDoc::OnSaveDoc()
{
	CFileDialog dlg(FALSE,"lns",0,0,szFilter,AfxGetMainWnd(),0);
	if (dlg.DoModal()== IDCANCEL) return;

	CString fn = dlg.GetPathName();
	CFile ar(fn,CFile::modeWrite|CFile::modeCreate);

	field->SaveStateToFile(ar);
	ar.Close();
	//return CDocument::OnSaveDocument(lpszPathName);
	
}

void CLensDoc::OnOpenDoc()
{
	CFileDialog dlg(TRUE,"lns",0,0,szFilter,AfxGetMainWnd(),0);
	if (dlg.DoModal()== IDCANCEL) return;

//	if (!OnceOpen ) Initialize();
	OnceOpen = true;
	CString fn = dlg.GetPathName();
	CFile ar(fn,CFile::modeRead);
	
	field->LoadStateFromFile(ar);

	stat_dlg->SetStatus();
	ar.Close();
	
	if (glView)
	{
		RECT r;
		glView->GetClientRect(&r);

		zoom_x = float(r.right)/field->X;
		zoom_y = float(r.bottom)/field->Y;
		glPixelZoom(zoom_x,zoom_y);	
	}

}

void RenderCallbackFunc( void * pt)
{
	
	CLensDoc* pDoc = (CLensDoc*) pt;
	if (pDoc->field->GetCommand()!=CMD_RUN) return;
	// update evoultion series
	for (int i=0; i< pDoc->graph_view->PSP_graph->Series.GetSize(); i++)
	{
		_2DGRAPH_PARAM_STRUCT* ps = (_2DGRAPH_PARAM_STRUCT*) pDoc->graph_view->PSP_graph->Series[i];
		ps->data->Add(pDoc->field->GetFieldPoint(ps->X,ps->Y,1)*1E3);
		ps = (_2DGRAPH_PARAM_STRUCT*) pDoc->graph_view->EPSC_graph->Series[i];
		ps->data->Add(pDoc->field->GetFieldPoint(ps->X,ps->Y,0)*1E12);
	}
	// Send redraw requests
	pDoc->glView->PostMessage(WM_PAINT,0,0);
	pDoc->graph_view->Invalidate(FALSE);//PostMessage(WM_PAINT,0,0);
	if (pDoc->bRecordAVI) { pDoc->recorder->RecordFrame();}
}
// // makes initializations of the environment
int CLensDoc::Initialize(void)
{
	app = (CLensApp *)AfxGetApp();

	bRecordAVI = false;
	nAVIFileIndex = 0;
	recorder = NULL;
	
	field->SetSimulationState(STAT_TONIC);
	field->SetRenderingCallback(this, (TFunc*)&RenderCallbackFunc);
	field->InitializeCore();
	
	stat_dlg = (CStatDlg*) new CStatDlg(this, AfxGetMainWnd());
	
	stat_dlg->Create(IDD_STATDLG,AfxGetMainWnd());
	 
	return 0;
}


int CLensDoc::AddPointEvolutionGraph(UINT X, UINT Y)
{
	_2DGRAPH_PARAM_STRUCT* ps = new _2DGRAPH_PARAM_STRUCT;
	ps->X = X;
	ps->Y = Y;
	ps->b_visible = true;
	ps->dt = field->dT;
	ps->n_LineWidth = 1;
	ps->sLabel.Format("Evolution at (%i,%i)",X,Y);
	ps->microns = false;
	ps->MouseOver = true;
	ps->c_type = GT_LINE;
	ps->data = new CArray<float,float>;
	graph_view->PSP_graph->AddSerie(ps);
	ps->flag_writing = false;
	ps->nTick = field->Ticks;

	ps = new _2DGRAPH_PARAM_STRUCT;
	ps->X = X;
	ps->Y = Y;
	ps->b_visible = true;
	ps->dt = field->dT;
	ps->n_LineWidth = 1;
	ps->sLabel.Format("Evolution at (%i,%i)",X,Y);
	ps->microns = false;
	ps->MouseOver = true;
	ps->c_type = GT_LINE;
	ps->data = new CArray<float,float>;
	graph_view->EPSC_graph->AddSerie(ps);
	ps->flag_writing = false;
	ps->nTick = field->Ticks;

	return 0;
}

int CLensDoc::RemoveEvolution(int i)
{
	_2DGRAPH_PARAM_STRUCT* ps = (_2DGRAPH_PARAM_STRUCT*) graph_view->PSP_graph->Series[i];
		graph_view->PSP_graph->Series.RemoveAt(i);
		ps->data->RemoveAll();
		delete ps;
	 ps = (_2DGRAPH_PARAM_STRUCT*) graph_view->EPSC_graph->Series[i];
		graph_view->EPSC_graph->Series.RemoveAt(i);
		ps->data->RemoveAll();
		delete ps;
	return 0;
}

