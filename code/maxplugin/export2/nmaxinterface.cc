#include "stdneb.h"
#include "export2/nmax.h"
//#include "kernel/ntypes.h"
#include "export2/nmaxinterface.h"
#include "core/debug.h"
#include "memory/memory.h"

nMaxInterface* nMaxInterface::Singleton = 0;

//-----------------------------------------------------------------------------
/**
*/
nMaxInterface::nMaxInterface() :
    maxInterface(0)
{
    n_assert(Singleton == 0);
    Singleton = this;
}

//-----------------------------------------------------------------------------
/**
*/
nMaxInterface::~nMaxInterface()
{
    Singleton = 0;
}

//-----------------------------------------------------------------------------
/**
    DO NOT FORGET TO CALL 'n_delete' for intance of nMaxOption 
    when application exit.
*/
nMaxInterface* nMaxInterface::Instance()
{
    //n_assert(Singleton);
    //return Singleton;

    // nMaxInterface is not nRoot derived class, so we make its instance 
    // directly with n_new if it is not created before.
    if (0 == Singleton)
    {
        n_new(nMaxInterface);
    }

    return Singleton;
}

//-----------------------------------------------------------------------------
/**
*/
bool nMaxInterface::Create(Interface * intf)
{
    n_assert(intf);

    this->maxInterface = intf;

    return true;
}

// This function passed to ProgressStart() is a dummy function, 
// and the processing is done outside it
DWORD WINAPI fn(LPVOID arg)
{
    return(0);
}

//-----------------------------------------------------------------------------
/**
*/
bool nMaxInterface::ProgressStart(TCHAR* title)
{
    return (this->maxInterface->ProgressStart(title, TRUE, fn, this) ? true : false);
}

//-----------------------------------------------------------------------------
/**
*/
void nMaxInterface::ProgressUpdate(int percent)
{
    this->maxInterface->ProgressUpdate(percent);
}

//-----------------------------------------------------------------------------
/**
    @return 
*/
bool nMaxInterface::CheckUserCancel()
{
    if (this->maxInterface->GetCancel()) 
    {
        int ret = MessageBox(this->maxInterface->GetMAXHWnd(), _T("Really Cancel"),
            _T("Question"), MB_ICONQUESTION | MB_YESNO);

        if (ret == IDYES)
        {
            return true;
        }
        else 
        if (ret == IDNO)
        {
            this->maxInterface->SetCancel(FALSE);
        }
    }

    return false;
}

//-----------------------------------------------------------------------------
/**
*/
void nMaxInterface::ProgressEnd()
{
    this->maxInterface->ProgressEnd();
}
