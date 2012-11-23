// xtremetools2.cpp : Defines the class behaviors for the application.
//

//#include "stdafx.h"
#include "stdneb.h"
#include "editorApp.h"
#include "MainFrm.h"
#include <graphics/graphicsinterface.h>

// Cxtremetools2App
using namespace Graphics;

BEGIN_MESSAGE_MAP(Cxtremetools2App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &Cxtremetools2App::OnAppAbout)
END_MESSAGE_MAP()


// Cxtremetools2App construction

Cxtremetools2App::Cxtremetools2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	//m_bIsActive = TRUE;
	//m_bShutDown = FALSE;
}


// The one and only Cxtremetools2App object

Cxtremetools2App theApp;


// Cxtremetools2App initialization

BOOL Cxtremetools2App::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,	NULL);
	
	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	//// call DragAcceptFiles only if there's a suffix
	////  In an SDI app, this should occur after ProcessShellCommand
	//this->m_gameApp.SetParentWindow( (void*)m_pMainWnd->GetSafeHwnd() );
	//if( !this->m_gameApp.Open())
	//	return FALSE;
	return TRUE;
}

BOOL Cxtremetools2App::OnIdle(LONG lCount)
{
	//this->m_gameApp.Run();
	CMainFrame* pFrame = (CMainFrame*)m_pMainWnd;
	pFrame->OnIdle(lCount);
	return TRUE;
}

//int Cxtremetools2App::ExitInstance()
//{
//	//this->m_gameApp.Close();
//	return CWinApp::ExitInstance();
//}

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void Cxtremetools2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// Cxtremetools2App message handlers

