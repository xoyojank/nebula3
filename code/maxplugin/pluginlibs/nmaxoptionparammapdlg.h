//------------------------------------------------------------------------------
//  nmaxoptionparammapdlg.h
//
//  (c)2004 Kim, Hyoun Woo.
//------------------------------------------------------------------------------
#ifndef N_MAXOPTIONPARAMMAPDLG_H
#define N_MAXOPTIONPARAMMAPDLG_H
//------------------------------------------------------------------------------
/**
    @class nMaxOptionParamMapDlg
    @ingroup NebulaMaxExport2Contrib

    @brief Nebula export option dialog in utility panel.
*/
#include "export2/nmax.h"
#include <iparamm.h>

//------------------------------------------------------------------------------
class nMaxOptionParamMapDlg : public ParamMapUserDlgProc
{
public:
    // supported script server types.
    enum {
        Tcl    = 0,
        Python = 1,
        Lua    = 2,
        Ruby   = 3,
        Binary = 4,
        NumScriptServers = 5,
    };

    nMaxOptionParamMapDlg();
    virtual ~nMaxOptionParamMapDlg();

    // @name 3dsmax functions which should be defined.
    // @{
    BOOL DlgProc(TimeValue t, IParamMap *map, HWND hWnd, UINT msg,
                 WPARAM wParam, LPARAM lParam);
    void DeleteThis() { delete this; }
    // @}

private:
    void InitDialog(HWND hwnd);
    void OnCommand(HWND hwnd, WORD highParam, WORD lowParam);

    bool IsChecked(HWND hwnd, int id);

    void OnVertexComponent(HWND hwnd);

    void OnMeshFileType(HWND hwnd);
    void OnAnimFileType(HWND hwnd);

	void OnUseCharacter3SkinAnimator(HWND hwnd);

    void OnHiddenObject(HWND hwnd);

    /// @name script server combobox
    /// @{
    void OnSelectedScriptServer(HWND hwnd);
    const char* GetScriptServer(const char* item);
    int GetItemIndexFromScript(const char* name);
    /// @}

    /// @name verbose level combobox
    /// @{
    void OnSelectedVerboseLevel(HWND hwnd);
    /// @}

private:
    /// spinner control for setting of geometry scaling.
    ISpinnerControl* spinGeomScale;
    /// spinner control for setting of maximum joint palette.
    ISpinnerControl* spinMaxJointPalette;
    /// spinner control for setting of weight threshold.
    ISpinnerControl* spinWeightThreshold;
    /// spinner control for setting global animation sample rate.
    ISpinnerControl* spinSampleRate;


};
//------------------------------------------------------------------------------
#endif
