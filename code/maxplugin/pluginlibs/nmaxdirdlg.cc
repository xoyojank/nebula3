//-----------------------------------------------------------------------------
//  nmaxdirdlg.cc
//
//  (C)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#define _WIN32_DCOM
#include "stdneb.h"
//#include <windows.h>

#include <shlobj.h>
#include <objbase.h>
#include "util/string.h"
#include "export2/nmax.h"
#include "export2/nmaxexport2.h"

#include "../res/nmaxtoolbox.h"

#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxdirdlg.h"
//#include "util/String.h"
//#include "kernel/nfileserver2.h"
//#include "kernel/nfile.h"
//#include "misc/niniprefserver.h"
using namespace Util;

//-----------------------------------------------------------------------------
/**
*/
nMaxDirDlg::nMaxDirDlg(WORD resID, HINSTANCE hInst, HWND hwndParent) : 
    nMaxDlg(resID, hInst, hwndParent)
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxDirDlg::~nMaxDirDlg()
{
}

//-----------------------------------------------------------------------------
/**
    called when the dialog get the message WM_INITDIALOG.
*/
void nMaxDirDlg::OnInitDialog()
{
    String homeDir;
    String binaryPath;

    String animsAssign;
    String gfxlibAssign;
    String guiAssign;
    String lightsAssign;
    String meshesAssign;
    String shadersAssign;
    String texturesAssign;

    String animsPath;
    String gfxlibPath;
    String guiPath;
    String lightsPath;
    String meshesPath;
    String shadersPath;
    String texturesPath;

  //  nFileServer2* fileServer = nFileServer2::Instance();

  //  String iniFilename;
  //  iniFilename += GetCOREInterface()->GetDir(APP_PLUGCFG_DIR);
  //  iniFilename += "\\";
  //  iniFilename += N_MAXEXPORT_INIFILE;

  //  // check the .ini file exist in 3dsmax plugcfg directory.
  //  if (!fileServer->FileExists(iniFilename))
  //  {
  //      // the .ini file does not exist, so make new one.
  //      //nFile* file = fileServer->NewFileObject();
  //      //file->Open(iniFilename.Get(), "w");
  //      //file->Close();
  //      //file->Release();

		////FIXME: put an messago box 
		//// try to find the .ini file in the 'scripts\nebula' dirtectory. e.g.) d:\3dsmax\Scripts\nebula2
		//iniFilename = "";
  //      iniFilename += GetCOREInterface()->GetDir(APP_SCRIPTS_DIR);
  //      iniFilename += "\\";
		//iniFilename += "nebula2\\";
  //      iniFilename += N_MAXEXPORT_INIFILE;
  //      if (!fileServer->FileExists(iniFilename))
  //      {
		//	n_message("Cannot find %s file in $3dsmax/plugcfg or $3dsmax/scripts/nebula2 directory.", 
		//		iniFilename.Get());

		//	return;
		//}
  //  }

  //  // read values from .ini file and specify those to dialog controls.
  //  nIniPrefServer* iniFile = (nIniPrefServer*)nKernelServer::Instance()->New("niniprefserver", "/iniprefsrv");
  //  iniFile->SetFileName(iniFilename);
  //  iniFile->SetSection("GeneralSettings");
  //  iniFile->SetDefault(".");

  //  homeDir = iniFile->ReadString("HomeDir");

  //  //projDir = iniFile->ReadString("ProjDir");

  //  binaryPath = iniFile->ReadString("BinaryPath") ;

  //  iniFile->SetDefault(N_MAXEXPORT_ANIMS_ASSIGN);
  //  animsAssign = iniFile->ReadString("AnimsAssign");

  //  iniFile->SetDefault(N_MAXEXPORT_GFXLIB_ASSIGN);
  //  gfxlibAssign = iniFile->ReadString("GfxlibAssign");

  //  iniFile->SetDefault(N_MAXEXPORT_GUI_ASSIGN);
  //  guiAssign = iniFile->ReadString("GuiAssign") ;

  //  iniFile->SetDefault(N_MAXEXPORT_LIGHTS_ASSIGN);
  //  lightsAssign = iniFile->ReadString("LightsAssign");

  //  iniFile->SetDefault(N_MAXEXPORT_MESHES_ASSIGN);
  //  meshesAssign = iniFile->ReadString("MeshesAssign");

  //  iniFile->SetDefault(N_MAXEXPORT_SHADERS_ASSIGN);
  //  shadersAssign = iniFile->ReadString("ShadersAssign");

  //  iniFile->SetDefault(N_MAXEXPORT_TEXTURES_ASSIGN);
  //  texturesAssign = iniFile->ReadString("TexturesAssign");

  //  iniFile->SetDefault(N_MAXEXPORT_ANIMS_PATH);
  //  animsPath = iniFile->ReadString("AnimsPath");

  //  iniFile->SetDefault(N_MAXEXPORT_GFXLIB_PATH);
  //  gfxlibPath = iniFile->ReadString("GfxlibPath");

  //  iniFile->SetDefault(N_MAXEXPORT_GUI_PATH);
  //  guiPath = iniFile->ReadString("GuiPath");

  //  iniFile->SetDefault(N_MAXEXPORT_LIGHTS_PATH);
  //  lightsPath = iniFile->ReadString("LightsPath");

  //  iniFile->SetDefault(N_MAXEXPORT_MESHES_PATH);
  //  meshesPath = iniFile->ReadString("MeshesPath");

  //  iniFile->SetDefault(N_MAXEXPORT_SHADERS_PATH);
  //  shadersPath = iniFile->ReadString("ShadersPath");

  //  iniFile->SetDefault(N_MAXEXPORT_TEXTURES_PATH);
  //  texturesPath = iniFile->ReadString("TexturesPath");

  //  iniFile->Release();

  //  SetDlgItemText(hWnd, IDC_EXT_DIRNAME,     homeDir.Get());    // home dir
  //  SetDlgItemText(hWnd, IDC_EXT_BINARY_PATH, binaryPath.Get()); // binary path

  //  // assigns
  //  SetDlgItemText(hWnd, IDC_EXT_ASSIGN_ANIM,     animsAssign.Get());    // anim
  //  SetDlgItemText(hWnd, IDC_EXT_ASSIGN_GFXLIB,   gfxlibAssign.Get());   // gfx
  //  SetDlgItemText(hWnd, IDC_EXT_ASSIGN_GUI,      guiAssign.Get());      // gui
  //  SetDlgItemText(hWnd, IDC_EXT_ASSIGN_LIGHTS,   lightsAssign.Get());   // lights
  //  SetDlgItemText(hWnd, IDC_EXT_ASSIGN_MESHES,   meshesAssign.Get());   // meshes
  //  SetDlgItemText(hWnd, IDC_EXT_ASSIGN_SHADERS,  shadersAssign.Get());  // shaders
  //  SetDlgItemText(hWnd, IDC_EXT_ASSIGN_TEXTURES, texturesAssign.Get()); // textures

  //  // paths
  //  SetDlgItemText(hWnd, IDC_EXT_PATH_ANIM,     animsPath.Get());
  //  SetDlgItemText(hWnd, IDC_EXT_PATH_GFXLIB,   gfxlibPath.Get());
  //  SetDlgItemText(hWnd, IDC_EXT_PATH_GUI,      guiPath.Get());
  //  SetDlgItemText(hWnd, IDC_EXT_PATH_LIGHTS,   lightsPath.Get());
  //  SetDlgItemText(hWnd, IDC_EXT_PATH_MESHES,   meshesPath.Get());
  //  SetDlgItemText(hWnd, IDC_EXT_PATH_SHADERS,  shadersPath.Get());
  //  SetDlgItemText(hWnd, IDC_EXT_PATH_TEXTURES, texturesPath.Get());
}

