// 2DGraph.cpp: implementation of the C2DGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "2DGraph.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C2DGraph::C2DGraph()
{

	xFrom=0.;
	xTo=-1.0;
	yFrom=-150.;
	yTo=50.;
	xZoom = 1.;
	yZoom = 1.;
	sLabel= "Unlabeled graph";
	//CFVProfileApp* app = (CFVProfileApp*)AfxGetApp();

	m_cTrialCol = RGB(0,0,0);
	m_cAverCol = RGB(0,0,0);
	m_cTrialColSel = RGB(0,0,0);
	m_cAverColSel = RGB(0,0,0);
	m_nTrialStyle = RGB(0,0,0);
	m_nAverStyle = RGB(0,0,0);
	m_nTrialLineWidth = RGB(0,0,0);
	m_nAverLineWidth = RGB(0,0,0);
 
}

C2DGraph::~C2DGraph()
{
	for (int i=0; i < Series.GetSize(); i++)
		{
			((_2DGRAPH_PARAM_STRUCT *)(Series[i]))->data->RemoveAll();
			delete ((_2DGRAPH_PARAM_STRUCT *)(Series[i]))->data;
			delete Series[i];
		}
	Series.RemoveAll();
}


void C2DGraph::SetCanvas(CDC *dc,RECT &r)
{
   //DC = dc;
   Area =  r;
}

int C2DGraph::AddSerie(_2DGRAPH_PARAM_STRUCT * s)
{
  
  return Series.Add(s);
}

void C2DGraph::SetSerieAttributes(int idx, _2DGRAPH_PARAM_STRUCT *s)
{

}

void C2DGraph::Draw(CDC* DC)
{
	//CFVProfileApp* app = (CFVProfileApp*)AfxGetApp();	
	CFont TitleFont;
	TitleFont.CreatePointFont(100, "Arial Bold", DC);
	
	CFont AxisFont;
	AxisFont.CreatePointFont(70, "Arial", DC);

	DC->SelectObject(&TitleFont);
	DC->SetTextColor(RGB(100,0,0));
	DC->DrawText(sLabel,&Area,DT_CENTER);
	
	RECT g;
	g.left = Area.left+30;
	g.right = Area.right-20;
	g.top = Area.top + 20;
	g.bottom = Area.bottom-20;

	CPen borderPen,dataPen;
	borderPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 50));
	
	DC->SelectObject(&borderPen);
	DC->Rectangle(&g);

	// Determine maximum length of Series
	float MaxTime=0.0;
