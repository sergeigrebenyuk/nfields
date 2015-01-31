#if !defined(AFX_STATDLG_H__8DD4F54C_2319_4F32_8EFE_82C52A75FA89__INCLUDED_)
#define AFX_STATDLG_H__8DD4F54C_2319_4F32_8EFE_82C52A75FA89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatDlg dialog

class CStatDlg : public CDialog
{
// Construction
public:
	void OnCancel();
	void OnOK();
	void SetStatus();
	CStatDlg(CWnd* pParent = NULL);   // standard constructor
	CStatDlg(CLensDoc* doc, CWnd* pParent = NULL);   // standard constructor
	CLensDoc* pDoc;
// Dialog Data
	//{{AFX_DATA(CStatDlg)
	enum { IDD = IDD_STATDLG };
	float	m_size;
	//float	m_value;
	long	m_X;
	long	m_Y;
	CString	m_SX;
	CString	m_SY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditsize();
	afx_msg void OnChangeEditvalue();
	afx_msg void OnRestart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	float m_dT;
	float m_edb;
	float m_ede;
	float m_edx;
	float m_ewb;
	float m_ewe;
	float m_ewx;
	afx_msg void OnEnChangeEditwx();
	afx_msg void OnEnChangeEditdx();
	afx_msg void OnEnChangeEditdt();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATDLG_H__8DD4F54C_2319_4F32_8EFE_82C52A75FA89__INCLUDED_)
