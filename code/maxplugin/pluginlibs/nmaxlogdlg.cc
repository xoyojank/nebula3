//-----------------------------------------------------------------------------
//  nmaxlogdlg.cc
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "stdneb.h"
//#include <windows.h>

#include "export2/nmax.h"
#include "export2/nmaxexport2.h"
#include "../res/nmaxtoolbox.h"

#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"
#include "export2/nmaxoptions.h"

using namespace Util;
nMaxLogDlg* nMaxLogDlg::Singleton = 0;

static const char * sColorTables[NumVerboseLevel + 1] = {
    "{\\colortbl;\\red255\\green0\\blue0;}",
    "{\\colortbl;\\red255\\green126\\blue0;}",
    "{\\colortbl;\\red0\\green0\\blue0;}",
    "{\\colortbl;\\red0\\green0\\blue0;}",
    "{\\colortbl;\\red0\\green0\\blue0;}",
    NULL
};

//-----------------------------------------------------------------------------
/**
*/
nMaxLogDlg* nMaxLogDlg::Instance()
{
    if (0 == Singleton)
    {
        // get instance hanlde of dll
        HINSTANCE hInstance = maxExportInterfaceClassDesc2.HInstance();

        // get window handle of 3dsmax
        Interface* intf = GetCOREInterface();
        HWND parentHWnd = intf->GetMAXHWnd();

        return n_new(nMaxLogDlg(IDD_LOG_DLG, hInstance, parentHWnd));
    }

    return Singleton;
}

//-----------------------------------------------------------------------------
/**
*/
nMaxLogDlg::nMaxLogDlg(WORD resID, HINSTANCE hInst, HWND hwndParent) :
    nMaxDlg(resID, hInst, hwndParent),
    endWait(false)
{
    n_assert (0 == Singleton);
    Singleton = this;
}

//-----------------------------------------------------------------------------
/**
*/
nMaxLogDlg::~nMaxLogDlg()
{
    n_assert (Singleton);

    Singleton = 0;
}

//-----------------------------------------------------------------------------
/**
*/
BOOL nMaxLogDlg::OnCommand(int wParamLow, int wParamHigh, long lParam)
{
    switch(wParamLow)
    {
    case IDOK:
        if (OnOK())
        {
            EndDialog(this->hWnd, IDOK);
        }
        break;
    default:
        break;
    }
    return TRUE;
}

//-----------------------------------------------------------------------------
/**
*/
bool nMaxLogDlg::OnOK()
{
    this->endWait = true;
    return true;
}

//-----------------------------------------------------------------------------
/**
*/
void nMaxLogDlg::OnClose()
{
    this->endWait = true;
}

//-----------------------------------------------------------------------------
/**
*/
void nMaxLogDlg::LogMessage(int logLevel, const char* msg, ...)
{
    n_assert(Singleton);

    int verboseLevel = nMaxOptions::Instance()->GetVerboseLevel();

    // If the log level is higher than the specified verbose level of the option,
    // we do not put this log on log dialog.
    if (logLevel > verboseLevel)
        return;

    char buffer[2 * 1024];
    va_list argList;
    va_start(argList, msg);
    vsnprintf(buffer, sizeof(buffer), msg, argList);
    va_end(argList);

    String rtfStr = "{\\rtf ";
    rtfStr += sColorTables[logLevel];
    rtfStr += "{\\par\\pard \\cf1";
    rtfStr += buffer;
    rtfStr += "}}\n";

    this->LogMessage(rtfStr);
}

//-----------------------------------------------------------------------------
/**
*/
void nMaxLogDlg::LogMessage(const String &str)
{
    n_assert(Singleton);

    HWND hWndEdit = GetDlgItem(this->hWnd, IDC_LOG_EDIT);

    SendMessage(hWndEdit, EM_SETSEL, -1, -1);
    SendMessage(hWndEdit, EM_REPLACESEL, 0/*no undo*/, (LPARAM)str.Get()/*new text to replace*/);
    SendMessage(hWndEdit, EM_SCROLL, SB_PAGEDOWN, 0/*unused*/);
}

//-----------------------------------------------------------------------------
/**
*/
void nMaxLogDlg::ClearMessage()
{
    n_assert(Singleton);

    HWND hWndEdit = GetDlgItem(this->hWnd, IDC_LOG_EDIT);

    SendMessage(hWndEdit, EM_SETSEL, 0, -1);
    SendMessage(hWndEdit, EM_REPLACESEL, 0/*no undo*/, (LPARAM)""/*new text to replace*/);
    SendMessage(hWndEdit, EM_SCROLL, SB_PAGEDOWN, 0/*unused*/);
}

//-----------------------------------------------------------------------------
/**
    Suspend message loop until 'ok' button is pressed.
*/
void nMaxLogDlg::Wait()
{
    this->endWait = false;

    for(;;)
    {
        for (int i=0; i<10; i++)
        {
            MSG msg;
            if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (this->endWait)
            break;

        Sleep(100);
    }
}
