//-----------------------------------------------------------------------------
//  nmaxdlg.cc
//
//  (c)2004 Kim, Hyoun Woo.
//-----------------------------------------------------------------------------
//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
//#include <commctrl.h>
//#include "kernel/ntypes.h"
#include "stdneb.h"
#include "core/debug.h"
#include "pluginlibs/nmaxdlg.h"

//-----------------------------------------------------------------------------
/**
*/
nMaxDlg::nMaxDlg(WORD resID, HINSTANCE hInst, HWND hwndParent) :
    hWnd(0)
{
    this->resourceID = resID;
    this->hInstance  = hInst;
    this->hWndParent = hwndParent;
}

//-----------------------------------------------------------------------------
/**
*/
nMaxDlg::~nMaxDlg()
{
    if (this->hWnd)
    {
        Destroy();
    }
}

//-----------------------------------------------------------------------------
/**
    Create dialog.
*/
void nMaxDlg::Create()
{
    HWND hwnd = CreateDialogParam(this->hInstance,
                                  MAKEINTRESOURCE(this->resourceID),
                                  this->hWndParent,
                                  &DlgProc,
                                  (LPARAM)this);
    if (0 == hwnd)
    {
        DWORD dwError = GetLastError();
    }

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    this->hWnd = hwnd;
    this->bModal = FALSE;
}

//-----------------------------------------------------------------------------
/**
*/
void nMaxDlg::Destroy()
{
    ShowWindow(this->hWnd, SW_HIDE);
    DestroyWindow(this->hWnd);
    this->hWnd = 0;
}

//-----------------------------------------------------------------------------
/**
    Create modal dialog.
*/
int nMaxDlg::DoModal()
{
    int result;
    result = DialogBoxParam(this->hInstance, 
                            MAKEINTRESOURCE(this->resourceID),
                            this->hWnd, 
                            &DlgProc,
                            (LPARAM)this);
    if (result < 0)
    {
        DWORD dwError = GetLastError();
    }
    this->bModal = TRUE;

    return result;
}

//-----------------------------------------------------------------------------
/**
*/
void nMaxDlg::OnInitDialog()
{
    // empty.
}

//-----------------------------------------------------------------------------
/**
*/
void nMaxDlg::OnClose()
{
    // empty.
}

//-----------------------------------------------------------------------------
/**
*/
void nMaxDlg::OnDestroy()
{
    // empty.
    this->hWnd = 0;
}

//-----------------------------------------------------------------------------
/**
*/
BOOL nMaxDlg::OnCommand(int lowParam, int highParam, long lParam)
{
    return FALSE;
}

//-----------------------------------------------------------------------------
/**
*/
BOOL nMaxDlg::OnMessage(UINT msg, int lowParam, int highParam, long lParam)
{
    return FALSE;
}

//-----------------------------------------------------------------------------
/**
    Dialog window procedure
*/
BOOL CALLBACK nMaxDlg::DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int id = LOWORD(wParam);
    int code = HIWORD(wParam);

    nMaxDlg* dlg = (nMaxDlg*)GetWindowLong(hwnd, GWL_USERDATA);

    switch(msg)
    {
    case WM_INITDIALOG:
        dlg = (nMaxDlg*)lParam;
        dlg->SetHWnd(hwnd);
        SetWindowLong(hwnd, GWL_USERDATA, (long)lParam);
        dlg->OnInitDialog();
        UpdateWindow(hwnd);
        break;

    case WM_COMMAND:
        if (dlg)
        {
            return dlg->OnCommand(id, code, lParam);
        }
        break;

    case WM_CLOSE:
        if (dlg)
        {
            dlg->OnClose();
        }

        EndDialog(hwnd, IDCANCEL);
        break;

    case WM_DESTROY:
        if (dlg)
        {
            dlg->OnDestroy();
        }
        break;

    default:
        if (dlg)
        {
            return dlg->OnMessage(msg, id, code, lParam);
        }
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}


//-----------------------------------------------------------------------------
/** 
    Use this function to prevent from calling EndDialog to a modeless dialog box
*/
BOOL nMaxDlg::EndDialog(HWND hDlg, INT_PTR nResult)
{
    nMaxDlg* dlg = (nMaxDlg*)GetWindowLong(hDlg, GWL_USERDATA);
    if (dlg == 0)
    {
        n_assert(0 && "Invalid nMaxDlg");
        return ::EndDialog(hDlg, nResult);
    }

    if (dlg->bModal)
    {
        return ::EndDialog(hDlg, nResult);
    }

    dlg->Destroy();
    return TRUE;
}
