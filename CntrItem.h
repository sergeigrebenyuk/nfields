// CntrItem.h : interface of the CLensCntrItem class
//

#if !defined(AFX_CNTRITEM_H__4421C6C9_BBF5_4D53_8EF9_20DD1C26D5AF__INCLUDED_)
#define AFX_CNTRITEM_H__4421C6C9_BBF5_4D53_8EF9_20DD1C26D5AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLensDoc;
class CLensView;

class CLensCntrItem : public COleClientItem
{
	DECLARE_SERIAL(CLensCntrItem)

// Constructors
public:
	CLensCntrItem(CLensDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CLensDoc* GetDocument()
		{ return (CLensDoc*)COleClientItem::GetDocument(); }
	CLensView* GetActiveView()
		{ return (CLensView*)COleClientItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLensCntrItem)
	public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();
	protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CLensCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__4421C6C9_BBF5_4D53_8EF9_20DD1C26D5AF__INCLUDED_)