//-----------------------------------------------------------------------------
/**
    called when the dialog get the message WM_COMMAND.
*/
BOOL nMaxDirDlg::OnCommand(int wParamLow, int wParamHigh, long lParam)
{
    switch(wParamLow)
    {
    case IDOK:
        if (OnOK())
        {
            EndDialog(this->hWnd, IDOK);
        }
        break;

    case IDCANCEL:
        EndDialog(this->hWnd, IDCANCEL);
        break;

    case IDC_EXP_FILESEL:
        OnSelHomeDir();
        break;

    // 'Set Default' button in 'Assigns'
    case IDC_SET_DEFAULT_ASSIGNS:
        OnSetDefaultAssigns();
        break;

    // 'Set Default' button in 'Paths'
    case IDC_SET_DEFAULT_PATHS:
        OnSetDefaultPaths();
        break;

    default:
        break;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/**
    Message handler function for 'ok' button.
*/
bool nMaxDirDlg::OnOK()
{
    String homeDir;
    String binaryPath;

    String animsAssign;
    String gfxlibAssign;
    String guiAssign;
    String lightsAssign;
    String meshesAssign;
    String shadersAssign;
    String texturesAssign;

    String animsPath;
    String gfxlibPath;
    String guiPath;
    String lightsPath;
    String meshesPath;
    String shadersPath;
    String texturesPath;

    const int BUFSIZE = 512;
    char str[BUFSIZE];

    // retrieves value from dialog controls.
    GetDlgItemText(hWnd, IDC_EXT_DIRNAME,         str, BUFSIZE); homeDir = str;
    GetDlgItemText(hWnd, IDC_EXT_BINARY_PATH,     str, BUFSIZE); binaryPath = str;

    GetDlgItemText(hWnd, IDC_EXT_ASSIGN_ANIM,     str, BUFSIZE); animsAssign = str;
    GetDlgItemText(hWnd, IDC_EXT_ASSIGN_GFXLIB,   str, BUFSIZE); gfxlibAssign = str;
    GetDlgItemText(hWnd, IDC_EXT_ASSIGN_GUI,      str, BUFSIZE); guiAssign = str;
    GetDlgItemText(hWnd, IDC_EXT_ASSIGN_LIGHTS,   str, BUFSIZE); lightsAssign = str;
    GetDlgItemText(hWnd, IDC_EXT_ASSIGN_MESHES,   str, BUFSIZE); meshesAssign = str;
    GetDlgItemText(hWnd, IDC_EXT_ASSIGN_SHADERS,  str, BUFSIZE); shadersAssign = str;
    GetDlgItemText(hWnd, IDC_EXT_ASSIGN_TEXTURES, str, BUFSIZE); texturesAssign = str;

    GetDlgItemText(hWnd, IDC_EXT_PATH_ANIM,     str, BUFSIZE); animsPath = str;
    GetDlgItemText(hWnd, IDC_EXT_PATH_GFXLIB,   str, BUFSIZE); gfxlibPath = str;
    GetDlgItemText(hWnd, IDC_EXT_PATH_GUI,      str, BUFSIZE); guiPath = str;
    GetDlgItemText(hWnd, IDC_EXT_PATH_LIGHTS,   str, BUFSIZE); lightsPath = str;
    GetDlgItemText(hWnd, IDC_EXT_PATH_MESHES,   str, BUFSIZE); meshesPath = str;
    GetDlgItemText(hWnd, IDC_EXT_PATH_SHADERS,  str, BUFSIZE); shadersPath = str;
    GetDlgItemText(hWnd, IDC_EXT_PATH_TEXTURES, str, BUFSIZE); texturesPath = str;

  //  // check the specified assigns and paths are valid.
  //  if (!nFileServer2::Instance()->DirectoryExists(homeDir))
  //  {
  //      // specified home directory does not exist.
  //      MessageBox(this->hWnd, "The specified 'Home' assign directory does not exist.", "Error", MB_OK);
  //      return false;
  //  }
  //  else
  //  {
		//nFileServer2* fileServer = nFileServer2::Instance();

  //      // write the values to .ini file.
  //      String iniFilename;
  //      iniFilename += GetCOREInterface()->GetDir(APP_PLUGCFG_DIR);
  //      iniFilename += "\\";
  //      iniFilename += N_MAXEXPORT_INIFILE;

  //      // check the .ini file exist in 3dsmax plugin directory.
  //      if (!fileServer->FileExists(iniFilename))
  //      {
		//    // try to find the .ini file in the 'scripts\nebula' dirtectory. e.g.) d:\3dsmax\Scripts\nebula2
		//    iniFilename = "";
  //          iniFilename += GetCOREInterface()->GetDir(APP_SCRIPTS_DIR);
  //          iniFilename += "\\";
		//    iniFilename += "nebula2\\";
  //          iniFilename += N_MAXEXPORT_INIFILE;
		//}

  //      nIniPrefServer* iniFile = (nIniPrefServer*)nKernelServer::Instance()->New("niniprefserver", "/iniprefsrv");
  //      iniFile->SetFileName(iniFilename);
  //      iniFile->SetSection("GeneralSettings");

  //      iniFile->WriteString("HomeDir", homeDir);
  //      //iniFile->WriteString("ProjDir", projDir);
  //      iniFile->WriteString("BinaryPath", binaryPath) ;
  //      iniFile->WriteString("AnimsAssign", animsAssign);
  //      iniFile->WriteString("GfxlibAssign", gfxlibAssign);
  //      iniFile->WriteString("GuiAssign", guiAssign) ;
  //      iniFile->WriteString("LightsAssign", lightsAssign);
  //      iniFile->WriteString("MeshesAssign", meshesAssign);
  //      iniFile->WriteString("ShadersAssign", shadersAssign);
  //      iniFile->WriteString("TexturesAssign", texturesAssign);
  //      iniFile->WriteString("AnimsPath", animsPath);
  //      iniFile->WriteString("GfxlibPath", gfxlibPath);
  //      iniFile->WriteString("GuiPath", guiPath);
  //      iniFile->WriteString("LightsPath", lightsPath);
  //      iniFile->WriteString("MeshesPath", meshesPath);
  //      iniFile->WriteString("ShadersPath", shadersPath);
  //      iniFile->WriteString("TexturesPath", texturesPath);

  //      iniFile->Release();
  //  }

    return true;
}

//-----------------------------------------------------------------------------
/**
    Shows up file select browser to specify nebula2 installed directory.

    FIXME: shown up the dialog has ugly white background. 
           some setting of it seems be wrong.
*/
void nMaxDirDlg::OnSelHomeDir()
{
    // Retrieve the task memory allocator.
    LPMALLOC pIMalloc;
    if (!SUCCEEDED(::SHGetMalloc(&pIMalloc)))
    {
        return;
    }

    char szBuf[MAX_PATH];

    // Initialize a BROWSEINFO structure,
    BROWSEINFO brInfo;
    ::ZeroMemory(&brInfo, sizeof(brInfo));

    brInfo.hwndOwner      = this->hWnd;
    brInfo.pidlRoot       = NULL;
    brInfo.pszDisplayName = szBuf;
    brInfo.lpszTitle      = "Select Nebula2 installed directory";

    // only want folders (no printers, etc.)
    brInfo.ulFlags = BIF_RETURNONLYFSDIRS;

    // Display the browser.
    ITEMIDLIST* browseList = NULL;
    browseList = ::SHBrowseForFolder(&brInfo);

    // if the user selected a folder . . .
    if (browseList)
    {
        // Convert the item ID to a pathname, 
        if(::SHGetPathFromIDList(browseList, szBuf))
        {
            String homeDir;
            homeDir.Format("%s", szBuf);
            SetDlgItemText(hWnd, IDC_EXT_DIRNAME, homeDir.Get());
        }

        // Free the PIDL
        pIMalloc->Free(browseList);
    }
    else
    {
        *szBuf=_T('\0');
    }

    // Decrement ref count on the allocator
    pIMalloc->Release();
}

//-----------------------------------------------------------------------------
/**
    Specifies 'assign' settings to default values.
*/
void nMaxDirDlg::OnSetDefaultAssigns()
{
    SetDlgItemText(hWnd, IDC_EXT_ASSIGN_ANIM,     N_MAXEXPORT_ANIMS_ASSIGN);
    SetDlgItemText(hWnd, IDC_EXT_ASSIGN_GFXLIB,   N_MAXEXPORT_GFXLIB_ASSIGN);
    SetDlgItemText(hWnd, IDC_EXT_ASSIGN_GUI,      N_MAXEXPORT_GUI_ASSIGN);
    SetDlgItemText(hWnd, IDC_EXT_ASSIGN_LIGHTS,   N_MAXEXPORT_LIGHTS_ASSIGN);
    SetDlgItemText(hWnd, IDC_EXT_ASSIGN_MESHES,   N_MAXEXPORT_MESHES_ASSIGN);
    SetDlgItemText(hWnd, IDC_EXT_ASSIGN_SHADERS,  N_MAXEXPORT_SHADERS_ASSIGN);
    SetDlgItemText(hWnd, IDC_EXT_ASSIGN_TEXTURES, N_MAXEXPORT_TEXTURES_ASSIGN);
}

//-----------------------------------------------------------------------------
/**
    Specifies 'path' settings to default values.
*/
void nMaxDirDlg::OnSetDefaultPaths()
{
    SetDlgItemText(hWnd, IDC_EXT_PATH_ANIM,     N_MAXEXPORT_ANIMS_PATH);
    SetDlgItemText(hWnd, IDC_EXT_PATH_GFXLIB,   N_MAXEXPORT_GFXLIB_PATH);
    SetDlgItemText(hWnd, IDC_EXT_PATH_GUI,      N_MAXEXPORT_GUI_PATH);
    SetDlgItemText(hWnd, IDC_EXT_PATH_LIGHTS,   N_MAXEXPORT_LIGHTS_PATH);
    SetDlgItemText(hWnd, IDC_EXT_PATH_MESHES,   N_MAXEXPORT_MESHES_PATH);
    SetDlgItemText(hWnd, IDC_EXT_PATH_SHADERS,  N_MAXEXPORT_SHADERS_PATH);
    SetDlgItemText(hWnd, IDC_EXT_PATH_TEXTURES, N_MAXEXPORT_TEXTURES_PATH);
}

//-----------------------------------------------------------------------------
/**
    Defined for scripting interface. See nmaxdlgscript.cc
*/
void ShowDirSettingDlg()
{
    // get instance hanlde of dll
    HINSTANCE hInstance = maxExportInterfaceClassDesc2.HInstance();

    // get window handle of 3dsmax
    Interface* intf = GetCOREInterface();
    HWND parentHWnd = intf->GetMAXHWnd();

    // show directory setting dialog up.
    nMaxDirDlg dirDlg(IDD_DIR_SETTING, hInstance, parentHWnd);
    int ret = dirDlg.DoModal();
}
