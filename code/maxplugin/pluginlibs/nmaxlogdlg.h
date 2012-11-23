//-----------------------------------------------------------------------------
//   nmaxlogdlg.h
//
//   (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXLOGDLG_H
#define N_MAXLOGDLG_H
//-----------------------------------------------------------------------------
/**
    @class nMaxLogDlg
    @ingroup NebulaMaxExport2Contrib

    @brief Put log message out when exporting.
           Call nMaxLogDlg::Create() to create log dialog.

*/
//#include "util/nstring.h"
#include "util/string.h"

class nMaxDlg;

//-----------------------------------------------------------------------------
class nMaxLogDlg : public nMaxDlg
{
public:
    static nMaxLogDlg* Instance();

    virtual ~nMaxLogDlg();

    void LogMessage(int verboseLevel, const char* msg, ...);
	void LogMessage(const Util::String &str);
    void ClearMessage();

    void Wait();

protected:
    virtual BOOL OnCommand(int wParamLow, int wParamHigh, long lParam);
    bool OnOK();
    void OnClose();

    bool endWait;

private:
    nMaxLogDlg(WORD resID, HINSTANCE hInst, HWND hwndParent);    
    static nMaxLogDlg* Singleton;

};
//-----------------------------------------------------------------------------
#define n_maxlog nMaxLogDlg::Instance()->LogMessage

#endif
