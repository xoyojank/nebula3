//-----------------------------------------------------------------------------
//  nMaxOptions.cc
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "stdneb.h"
#include "base/nmaxlistener.h"
#include "export2/nmax.h"
#include "export2/nmaxinterface.h"
#include "export2/nmaxcustattrib.h"
#include "export2/nmaxbones.h"
#include "tinyxml/tinyxml.h"

//#include "kernel/nkernelserver.h"
//#include "kernel/nfileserver2.h"
//#include "kernel/nfile.h"
//#include "misc/niniprefserver.h"

#include "export2/nmaxoptions.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

using namespace Util;
nMaxOptions* nMaxOptions::Singleton = 0;

//-----------------------------------------------------------------------------
/**
*/
nMaxOptions::nMaxOptions() : 
    useSameDir(false),
    exportNormal(true),
    exportVertColor(true),
    exportUvs(true),
    exportTangent(true),
    exportHiddenNodes(false),
    exportAnimations(true),
    groupMeshes(true),
    meshFileExtension(".n3d2"),
    animFileExtension(".nanim2"),
	useCharacter3SkinAnimator(false),
    geomScale(0.01f),
    maxJointPaletteSize(24),
    weightThreshold(0.0001f),
    sampleRate (2),
    addJointName(false),
    runViewer(true),
    saveScriptServer("ntclserver"),
    useIndivisualMesh(false),
    verboseLevel(2/*Low*/),
    overwriteExistTexture(true),
    //previewMode(false),
	exportMode(Normal),
    useDefaultViewer(true),
    meshSuffixStatic(""),
    meshSuffixSkinned("_skinned"),
    meshSuffixShadow("_shadow"),
    meshSuffixSkinnedShadow("_skinnedshadow"),
    meshSuffixCollision("_collision"),
    meshSuffixParticle2("_particle2"),
    meshSuffixSwing("_swing")
{
    n_assert(Singleton == 0);
    Singleton = this;
}

//-----------------------------------------------------------------------------
/**
*/
nMaxOptions::~nMaxOptions()
{
    Singleton = 0;
}

////-----------------------------------------------------------------------------
///**
//    Open nmaxtoolbox.ini file and prepare to read the specified section.
//*/
//nIniPrefServer* nMaxOptions::BeginINIPref(const String& section)
//{
//    String iniFilename;
//    iniFilename += GetCOREInterface()->GetDir(APP_PLUGCFG_DIR);
//    iniFilename += "\\";
//    iniFilename += N_MAXEXPORT_INIFILE;
//
//    nFileServer2* fileServer = nFileServer2::Instance();
//
//    // check the .ini file exist in 3dsmax plugin directory.
//    if (!fileServer->FileExists(iniFilename))
//    {
//		// try to find the .ini file in the 'scripts\nebula' dirtectory. e.g.) d:\3dsmax\Scripts\nebula2
//		iniFilename = "";
//        iniFilename += GetCOREInterface()->GetDir(APP_SCRIPTS_DIR);
//        iniFilename += "\\";
//		iniFilename += "nebula2\\";
//        iniFilename += N_MAXEXPORT_INIFILE;
//        if (!fileServer->FileExists(iniFilename))
//        {
//            // .ini file does not exist in '/plugcfg' directory.
//            n_listener("%s file does not exist in '$3dsmax/plugcfg' directory.\n", N_MAXEXPORT_INIFILE);
//            return 0;
//		}
//    }
//
//    nIniPrefServer* iniFile = (nIniPrefServer*)nKernelServer::Instance()->New("niniprefserver", "/iniprefsrv");
//    iniFile->SetFileName(iniFilename);
//    iniFile->SetSection(section);
//
//    return iniFile;
//}
//
////-----------------------------------------------------------------------------
///**
//    Release opened INI file.
//*/
//void nMaxOptions::EndINIPref(nIniPrefServer* iniFile)
//{
//    iniFile->Release();
//}

//-----------------------------------------------------------------------------
/**
*/
bool nMaxOptions::Initialize()
{
    if (!LoadDirSettings())
        return false;

    if (!LoadMeshSuffix())
        return false;

    return true;
}

