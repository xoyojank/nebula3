// SelectTreeCtrl.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2011 Codejock Software, All Rights Reserved.
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
#include "SelectTreeCtrl.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectTreeCtrl

CSelectTreeCtrl::CSelectTreeCtrl()
: m_pParentDlg(NULL)
{
}

CSelectTreeCtrl::~CSelectTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(CSelectTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CSelectTreeCtrl)
	ON_WM_NCPAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectTreeCtrl message handlers

void CSelectTreeCtrl::OnNcPaint() 
{
	Default();

	if (m_pParentDlg && m_pParentDlg->m_pTreeFocus == this)
	{
		CWindowDC dc(this);
		CXTPWindowRect rc(this);
		rc.OffsetRect(-rc.TopLeft());
		
		dc.Draw3dRect(rc, RGB(0,0,255),RGB(0,0,255));
		rc.DeflateRect(1,1);
		dc.Draw3dRect(rc, RGB(0,0,255),RGB(0,0,255));
	}
	
	// Do not call CTreeCtrl::OnNcPaint() for painting messages
}

void CSelectTreeCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CTreeCtrl::OnSetFocus(pOldWnd);

	if (::IsWindow(m_pParentDlg->GetSafeHwnd()))
		m_pParentDlg->SetTreeFocus(this);
}

void CSelectTreeCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CTreeCtrl::OnKillFocus(pNewWnd);

	if (::IsWindow(m_pParentDlg->GetSafeHwnd()))
		m_pParentDlg->SetTreeFocus(this);
}

void CSelectTreeCtrl::PreSubclassWindow() 
{
	CTreeCtrl::PreSubclassWindow();

	m_pParentDlg = DYNAMIC_DOWNCAST(CTreeCtrlDlg, GetParent());
	ASSERT_VALID(m_pParentDlg);

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_TREE_IMAGES);

	m_imageList.Create(16, 16, ILC_COLOR24|ILC_MASK, 2, 1);
	m_imageList.Add(&bitmap, RGB(0,255,0));
	SetImageList(&m_imageList, TVSIL_NORMAL);
}

void CSelectTreeCtrl::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	UNREFERENCED_PARAMETER(pNMTreeView);
	
	// TODO: Add your control notification handler code here
	m_pParentDlg->EnableProperties();
	
	*pResult = 0;
}
