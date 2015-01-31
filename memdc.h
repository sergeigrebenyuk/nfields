// memdc.h

#ifndef _MEMDC_H_
#define _MEMDC_H_
#include <wingdi.h>
//#include "DJDoc.h"
//class CDJDoc;
class CMemDC : public CDC {
protected://TRK
   CBitmap  m_bitmap;       // Offscreen bitmap
   CBitmap* m_oldBitmap;    // bitmap originally found in CMemDC
   CDC*     m_pDC;          // Saves CDC passed in constructor
   CRect    m_rect;         // Rectangle of drawing area.
   BOOL     m_bMemDC;       // TRUE if CDC really is a Memory DC.
	BYTE    Alpha;
    
   void Construct(CDC* pDC)
   {
        ASSERT(pDC != NULL); 

        // Some initialization
        m_pDC = pDC;
        m_oldBitmap = NULL;
        m_bMemDC = !pDC->IsPrinting();

        if (m_bMemDC) {
            // Create a Memory DC
            CreateCompatibleDC(pDC);
            pDC->LPtoDP(&m_rect);

            if (!m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height()))
			{
				//((CDJDoc*)GetDocument())->_D("GDI error.\n"); 
				return;
			}
				
            m_oldBitmap = SelectObject(&m_bitmap);
            
            SetMapMode(pDC->GetMapMode());
            pDC->DPtoLP(&m_rect);
            SetWindowOrg(m_rect.left, m_rect.top);
        } else {
            // Make a copy of the relevent parts of the current DC for printing
            m_bPrinting = pDC->m_bPrinting;
            m_hDC       = pDC->m_hDC;
            m_hAttribDC = pDC->m_hAttribDC;
        }
		Alpha = 150;
        // Fill background 
        FillSolidRect(m_rect, pDC->GetBkColor());
    }

// TRK begin
public:
   CMemDC(CDC* pDC                  ) : CDC() { pDC->GetClipBox(&m_rect); Construct(pDC); }
   CMemDC(CDC* pDC, const RECT& rect) : CDC() { m_rect = rect           ; Construct(pDC); }
   CMemDC(CDC* pDC, BYTE al)		  : CDC() { Construct(pDC); Alpha = al;}

// TRK end
    
   virtual ~CMemDC()
   {        
        if (m_bMemDC) {
            // Copy the offscreen bitmap onto the screen.
			/*
			BLENDFUNCTION bf;
			bf.AlphaFormat = AC_DST_NO_ALPHA;
			bf.BlendFlags = 0;
			bf.BlendOp = AC_SRC_OVER;
			bf.SourceConstantAlpha = Alpha;
			*/
		   
            m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
                this, m_rect.left, m_rect.top, SRCCOPY);            
				
            /*if (!AlphaBlend(
  m_pDC->m_hDC,                 // handle to destination DC
  m_rect.left,             // x-coord of upper-left corner
  m_rect.top,            // y-coord of upper-left corner
  m_rect.Width(),              // destination width
  m_rect.Height(),             // destination height
  m_hDC,                  // handle to source DC
  m_rect.left,              // x-coord of upper-left corner
  m_rect.top,             // y-coord of upper-left corner
  m_rect.Width(),               // source width
  m_rect.Height(),              // source height
  bf  // alpha-blending function
)) TRACE0("AlphaBlend failed\n");
								  */
            //Swap back the original bitmap.
            if (m_oldBitmap) SelectObject(m_oldBitmap);        
        } else {
            // All we need to do is replace the DC with an illegal value,
            // this keeps us from accidently deleting the handles associated with
            // the CDC that was passed to the constructor.            
            m_hDC = m_hAttribDC = NULL;
        }    
    }
    
    // Allow usage as a pointer    
    CMemDC* operator->() 
    {
        return this;
    }    

    // Allow usage as a pointer    
    operator CMemDC*() 
    {
        return this;
    }
};

class CxMemDC : public CMemDC
{
// Attributes
protected:
   BOOL m_bWantBitmap;
public:
   CxMemDC(CDC* pDC, const RECT& rect) : CMemDC(pDC,          rect), m_bWantBitmap(FALSE) { }
   CxMemDC(CDC* pDC                  ) : CMemDC(pDC               ), m_bWantBitmap(FALSE) { }
   CxMemDC(const SIZE& size          ) : CMemDC(CDC::FromHandle(::GetDC(NULL)), CRect(0, 0, size.cx, size.cy)), m_bWantBitmap(TRUE)
   { 
      ::ReleaseDC(NULL, m_pDC->m_hDC); // m_pDC is temporary MFC object, dispose of m_hDC
      m_pDC = NULL, m_bMemDC = FALSE;
   }

// Operations
public:
   operator CBitmap&(void) { return m_bitmap; }
   operator HBITMAP (void) { return m_bitmap; }
   HBITMAP DetachBitmap(void)
   { 
      SelectObject(m_oldBitmap);
      m_oldBitmap = NULL;
      return (HBITMAP)m_bitmap.Detach(); 
   }

// Implementation
public:
   virtual ~CxMemDC()
   {
      if (m_bWantBitmap)
      {
         // m_pDC is NULL so we need to clean up before ~CMemDC
         ASSERT(!m_bMemDC);
         SelectObject(m_oldBitmap);
		   ::DeleteDC(Detach());
      }
   }
};

#endif // _MEMDC_H_