//-----------------------------------------------------------------------------
/**
    Read .ini file in '$3dsmax/plugcfg' directory and specifies assigns and 
    paths to file server.

    @return true if the function call is success.

    -19-Sep-05    kims    Added reading 'proj' assign from .ini file.
*/
bool nMaxOptions::LoadDirSettings()
{
    //nIniPrefServer* iniFile = this->BeginINIPref("GeneralSettings");

    //iniFile->SetDefault(".");
    //
    //homeDir = iniFile->ReadString("HomeDir");
    //if (homeDir.IsEmpty()) 
    //    return false;
    //
    //projDir = iniFile->ReadString("ProjDir");
    //if (projDir.IsEmpty()) 
    //    return false;
    //
    //binaryPath = iniFile->ReadString("BinaryPath") ;
    //if (binaryPath.IsEmpty())    
    //    return false;

    //iniFile->SetDefault(N_MAXEXPORT_ANIMS_ASSIGN);
    //animsAssign = iniFile->ReadString("AnimsAssign");
    //if (animsAssign.IsEmpty())
    //    return false;
    //
    //iniFile->SetDefault(N_MAXEXPORT_GFXLIB_ASSIGN);
    //gfxlibAssign = iniFile->ReadString("GfxlibAssign");
    //if (gfxlibAssign.IsEmpty())
    //    return false;
    //
    //iniFile->SetDefault(N_MAXEXPORT_GUI_ASSIGN);
    //guiAssign = iniFile->ReadString("GuiAssign") ;
    //if (guiAssign.IsEmpty())
    //    return false;
    //
    //iniFile->SetDefault(N_MAXEXPORT_LIGHTS_ASSIGN);
    //lightsAssign = iniFile->ReadString("LightsAssign");
    //if (lightsAssign.IsEmpty())
    //    return false;

    //iniFile->SetDefault(N_MAXEXPORT_MESHES_ASSIGN);
    //meshesAssign = iniFile->ReadString("MeshesAssign");
    //if (meshesAssign.IsEmpty())
    //    return false;

    //iniFile->SetDefault(N_MAXEXPORT_SHADERS_ASSIGN);
    //shadersAssign = iniFile->ReadString("ShadersAssign");
    //if (shadersAssign.IsEmpty())
    //    return false;

    //iniFile->SetDefault(N_MAXEXPORT_TEXTURES_ASSIGN);
    //texturesAssign = iniFile->ReadString("TexturesAssign");
    //if (texturesAssign.IsEmpty())
    //    return false;

    //iniFile->SetDefault(N_MAXEXPORT_ANIMS_PATH);
    //animsPath = iniFile->ReadString("AnimsPath");
    //if (animsPath.IsEmpty())
    //    return false;

    //iniFile->SetDefault(N_MAXEXPORT_GFXLIB_PATH);
    //gfxlibPath = iniFile->ReadString("GfxlibPath");
    //if (gfxlibPath.IsEmpty())
    //    return false;

    //iniFile->SetDefault(N_MAXEXPORT_GUI_PATH);
    //guiPath = iniFile->ReadString("GuiPath");
    //if (guiPath.IsEmpty())
    //    return false;

    //iniFile->SetDefault(N_MAXEXPORT_LIGHTS_PATH);
    //lightsPath = iniFile->ReadString("LightsPath");
    //if (lightsPath.IsEmpty())
    //    return false;

    //iniFile->SetDefault(N_MAXEXPORT_MESHES_PATH);
    //meshesPath = iniFile->ReadString("MeshesPath");
    //if (meshesPath.IsEmpty())
    //    return false;

    //iniFile->SetDefault(N_MAXEXPORT_SHADERS_PATH);
    //shadersPath = iniFile->ReadString("ShadersPath");
    //if (shadersPath.IsEmpty())
    //    return false;

    //iniFile->SetDefault(N_MAXEXPORT_TEXTURES_PATH);
    //texturesPath = iniFile->ReadString("TexturesPath");
    //if (texturesPath.IsEmpty())
    //    return false;

    ////iniFile->Release();
    //this->EndINIPref(iniFile);

    //nFileServer2* fileServer = nFileServer2::Instance();

    //if (!fileServer->DirectoryExists(this->homeDir.Get()))
    //{
    //    n_listener("Home path '%s' does not exist.\n", this->homeDir.Get());
    //    return false;
    //}
    //else
    //{
    //    String tmp;

    //    fileServer->SetAssign("home", homeDir.Get());

    //    if (this->projDir == ".")
    //    {
    //        // the 'proj' dir was not specified, set 'home' dir for it.
    //        fileServer->SetAssign("proj", fileServer->GetAssign("home"));
    //    }
    //    else
    //    {
    //        fileServer->SetAssign("proj", projDir.Get());
    //    }

    //    if (!fileServer->DirectoryExists(binaryPath))
    //    {
    //        tmp += "\n  binary path: ";
    //        tmp += binaryPath;
    //    }

    //    if (!fileServer->DirectoryExists(animsAssign))
    //    {
    //        tmp += "\n  'anims' assign: ";
    //        tmp += animsAssign;
    //    }
    //    else
    //    {
    //        fileServer->SetAssign("anims", animsAssign.Get());
    //        if (!fileServer->DirectoryExists(animsPath))
    //        {
    //            tmp += "\n  'Animation' path: ";
    //            tmp += animsPath;
    //        }
    //    }

    //    if (!fileServer->DirectoryExists(gfxlibAssign))
    //    {
    //        tmp += "\n  'gfxlib' assign: ";
    //        tmp += gfxlibAssign;
    //    }
    //    else
    //    {
    //        fileServer->SetAssign("gfxlib", gfxlibAssign.Get());
    //        if (!fileServer->DirectoryExists(gfxlibPath))
    //        {
    //            tmp += "\n  'Gfx' path: ";
    //            tmp += gfxlibPath;
    //        }
    //    }

    //    if (!fileServer->DirectoryExists(guiAssign))
    //    {
    //        tmp += "\n  'gui' assign: ";
    //        tmp += guiAssign;
    //    }
    //    else
    //    {
    //        fileServer->SetAssign("gui", guiAssign.Get());
    //        if (!fileServer->DirectoryExists(guiPath))
    //        {
    //            tmp += "\n  'Gui' path: ";
    //            tmp += guiPath;
    //        }
    //    }

    //    if (!fileServer->DirectoryExists(lightsAssign))
    //    {
    //        tmp += "\n  'lights' assign: ";
    //        tmp += lightsAssign;
    //    }
    //    else
    //    {
    //        fileServer->SetAssign("lights", lightsAssign.Get());
    //        if (!fileServer->DirectoryExists(lightsPath))
    //        {
    //            tmp += "\n  'Lights' path: ";
    //            tmp += lightsPath;
    //        }
    //    }

    //    if (!fileServer->DirectoryExists(meshesAssign))
    //    {
    //        tmp += "\n  'meshes' assign: ";
    //        tmp += meshesAssign;
    //    }
    //    else
    //    {
    //        fileServer->SetAssign("meshes", meshesAssign.Get());
    //        if (!fileServer->DirectoryExists(meshesPath))
    //        {
    //            tmp += "\n  'Meshes' path: ";
    //            tmp += meshesPath;
    //        }
    //    }

    //    if (!fileServer->DirectoryExists(shadersAssign))
    //    {
    //        tmp += "\n  'shaders' assign: ";
    //        tmp += shadersAssign;
    //    }
    //    else
    //    {
    //        fileServer->SetAssign("shaders", shadersAssign.Get());
    //        if (!fileServer->DirectoryExists(shadersPath))
    //        {
    //            tmp += "\n  'Shaders' path: ";
    //            tmp += shadersPath;
    //        }
    //    }

    //    if (!fileServer->DirectoryExists(texturesAssign))
    //    {
    //        tmp += "\n  'textures' assign: ";
    //        tmp += texturesAssign;
    //    }
    //    else
    //    {
    //        fileServer->SetAssign("textures", texturesAssign.Get());
    //        if (!fileServer->DirectoryExists(texturesPath))
    //        {
    //            tmp += "\n  'Textures' path: ";
    //            tmp += texturesPath;
    //        }
    //    }

    //    if (tmp.Length() > 0)
    //    {
    //        String alerts("ALERT: assignment of paths are wrong:");
    //        alerts += tmp;
    //        n_listener("%s\n", alerts.Get());
    //        return false;
    //    }
    //}

    return true;
}

