// MainFrm.cpp : implementation of the CMainFrame class
//

//#include "stdafx.h"
#include "stdneb.h"
#include "editorApp.h"

#include "MainFrm.h"
#include "io/assignregistry.h"
#include "input/win32/win32inputdisplayeventhandler.h"
#include "DAE2Nebula3Writer.h"

using namespace CoreGraphics;
using namespace IO;


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CXTPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CXTPFrameWnd)
	ON_WM_CREATE()
	//ON_WM_SETFOCUS()
	//ON_WM_KILLFOCUS()
	ON_WM_CLOSE()
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_40017, &CMainFrame::OnImportCollada)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT uHideCmds[] =
{
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_bIsActive = true;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Initialize the command bars
	if (!InitCommandBars())
		return -1;

	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}

	// Add the menu bar
	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(
		_T("Menu Bar"), IDR_MAINFRAME);
	if(pMenuBar == NULL)
	{
		TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}

	// Create ToolBar
	CXTPToolBar* pToolBar = (CXTPToolBar*)
		pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pToolBar || !pToolBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Set Office XP Theme
	CXTPPaintManager::SetTheme(xtpThemeOffice2003);

	// Hide array of commands
	pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));

	// Set "Always Show Full Menus" option to the FALSE
	pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);

	// Load the previous state for toolbars and menus.
	LoadCommandBars(_T("CommandBars"));

	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);
	// Set Visual Studio .NET / Office XP Theme
	m_paneManager.SetTheme(xtpPaneThemeVisualStudio2003);

	// Create docking panes.
	CXTPDockingPane* pwndPane1 = m_paneManager.CreatePane(
		IDR_PANE_OPTIONS, CRect(0, 0,200, 120), xtpPaneDockRight);
	CXTPDockingPane* pwndPane2 = m_paneManager.CreatePane(
		IDR_PANE_PROPERTIES, CRect(0, 0,200, 120), xtpPaneDockBottom, pwndPane1);

	// Set the icons for the docking pane tabs.
	int nIDIcons[] = {IDR_PANE_OPTIONS, IDR_PANE_PROPERTIES};
		m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDIcons,
		_countof(nIDIcons), RGB(0, 255, 0));

	// Load the previous state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	if (layoutNormal.Load(_T("NormalLayout")))
	{
		m_paneManager.SetLayout(&layoutNormal);
	}

	//m_wndThread = AfxBeginThread(RUNTIME_CLASS(CMyThread));
	HWND hWnd = m_wndView.GetSafeHwnd();
	this->m_gameApp.SetParentWindow( (void*)hWnd );
	if( !this->m_gameApp.Open())
		return FALSE;

	//this->m_gameApp.LoadModel("mdl:examples\tong.n3");
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CXTPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();	
}

void CMainFrame::OnKillFocus(CWnd* pNewWnd)
{		
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//CXTPDockingPane* CMainFrame::GetActivePane()
//{
//	return m_paneManager.GetActivePane();
//}
//
//CWnd* CMainFrame::GetActivePaneView()
//{
//	CXTPDockingPane* pActivePane = GetActivePane();
//	if (pActivePane)
//		return pActivePane->GetChild();
//	return NULL;
//}

BOOL CMainFrame::OnIdle(LONG lCount)
{	
	if( m_bIsActive )
	{
		this->m_gameApp.Run();		
	}
	return TRUE;
}

void CMainFrame::OnClose()
{	
	this->m_gameApp.Close();
	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));

	// Save the current state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	m_paneManager.GetLayout(&layoutNormal);
	layoutNormal.Save(_T("NormalLayout"));

	CFrameWnd::OnClose();
}


