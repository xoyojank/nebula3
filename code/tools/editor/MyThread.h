#pragma once



// CMyThread
#include "n3viewerapp.h"

class CMyThread : public CWinThread
{
	DECLARE_DYNCREATE(CMyThread)

protected:
	CMyThread();           // protected constructor used by dynamic creation
	virtual ~CMyThread();

public:
	Toolkit::N3ViewerApp m_gameApp;

public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