//-----------------------------------------------------------------------------
/**
    Retrieve scene option from utility panel.

*/
void nMaxOptions::LoadSceneOptions()
{
    //INode* sceneRoot = nMaxInterface::Instance()->GetInterface()->GetRootNode();

    //TiXmlDocument xmlDoc;
    //nMaxCustAttrib custAttrib;

    //// HACK: sceneRoot is root node in the current 3dsmax viewport.
    //if (!custAttrib.Convert(sceneRoot, xmlDoc))
    //{
    //    n_maxlog(Low, "A scene option custom attribute is not specified.");
    //    return;
    //}
    //TiXmlHandle xmlHandle(&xmlDoc);

    //// parameter block name for gfx directory setting.
    //const char* dirParamName = "SceneDirSetting";
    //TiXmlElement* e;
    //e = xmlHandle.FirstChild(dirParamName).Element();
    //if (!e)
    //    return;

    //nFileServer2* fileServer = nFileServer2::Instance();

    //// find parameter with the given its name.
    //TiXmlElement* child;
    //child = xmlHandle.FirstChild(dirParamName).FirstChild("gfxDir").Child("", 0).Element();
    //if (child)
    //{
    //    String scenePath = child->Attribute("value");

    //    // The directory parameter has "" for default string. It is absolutely necessary in Max6.
    //    // Without that, the exporter is not usable as the panels that have those controls in them don't work.
    //    if (scenePath.IsEmpty())
    //    {
    //        // The given scene (which to be exported) has already scene option custom attribute
    //        // but the directory is not specified so we just use default gfx assign value for it.
    //        // (the directory path which is specified in "Directory Setting Dialog".)
    //        this->sceneDir = fileServer->ManglePath(nMaxOptions::Instance()->GetGfxLibAssign());
    //    }
    //    else
    //    {
    //        this->sceneDir = fileServer->ManglePath(scenePath);
    //    }
    //    fileServer->MakePath(this->sceneDir);
    //}

    //child = xmlHandle.FirstChild(dirParamName).FirstChild("useSameDir").Child("", 0).Element();
    //if (!child)
    //    return;

    //String samePath = child->Attribute("value");
    //this->useSameDir = samePath == "1";
    //if (this->useSameDir)
    //{
    //    String gfxPath = fileServer->ManglePath("gfxlib:");
    //    String exportPath = this->sceneDir.ExtractRange(gfxPath.Length(), sceneDir.Length() - gfxPath.Length());

    //    this->meshExportPath = fileServer->GetAssign("meshes");
    //    this->meshExportPath = fileServer->ManglePath(this->meshExportPath);
    //    this->meshExportPath += exportPath;
    //    this->animExportPath = fileServer->GetAssign("anims");
    //    this->animExportPath = fileServer->ManglePath(this->animExportPath);
    //    this->animExportPath += exportPath;

    //    fileServer->MakePath(this->meshExportPath);

    //    if( nMaxBoneManager::Instance()->GetNumBones() > 0 )
    //    {
    //        fileServer->MakePath(this->animExportPath);
    //    }
    //}
}

