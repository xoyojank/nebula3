// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "ChildView.h"
//#include "MyThread.h"
#include "n3viewerapp.h"
#include "PaneDialog.h"
#include "ExplorerView.h"


class CMainFrame : public CXTPFrameWnd
{

public:
	CMainFrame();
protected:
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:
	BOOL OnIdle(LONG lCount);
	//CXTPDockingPane* GetActivePane();
	//CWnd* GetActivePaneView();
	void  SetActive(BOOL bActive) { m_bIsActive = bActive; m_bRedrawScreen = bActive; };

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CXTPStatusBar  m_wndStatusBar;
	CChildView    m_wndView;
	CXTPDockingPaneManager m_paneManager;
	CXTPOfficeBorder<CStatic> m_wndOptions;
	CXTPOfficeBorder<CEdit> m_wndProperties;
	//CWinThread* m_wndThread;
	BOOL  m_bIsActive;    
	BOOL  m_bRedrawScreen;
	CPaneDialog m_dlgPane;
	CExplorerView m_wndExplorerView;
	

public:
	Editor::N3ViewerApp m_gameApp;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnClose();
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnImportCollada();
};
