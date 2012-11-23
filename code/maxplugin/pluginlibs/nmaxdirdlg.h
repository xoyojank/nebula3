#ifndef N_MAXDIRDLG_H
#define N_MAXDIRDLG_H
//-----------------------------------------------------------------------------
/**
    @class nMaxDirDlg
    @ingroup NebulaMaxExport2Contrib

    @brief directory setting dialog
*/
class nMaxDlg;

//-----------------------------------------------------------------------------
class nMaxDirDlg : public nMaxDlg
{
public:
    nMaxDirDlg(WORD resID, HINSTANCE hInst, HWND hwndParent);
    virtual ~nMaxDirDlg();

    //virtual int DoModal();

protected:
    virtual BOOL OnCommand(int wParamLow, int wParamHigh, long lParam);

    virtual void OnInitDialog();

    bool OnOK();
    void OnSelHomeDir();
    void OnSetDefaultAssigns();
    void OnSetDefaultPaths();

//    void CreateDefaultIniFile(const char* filename);
    
};

void ShowDirSettingDlg();
//-----------------------------------------------------------------------------
#endif