//-----------------------------------------------------------------------------
/**
*/
bool nMaxOptions::LoadMeshSuffix()
{
    //nIniPrefServer* iniFile = this->BeginINIPref("MeshSuffix");
    //if (!iniFile)
    //    return false;

    //iniFile->SetDefault("");
    //this->meshSuffixStatic = iniFile->ReadString("static");
    //iniFile->SetDefault("_skinned");
    //this->meshSuffixSkinned = iniFile->ReadString("skinned");
    //iniFile->SetDefault("_shadow");
    //this->meshSuffixShadow = iniFile->ReadString("shadow");
    //iniFile->SetDefault("_skinnedshadow");
    //this->meshSuffixSkinnedShadow = iniFile->ReadString("skinnedshadow");
    //iniFile->SetDefault("_collision");
    //this->meshSuffixCollision = iniFile->ReadString("collision");
    //iniFile->SetDefault("_particle2");
    //this->meshSuffixParticle2 = iniFile->ReadString("particle2");
    //iniFile->SetDefault("_swing");
    //this->meshSuffixSwing = iniFile->ReadString("swing");

    ////iniFile->Release();
    //this->EndINIPref(iniFile);

    return true;
}

//-----------------------------------------------------------------------------
/**
    Load various utility options from nmaxtoolbox.ini file. <br>
    It is called whenever the plugin is initialized.

    @note
    If you add any optin in .ini file, don't forget to add code saves the option 
    in nMaxOptions::SaveUtilityOptions() function.
*/
bool nMaxOptions::LoadUtilityOptions()
{
 //   nIniPrefServer* iniFile = this->BeginINIPref("UtilityOptions");
 //   if (!iniFile)
 //       return false;

 //   iniFile->SetDefault("0.01f");
 //   this->geomScale = iniFile->ReadFloat ("GeometryScale");

 //   iniFile->SetDefault("24");
 //   this->maxJointPaletteSize = iniFile->ReadInt ("MaxJointPalette");

 //   iniFile->SetDefault("0.0001f");
 //   this->weightThreshold = iniFile->ReadFloat ("WeightThreshold");

 //   iniFile->SetDefault(".n3d2");
 //   this->meshFileExtension = iniFile->ReadString("SaveMeshAs");

 //   iniFile->SetDefault(".nanim2");
 //   this->animFileExtension = iniFile->ReadString("SaveAnimAs");

 //   iniFile->SetDefault("2");
 //   this->sampleRate = iniFile->ReadInt ("SampleRate");

	//iniFile->SetDefault("false");
	//this->useCharacter3SkinAnimator = iniFile->ReadBool("UseCharacter3SkinAnimator");

 //   //FIXME: change it to use ReadBool()
 //   //int tmpExportHiddenNode;

 //   iniFile->SetDefault("false");
 //   this->exportHiddenNodes = iniFile->ReadBool("ExportHiddenNode");

 //   iniFile->SetDefault("ntclserver");
 //   this->saveScriptServer = iniFile->ReadString("ScriptServer");
 //   if (this->saveScriptServer.IsEmpty())
 //   {
 //       n_message("Faied to read script server option in nmaxtoolbox.ini. \
 //                 'ntclserver' is used as default.");
 //   }

 //   iniFile->SetDefault("4");
 //   this->verboseLevel= iniFile->ReadInt("Verbose");
 //   if (this->verboseLevel <0 || this->verboseLevel > 5)
 //   {
 //       n_message("Failed to read verbose option in %s or \
 //                  wrong verbose level in the ini file. \
 //                  Set to 'high' for the default value.", N_MAXEXPORT_INIFILE);
 //   }

 //   //iniFile->Release();
 //   this->EndINIPref(iniFile);

    return true;
}

