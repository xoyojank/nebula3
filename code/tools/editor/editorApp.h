// xtremetools2.h : main header file for the xtremetools2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
//#include "n3viewerapp.h"


// Cxtremetools2App:
// See xtremetools2.cpp for the implementation of this class
//

class Cxtremetools2App : public CWinApp
{
public:
	Cxtremetools2App();

//public:
//	BOOL  m_bIsActive;    
//	BOOL  m_bRedrawScreen;
//	BOOL  m_bShutDown;
//	void  SetActive(BOOL bActive) { m_bIsActive = bActive; };
//	void  ShutDown() { m_bShutDown = TRUE; };
//public:
//	Toolkit::N3ViewerApp m_gameApp;

public:

// Overrides
public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()	
};

extern Cxtremetools2App theApp;