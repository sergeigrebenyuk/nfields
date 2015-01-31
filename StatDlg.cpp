// StatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "lens.h"
#include "StatDlg.h"
#include "lensdoc.h"
#include ".\statdlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatDlg dialog


CStatDlg::CStatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatDlg::IDD, pParent)
	, m_dT(0)
	, m_edb(0)
	, m_ede(0)
	, m_edx(0)
	, m_ewb(0)
	, m_ewe(0)
	, m_ewx(0)
{
	//{{AFX_DATA_INIT(CStatDlg)
	m_size = 0.0f;
	//m_value = 0.0f;
	m_X = 0;
	m_Y = 0;
	m_SX = _T("");
	m_SY = _T("");
	//}}AFX_DATA_INIT
}

CStatDlg::CStatDlg(CLensDoc* doc, CWnd* pParent /*=NULL*/)
	: CDialog(CStatDlg::IDD, pParent)
{
	pDoc=doc;
}


void CStatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatDlg)
	DDX_Text(pDX, IDC_EDITSIZE, m_size);
	DDV_MinMaxFloat(pDX, m_size, 1.f, 5000.f);
	//DDX_Text(pDX, IDC_EDITVALUE, m_value);
	DDX_Text(pDX, IDC_EDITX, m_X);
	DDV_MinMaxLong(pDX, m_X, 1, 5000);
	DDX_Text(pDX, IDC_EDITY, m_Y);
	DDV_MinMaxLong(pDX, m_Y, 1, 5000);
	DDX_Text(pDX, IDC_STATICX, m_SX);
	DDX_Text(pDX, IDC_STATICY, m_SY);
	//}}AFX_DATA_MAP
	
	
	DDX_Text(pDX, IDC_EDITDA1, m_edb);
	DDV_MinMaxFloat(pDX, m_edb, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDITDA2, m_ede);
	DDV_MinMaxFloat(pDX, m_ede, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDITDX, m_edx);
	DDV_MinMaxFloat(pDX, m_edx, 0.f, 500.f);
	DDX_Text(pDX, IDC_EDITWA1, m_ewb);
	DDV_MinMaxFloat(pDX, m_ewb, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDITWA2, m_ewe);
	DDV_MinMaxFloat(pDX, m_ewe, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDITWX, m_ewx);
	DDV_MinMaxFloat(pDX, m_ewx, 0.f, 500.f);
	DDX_Text(pDX, IDC_EDITDT, m_dT);
	DDV_MinMaxFloat(pDX, m_ewx, 0.f, 500.f);
}


BEGIN_MESSAGE_MAP(CStatDlg, CDialog)
	//{{AFX_MSG_MAP(CStatDlg)
	ON_EN_CHANGE(IDC_EDITSIZE, OnChangeEditsize)
	ON_EN_CHANGE(IDC_EDITVALUE, OnChangeEditvalue)
	ON_BN_CLICKED(IDC_RESTART, OnRestart)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDITWX, OnEnChangeEditwx)
	ON_EN_CHANGE(IDC_EDITDX, OnEnChangeEditdx)
	ON_EN_CHANGE(IDC_EDITDT, OnEnChangeEditdt)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatDlg message handlers

void CStatDlg::SetStatus()
{
	float factor = 1.;

	switch (pDoc->field->SimState)
	{
	case STAT_INIT: 
		((CStatic*)GetDlgItem(IDC_STATUS))->SetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_INITVALUES)) );
		factor = 1E3;
		break;
	case STAT_SIM: ((CStatic*)GetDlgItem(IDC_STATUS))->SetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SIMULATION)) ); break;
	case STAT_WALLS: ((CStatic*)GetDlgItem(IDC_STATUS))->SetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_WALLS)) );break;
	case STAT_TONIC: 
		((CStatic*)GetDlgItem(IDC_STATUS))->SetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_TONICINPUT)) );
		factor = 1E12;
		break;
	
	}

	m_size = pDoc->field->ChangeSize;	
	char s[25];	
	_gcvt(pDoc->field->ChangeValue*factor,7,s);
	SetDlgItemText(IDC_EDITVALUE, s);
	
	m_X = pDoc->field->X;
	m_Y = pDoc->field->Y;
	m_dT= pDoc->field->dT*1E3;
	m_SX.Format("%i",m_X);
	m_SY.Format("%i",m_Y);
	float m_ed_noise,m_ew_noise;
	pDoc->field->GetAfferentFieldParams(m_edb,m_ede,m_ewb,m_ewe,m_ed_noise,m_ew_noise,m_ewx);
	m_edx = m_ewx;

	UpdateData(FALSE);

	
}

BOOL CStatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetStatus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatDlg::OnChangeEditsize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	int min = min(pDoc->field->X,pDoc->field->Y);
	if (m_size > min) m_size = min;
	pDoc->field->ChangeSize = m_size; 
	
	
	
	
}

void CStatDlg::OnChangeEditvalue() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	//UpdateData(TRUE);
	char s[10]; GetDlgItemText(IDC_EDITVALUE,s,10);
	float v = atof(s);

switch (pDoc->field->SimState)
	{
	case STAT_INIT: 
		pDoc->field->ChangeValue = v*1E-3;
		break;
	case STAT_SIM: break;
	case STAT_WALLS: break;
	case STAT_TONIC: 
		pDoc->field->ChangeValue = v*1E-12;
		break;
	}
}

void CStatDlg::OnOK()
{

}

void CStatDlg::OnCancel()
{

}

void CStatDlg::OnRestart() 
{
	UpdateData(TRUE);
	m_SX.Format("%i",m_X);
	m_SY.Format("%i",m_Y);
	UpdateData(FALSE);
	
	pDoc->field->X = m_X;
	pDoc->field->Y = m_Y;
	pDoc->field->dT = m_dT * 1E-3;
	float m_ed_noise=0,m_ew_noise=0;
	pDoc->field->SetAfferentFieldParams(m_edb,m_ede,m_ewb,m_ewe,m_ed_noise,m_ew_noise,m_ewx);

	pDoc->RestartSimulation();

}

void CStatDlg::OnEnChangeEditwx()
{
	UpdateData(TRUE);
	m_edx = m_ewx;	
	UpdateData(FALSE);
}

void CStatDlg::OnEnChangeEditdx()
{
	UpdateData(TRUE);
	m_ewx = m_edx;	
	UpdateData(FALSE);
}

void CStatDlg::OnEnChangeEditdt()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