//-----------------------------------------------------------------------------
/**
    Save the settings to nmaxtoolbox.ini file. Called when the plugin is detached.
    
    @note 
    The settings of 'GeneralSettings' section in the nmaxtoolbox.ini are saved
    when the 'ok' buttion of the directory setting dialog is clicked.
        
*/
void nMaxOptions::SaveUtilityOptions()
{
//    // write the values to .ini file.
//    nIniPrefServer* iniFile = this->BeginINIPref("UtilityOptions");
//
//    iniFile->WriteFloat ("GeometryScale",   this->geomScale);
//    iniFile->WriteInt   ("MaxJointPalette", this->maxJointPaletteSize);
//    iniFile->WriteFloat ("WeightThreshold", this->weightThreshold);
//    iniFile->WriteString("SaveMeshAs",      this->meshFileExtension);
//    iniFile->WriteString("SaveAnimAs",      this->animFileExtension);
//	iniFile->WriteBool  ("UseCharacter3SkinAnimator", this->useCharacter3SkinAnimator);
//    iniFile->WriteInt   ("SampleRate",      this->sampleRate);
//    iniFile->WriteBool  ("ExportHiddenNode",this->exportHiddenNodes);
//    iniFile->WriteString("ScriptServer",    this->saveScriptServer);
//    iniFile->WriteInt   ("Verbose",         this->verboseLevel);
//
////    iniFile->Release();
//    this->EndINIPref(iniFile);
}