/*
	for (int i=0; i< Series.GetSize(); i++)
	{
		_2DGRAPH_PARAM_STRUCT * s = (_2DGRAPH_PARAM_STRUCT *)Series[i];
		if (!s) {continue;}
				
		if (!s->microns) 
			if (MaxTime < float(s->data->GetSize())*s->dt) MaxTime = float(s->data->GetSize())*s->dt;

	}
	*/
	//yTo/=5.;
	if (xFrom < 0.) xFrom = 0;
	if (xTo <= 0.) xTo = MaxTime;
	//if (yTo <= 0.) yTo = 10.;


	float dy = 	float(g.bottom - g.top) / float(yTo-yFrom);

	float w =  (float) (g.right- g.left);
	float w2 =  (float) (g.bottom- g.top);

	float t =  xTo - xFrom;
	float t2 =  yTo - yFrom;
	
	float beta = t / w;
	float gamma = t2 / w2;


	//Draw Grid
	
	
	CPen axisPen; axisPen.CreatePen(PS_SOLID,1,RGB(150,150,150));
	DC->SelectObject(&axisPen);
	DC->SelectObject(&AxisFont);
	
	CString s; s.Format("%5.1f",float(xTo));
	CSize c = DC->GetOutputTextExtent(s);
	float x = float(xGrid) / beta;
	int ff = c.cx/x +1;
	
	//if ((xTo-xFrom)/xGrid > 100) xGrid = (xTo-xFrom)/100.0;
	xGrid = (xTo-xFrom)/20.0;
	for (float t_step=xFrom+xGrid, t_l=0; t_step < xTo; t_step+= xGrid, t_l++)
	{
		x = float(t_step-xFrom) / beta;
		DC->MoveTo(g.left+x, g.top );
		DC->LineTo(g.left+x, g.bottom);
		s.Format("%5.1f",float(t_step*1E3));
		c = DC->GetOutputTextExtent(s);
		if (! (int(t_l)%ff))
		{
			DC->TextOut(g.left+x-c.cx/2,g.bottom+2,s);
			DC->MoveTo(g.left+x, g.bottom );
			DC->LineTo(g.left+x, g.bottom-4);
		}
	}
	
	CPen NormPen; NormPen.CreatePen(PS_SOLID,2,RGB(255,255,0));
    CPen *oldPen; oldPen = DC->SelectObject(&NormPen);
   
   
	
	DC->SelectObject(oldPen);

	//if ((yTo-yFrom)/yGrid > 100) yGrid = (yTo-yFrom)/100.0;
	yGrid = (yTo-yFrom)/10.0;
	for (float y_step=yFrom+yGrid; y_step < yTo; y_step+= yGrid)
	{
		int y = float(y_step-yFrom) / gamma;
		DC->MoveTo(g.left, g.bottom-y );
		DC->LineTo(g.right, g.bottom-y);
		s.Format("%5.1f",float(y_step));
		c = DC->GetOutputTextExtent(s);
		DC->TextOut(g.left - c.cx-2,g.bottom-y-c.cy/2,s);
	}

	axisPen.DeleteObject();
	
	//Perform actual drawing
	for (int i=0; i< Series.GetSize(); i++)
	{
		_2DGRAPH_PARAM_STRUCT * s = (_2DGRAPH_PARAM_STRUCT *)Series[i];
                //if (!s) {continue;}
				if (!s->b_visible) continue;
                COLORREF col;
				int lw;
				if (s->MouseOver)
				{
					col = RGB(255,0,0);
					lw = 3;
				}
				else
				{
					col = RGB(0,0,0);
					lw = 1;
				}

                float l =  s->data->GetSize();
                float dt = s->dt;
                float ppp = l/w;
         
				//drawing axies
                
                DC->SetROP2(R2_COPYPEN );
                dataPen.CreatePen(PS_SOLID,lw,col);
                CPen * oldPen = DC->SelectObject(&dataPen);
		
		//float mx = s->pSI->max_roi[s->nROI].MaxX;
		//float my = s->pSI->max_roi[s->nROI].MaxY;

		if (s->c_type==GT_DOT)
		{
			int d = s->n_LineWidth/2.;
			for (float x = 0.0; x< g.right- g.left; x+=1.0 )
			{
				int idx =xFrom/dt + x	* beta / dt;
				
				if ((idx>=l )||(idx<0 )) continue;
				
				float dp = (*s->data)[idx];
				
				//if (dp<yFrom) dp = yFrom;
				//if (dp>yTo) dp = yTo;
				
				int xo = g.left+x;
				float yo =  dy*(dp-yFrom);

				if ((yo >=w2 )||(yo <= 0.0 )) continue;

				for (int k1=-d; k1<=d;k1++)
					for (int k2=-d; k2<=d;k2++)
						DC->SetPixel( xo+k1,g.bottom - yo+k2,col);
				
				
				
			}
		}
		if (s->c_type==GT_LINE)
		{
			
			bool f=false;
			for (float x = 0.0; x< g.right-g.left; x+=1.0 )
			{
				int idx =(xFrom)/dt- s->nTick + x* beta / dt;

				if ((idx>=l )||(idx<0 )) continue;
				
				float dp = (*s->data)[idx];
				
				//if (dp<yFrom) dp = yFrom;
				//if (dp>yTo) dp = yTo;
				
				float y = dy*(dp-yFrom);
				
				if ((y >=w2 )||(y <=0.0 )) continue;
				
				if (!f)	
					{DC->MoveTo(g.left+x, g.bottom - y);f=true;}
				else
					DC->LineTo(g.left+x, g.bottom - y);
				
				
			}

		  

			
		}	
		
		DC->SelectObject(oldPen);
		dataPen.DeleteObject();
		
	}
	
}

int C2DGraph::AddXMark(float X, CString &Label)
{
 return 0;
}

bool C2DGraph::OnPoint(CPoint &p)
{
	bool res = false;
	if ((p.x>Area.left)&&(p.x<Area.right)&&
		(p.y>Area.top)&&(p.y<Area.bottom))
		res = true;
	else
		res = false;
	
	return res;
}