void CMainFrame::OnCustomize()
{
	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars != NULL)
	{
		// Instanciate the customize dialog object.
		CXTPCustomizeSheet dlg(pCommandBars);

		// Add the keyboard page to the customize dialog.
		CXTPCustomizeKeyboardPage pageKeyboard(&dlg);
		dlg.AddPage(&pageKeyboard);
		pageKeyboard.AddCategories(IDR_MAINFRAME);

		// Add the options page to the customize dialog.
		CXTPCustomizeOptionsPage pageOptions(&dlg);
		dlg.AddPage(&pageOptions);

		// Add the commands page to the customize dialog.
		CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
		pCommands->AddCategories(IDR_MAINFRAME);

		// Use the command bar manager to initialize the
		// customize dialog.
		pCommands->InsertAllCommandsCategory();
		pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
		pCommands->InsertNewMenuCategory();

		// Dispaly the dialog.
		dlg.DoModal();
	}
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
		if (!pPane->IsValid())
		{			
			switch (pPane->GetID())
			{
			case IDR_PANE_PROPERTIES:
				{
					//if (m_wndProperties.GetSafeHwnd() == 0)
					//{
					//	m_wndProperties.Create(WS_CHILD|
					//		ES_AUTOVSCROLL|ES_MULTILINE,
					//		CRect(0, 0, 0, 0), this, 0);
					//}
					//pPane->Attach(&m_wndProperties);
					if (!::IsWindow(m_dlgPane.m_hWnd))
					{
						m_dlgPane.Create(IDD_PROPPAGE_MEDIUM, this);
					}
					pPane->Attach(&m_dlgPane);
					break;
				}
			case IDR_PANE_OPTIONS:
				{
					//if (m_wndOptions.GetSafeHwnd() == 0)
					//{
					//	m_wndOptions.Create(_T("\n\nOptions"),
					//		WS_CHILD|WS_CLIPCHILDREN|
					//		WS_CLIPSIBLINGS|SS_CENTER,
					//		CRect(0, 0, 0, 0), this, 0);
					//}
					//pPane->Attach(&m_wndOptions);
					pPane->Attach(m_wndExplorerView.CreatePane(this));
					break;
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

void CMainFrame::OnFileOpen()
{
	// TODO: Add your command handler code here	
	m_gameApp.DiscardModels();
	CFileDialog fileDlg(TRUE);
	URI localPath = AssignRegistry::Instance()->ResolveAssigns("mdl:");
	if( !IO::IoServer::Instance()->DirectoryExists(localPath.LocalPath()) )
	{
		return;
	}
	Util::String strPath = localPath.LocalPath();
	strPath.ReplaceChars("/",'\\');
	fileDlg.m_ofn.lpstrInitialDir = strPath.AsCharPtr();
 	if( fileDlg.DoModal() == IDOK)
	{
		CString filename = fileDlg.GetPathName();
		filename  = filename.MakeLower();
		strPath.ToLower();
		if( filename.Find(strPath.AsCharPtr())!=-1)
		{
			filename.Replace(strPath.AsCharPtr(),"mdl:");
			m_gameApp.LoadModel(filename.GetBuffer());
		}
		else
		{
			/// ERROR!
		}
	}
}

void CMainFrame::OnImportCollada()
{
	// TODO: Add your command handler code here
	m_gameApp.DiscardModels();
	CFileDialog fileDlg(TRUE);
	URI localPath = AssignRegistry::Instance()->ResolveAssigns("mdl:");
	if( !IO::IoServer::Instance()->DirectoryExists(localPath.LocalPath()) )
	{
		return;
	}
	Util::String strPath = localPath.LocalPath();

	if( fileDlg.DoModal() == IDOK)
	{
		CString filePath = fileDlg.GetPathName();
		filePath  = filePath.MakeLower();
		COLLADABU::String inputFile = filePath.GetBuffer();
		
		Util::String outputFilename = fileDlg.GetFileName().GetBuffer();
		outputFilename.StripFileExtension();
		outputFilename += ".n3";
		COLLADABU::String outputFile = strPath.AsCharPtr();
		outputFile					+= "//";
		outputFile                  += outputFilename.AsCharPtr();

		Models::Nebula3Writer writer( COLLADABU::URI::nativePathToUri(inputFile), COLLADABU::URI::nativePathToUri(outputFile));
		if( writer.write() )
		{
			Util::String strModelfile = "mdl:";
			strModelfile			 += outputFilename;
			m_gameApp.LoadModel(strModelfile);
		}
		else
		{
			/// import ERROR
		}
	}
}