//-----------------------------------------------------------------------------
void nMaxOptions::SetExportNormals(bool status)
{
    this->exportNormal = status;
}
//-----------------------------------------------------------------------------
void nMaxOptions::SetExportColors(bool status)
{
    this->exportVertColor = status;
}
//-----------------------------------------------------------------------------
void nMaxOptions::SetExportUvs(bool status)
{
    this->exportUvs = status;
}
//-----------------------------------------------------------------------------
void nMaxOptions::SetExportTangents(bool status)
{
    this->exportTangent = status;
}
//-----------------------------------------------------------------------------
void nMaxOptions::SetExportHiddenNodes(bool status)
{
    this->exportHiddenNodes = status;
}
//-----------------------------------------------------------------------------
bool nMaxOptions::ExportNormals()
{
    return this->exportNormal;
}
//-----------------------------------------------------------------------------
bool nMaxOptions::ExportColors()
{
    return this->exportVertColor;
}
//-----------------------------------------------------------------------------
bool nMaxOptions::ExportUvs()
{
    return this->exportUvs;
}
//-----------------------------------------------------------------------------
bool nMaxOptions::ExportTangents()
{
    return this->exportTangent;
}
//-----------------------------------------------------------------------------
bool nMaxOptions::ExportHiddenNodes()
{
    return this->exportHiddenNodes;
}
//-----------------------------------------------------------------------------
void nMaxOptions::SetSampleRate(int val)
{
    this->sampleRate = val;
}
//-----------------------------------------------------------------------------
int nMaxOptions::GetSampleRate() const
{
    return this->sampleRate;
}
//-----------------------------------------------------------------------------
/**
*/
const char* nMaxOptions::GetViewerDir() const
{
    return this->binaryPath.Get();
}
//-----------------------------------------------------------------------------
/**
*/
bool nMaxOptions::RunViewer()
{
    return this->runViewer;
}
//-----------------------------------------------------------------------------
const String& 
nMaxOptions::GetHomePath() const
{
    return this->homeDir;
}

//-----------------------------------------------------------------------------
const String&
nMaxOptions::GetProjPath() const
{
    return this->projDir;
}
//-----------------------------------------------------------------------------
const String& 
nMaxOptions::GetAnimAssign() const
{
    return this->animsAssign;
}
//-----------------------------------------------------------------------------
const String&
nMaxOptions::GetAnimPath() const
{
    return this->animsPath;
}
//-----------------------------------------------------------------------------
const String& 
nMaxOptions::GetGfxLibAssign() const
{
    return this->gfxlibAssign;
}
//-----------------------------------------------------------------------------
const String& 
nMaxOptions::GetGfxLibPath() const
{
    return this->gfxlibPath;
}
//-----------------------------------------------------------------------------
const String& 
nMaxOptions::GetMeshesAssign() const
{
    return this->meshesAssign;
}
//-----------------------------------------------------------------------------
const String& 
nMaxOptions::GetMeshesPath() const
{
    return this->meshesPath;
}
//-----------------------------------------------------------------------------
const String& 
nMaxOptions::GetTextureAssign() const
{
    return this->texturesAssign;
}
//-----------------------------------------------------------------------------
const String& 
nMaxOptions::GetTexturePath() const
{
    return this->texturesPath;
}
//-----------------------------------------------------------------------------
bool nMaxOptions::OverwriteExistTexture() const
{
    return this->overwriteExistTexture;
}
