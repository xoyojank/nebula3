//------------------------------------------------------------------------------
//  nmaxvieweroptions.cc
//
//  (C)2005 Kim, Hyoun Woo
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "export2/nmax.h"
#include "export2/nmaxoptions.h"
#include "export2/nmaxutil.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"
#include "pluginlibs/nmaxvieweroptions.h"
#include "util/string.h"

using namespace Util;
//#include "kernel/nkernelserver.h"
//#include "kernel/nfileserver2.h"
//#include "kernel/nfile.h"
//#include "misc/niniprefserver.h"

//------------------------------------------------------------------------------
/**
*/
nMaxViewerOptions::nMaxViewerOptions() :
    viewerType(Default),
    usePreview(false),
    executable("nviewer.exe")
{
}

//------------------------------------------------------------------------------
/**
*/
nMaxViewerOptions::~nMaxViewerOptions()
{
}

//------------------------------------------------------------------------------
/**
*/
bool nMaxViewerOptions::Read()
{
  //  String iniFilename;
  //  iniFilename += GetCOREInterface()->GetDir(APP_PLUGCFG_DIR);
  //  iniFilename += "\\";
  //  iniFilename += N_MAXEXPORT_INIFILE;

  //  nFileServer2* fileServer = nFileServer2::Instance(); 

  //  // check the .ini file exist in 3dsmax plugin directory.
  //  if (!fileServer->FileExists(iniFilename))
  //  {
	 //   // try to find the .ini file in the 'scripts\nebula' dirtectory. e.g.) d:\3dsmax\Scripts\nebula2
	 //   iniFilename = "";
  //      iniFilename += GetCOREInterface()->GetDir(APP_SCRIPTS_DIR);
  //      iniFilename += "\\";
	 //   iniFilename += "nebula2\\";

		//String iniFileDir = iniFilename;

  //      iniFilename += N_MAXEXPORT_INIFILE;

		//if (!fileServer->FileExists(iniFilename))
		//{
		//	// the .ini file does not exist, so make new one.
		//	//nFile* file = fileServer->NewFileObject();
		//	//file->Open(iniFilename.Get(), "w");
		//	//file->Close();
		//	//file->Release();
		//	
		//	n_message("%s file does not exist in %s directory.\n", iniFilename.Get(), iniFileDir.Get());

		//	return false;
		//}
  //  }

  //  // read viewer options.
  //  bool result = false;
  //  switch(viewerType)
  //  {
  //  case Default:
  //      result = ReadDefaultViewerOptions(iniFilename, "DefaultViewerSetting");
  //      break;
  //  case Custom:
  //      result = ReadCustomViewerOptions(iniFilename, "CustomViewerSetting");
  //      break;
  //  }

  //  if (!result)
  //  {
  //      n_maxlog(Warning, "Error on reading viewer options.");
  //      return false;
  //  }

  //  // specifies scene file name.
  //  if (!this->sceneFilename.IsEmpty())
  //  {
  //      this->arguments += "-view ";
  //      this->arguments += this->GetScenePath();
  //      this->arguments += " ";
  //  }
  //  else
  //  {
  //      n_maxlog(Warning, "No scene filename was specified.");
  //      return false;
  //  }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
String
nMaxViewerOptions::GetScenePath() const
{
    String path;
    if (this->sceneDir.IsValid())
    {
        String sceneDir = this->sceneDir;
        path += nMaxUtil::RelacePathToAssign(nMaxUtil::Gfx, sceneDir, this->sceneFilename);
    }
    else
    {
        path += nMaxOptions::Instance()->GetGfxLibAssign();
        path += this->sceneFilename;
    }
    return path;
}


//------------------------------------------------------------------------------
/**
    Read viewer options for default viewer.

    @param iniFilename .ini filename which contains default viewer options.
    @param sectionName section name in .ini file. "DefaultViewerSetting" is used 
                       for a custom viewer.
*/
bool nMaxViewerOptions::ReadDefaultViewerOptions(const String &iniFilename, 
                                                 const String &sectionName)
{
    //String scriptServer;
    //String x,y,w,h;
    //bool fullscreen, alwaysontop;
    //String projDir;
    //String featureset;
    //String gfxServer;
    //String sceneServer;
    //String stage;

    ////nIniFile iniFile(iniFilename, sectionName);
    //nIniPrefServer* iniFile = (nIniPrefServer*)nKernelServer::Instance()->New("niniprefserver", "/iniprefsrv");
    //iniFile->SetFileName(iniFilename);
    //iniFile->SetSection("DefaultViewerSetting");

    //iniFile->SetDefault("ntclserver");
    //scriptServer = iniFile->ReadString("-scriptserver");
    //if (!scriptServer.IsEmpty())
    //{
    //    this->arguments += "-scriptserver ";
    //    this->arguments += scriptServer;
    //    this->arguments += " ";
    //}

    //iniFile->SetDefault("false");
    //fullscreen = iniFile->ReadBool("-fullscreen");
    //if (fullscreen)
    //    this->arguments += "-fullscreen ";

    //iniFile->SetDefault("false");
    //alwaysontop = iniFile->ReadBool("-alwaysontop");
    //if (alwaysontop)
    //    this->arguments += "-alwaysontop ";

    //iniFile->SetDefault("0");
    //x = iniFile->ReadString("-x");
    //this->arguments += "-x";
    //this->arguments += " ";
    //this->arguments += x;
    //this->arguments += " ";

    //iniFile->SetDefault("0");
    //y = iniFile->ReadString("-y");
    //this->arguments += "-y";
    //this->arguments += " ";
    //this->arguments += y;
    //this->arguments += " ";

    //iniFile->SetDefault("640");
    //w = iniFile->ReadString("-w");
    //this->arguments += "-w ";
    //this->arguments += w;
    //this->arguments += " ";

    //iniFile->SetDefault("480");
    //h = iniFile->ReadString("-h");
    //this->arguments += "-h ";
    //this->arguments += h;
    //this->arguments += " ";

    //// project directory arg.
    //iniFile->SetDefault("");
    //projDir = iniFile->ReadString("-projdir");
    //this->arguments += "-projdir ";
    //if (!projDir.IsEmpty())
    //{
    //    this->arguments += projDir; 
    //}
    //else
    //{
    //    this->arguments += nMaxOptions::Instance()->GetHomePath();
    //}
    //this->arguments += " ";

    //iniFile->SetDefault("nsceneserver");
    //sceneServer = iniFile->ReadString("-sceneserver");
    //this->arguments += "-sceneserver ";
    //this->arguments += sceneServer;
    //this->arguments += " ";

    //iniFile->SetDefault("home:export/gfxlib/stdlight.n2");
    //stage = iniFile->ReadString("-stage");
    //this->arguments += "-stage ";
    //this->arguments += stage;
    //this->arguments += " ";

    //// gfxserver arg.
    //iniFile->SetDefault("");
    //gfxServer = iniFile->ReadString("-gfxserver");
    //if (!gfxServer.IsEmpty())
    //{
    //    this->arguments += "-gfxserver ";
    //    this->arguments += gfxServer;
    //    this->arguments += " ";
    //}

    //// featureset arg.
    //iniFile->SetDefault("");
    //featureset = iniFile->ReadString("-featureset");
    //if (!featureset.IsEmpty())
    //{
    //    this->arguments += "-featureset ";
    //    this->arguments += featureset;
    //    this->arguments += " ";
    //}

    //// specifies to use ram file in shared memory.
    //if (nMaxOptions::Preview == nMaxOptions::Instance()->GetExportMode())
    //    this->arguments += "-useram "; //use ram file.

    //iniFile->Release();

    return true;
}

//------------------------------------------------------------------------------
/**
    Read viewer options for custom viewer.

    @param iniFilename .ini filename which contains custom viewer options.
    @param sectionName section name in .ini file. "CustomViewerSetting" is used 
                       for a custom viewer.
*/
bool nMaxViewerOptions::ReadCustomViewerOptions(const String &iniFilename, 
                                                const String &sectionName)
{
    //nIniPrefServer* iniFile = (nIniPrefServer*)nKernelServer::Instance()->New("niniprefserver", "/iniprefsrv");
    //iniFile->SetFileName(iniFilename);
    //iniFile->SetSection("CustomViewerSetting");

    //String args;

    //iniFile->SetDefault("");
    //args = iniFile->ReadString("args");
    //if (args.IsEmpty())
    //{
    //    n_maxlog(Warning, "Failed to read 'CustomViewerSetting' in %s.ini file\n", iniFilename.Get());
    //    return false;
    //}
    //this->arguments += args;

    //iniFile->Release();
    return true;
}



