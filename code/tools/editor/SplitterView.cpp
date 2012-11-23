// SplitterWindowView.cpp : implementation of the CSplitterWindowView class
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2009 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdneb.h"

#include "xtremetools2.h"
#include "SplitterDoc.h"
#include "SplitterView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowView

IMPLEMENT_DYNCREATE(CSplitterView, CView)

BEGIN_MESSAGE_MAP(CSplitterView, CView)
	//{{AFX_MSG_MAP(CSplitterWindowView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowView construction/destruction

CSplitterView::CSplitterView()
{
	// TODO: add construction code here

}

CSplitterView::~CSplitterView()
{
}

BOOL CSplitterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowView drawing

void CSplitterView::OnDraw(CDC*)
{
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowView printing

BOOL CSplitterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSplitterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSplitterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSplitterView::OnMouseMove(UINT nFlags, CPoint point)
{
	BOOL bActive = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowView diagnostics

#ifdef _DEBUG
void CSplitterView::AssertValid() const
{
	CView::AssertValid();
}

void CSplitterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSplitterDoc* CSplitterView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSplitterDoc)));
	return (CSplitterDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowView message handlers

