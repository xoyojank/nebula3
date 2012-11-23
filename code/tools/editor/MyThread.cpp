// MyThread.cpp : implementation file
//

#include "stdneb.h"
#include "MyThread.h"
// CMyThread

IMPLEMENT_DYNCREATE(CMyThread, CWinThread)

CMyThread::CMyThread()
{
}

CMyThread::~CMyThread()
{
}

BOOL CMyThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	if( !this->m_gameApp.Open())
		return FALSE;

	this->m_gameApp.LoadModel("mdl:jiulou.n3");
	return TRUE;
}

BOOL CMyThread::OnIdle(LONG lCount)
{
	this->m_gameApp.Run();
	return TRUE;
}

int CMyThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	this->m_gameApp.Close();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMyThread, CWinThread)
END_MESSAGE_MAP()


// CMyThread message handlers
