//-----------------------------------------------------------------------------
//  nmaxoptions.h
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXOPTIONS_H
#define N_MAXOPTIONS_H
//-----------------------------------------------------------------------------
/**
    @class nMaxOptions
    @ingroup NebulaMaxExport2Contrib

    @brief A class for global options of the exporter.

*/
//#include "util/Util::String.h"
#include "util/string.h"

class nIniPrefServer;

//-----------------------------------------------------------------------------
class nMaxOptions
{
public:
	enum ExportMode
	{
		Preview  = 0,
		Normal, 
		AnimOnly,

		NumMode,
	};

public:
    /// the destructor.
    virtual ~nMaxOptions();

    static nMaxOptions* Instance();

    bool Initialize();
    bool LoadUtilityOptions();
    void SaveUtilityOptions();

    void LoadSceneOptions();

    bool UseSameDir();
	const Util::String& GetSceneDir() const;
    const Util::String& GetMeshExportPath() const;
    const Util::String& GetAnimExportPath() const;

    void SetSaveFileName(const Util::String&);
    Util::String GetSaveFileName() const;
    const Util::String& GetSaveFilePath() const;
    bool GroupMeshes();

    // @name Mesh options
    // @{
    void SetExportNormals(bool status);
    void SetExportColors(bool status);
    void SetExportUvs(bool status);
    void SetExportTangents(bool status);
    
    bool ExportNormals();
    bool ExportColors();
    bool ExportUvs();
    bool ExportTangents();
    /// @}

    // @name assign and path access functions.
    // @{
    const Util::String& GetHomePath() const;
    const Util::String& GetProjPath() const;
    const Util::String& GetAnimAssign() const;
    const Util::String& GetAnimPath() const;

    const Util::String& GetGfxLibAssign() const;
    const Util::String& GetGfxLibPath() const;

    const Util::String& GetMeshesAssign() const;
    const Util::String& GetMeshesPath() const;

    const Util::String& GetTextureAssign() const;
    const Util::String& GetTexturePath() const;
    /// @}

    /// @name Global utility panel options
    /// @{
    void SetGeomScaleValue(float val);
    float GetGeomScaleValue() const;

    void SetSampleRate(int val);
    int GetSampleRate() const;

    void SetMeshFileType(const char* type);
    const Util::String& GetMeshFileType() const;
    void SetAnimFileType(const char* type);
    const Util::String& GetAnimFileType() const;

	void SetUseCharacter3SkinAnimator(bool b);
	bool GetUseCharacter3SkinAnimator() const;

    void SetExportHiddenNodes(bool status);
    bool ExportHiddenNodes();

    void SetMaxJointPaletteSize(int size);
    int GetMaxJointPaletteSize();
    void SetWeightThreshold(float val);
    float GetWeightThreshold() const;

    void SetSaveScriptServer(const char* serverName);
    const Util::String& GetSaveScriptServer();
    /// @}

    bool RunViewer();

    const char* GetViewerDir() const;

    bool UseIndivisualMesh() const;

    int GetVerboseLevel() const;
    void SetVerboseLevel(int level);

    bool OverwriteExistTexture() const;

    //void SetPreviewMode(bool on);
    //bool UsePreviewMode() const;
    void SetExportMode(ExportMode m);
    ExportMode GetExportMode() const;

    bool UseDefaultViewer() const;
    void SetUseDefaultViewer(bool use);

    /// @name Mesh File Extension
    /// @{
    Util::String GetStaticMeshSuffix() const;
    Util::String GetSkinnedMeshSuffixt() const;
    Util::String GetShadowMeshSuffix() const;
    Util::String GetSkinnedShadowMeshSuffix() const;
    Util::String GetSwingMeshSuffix() const;
    Util::String GetCollisionMeshSuffix() const;
    Util::String GetParticle2MeshSuffix() const;
    /// @}

	/// @name Pack Vertex Component 
	/// @{
	bool PackVertexComponents() const;
	/// @}
protected:
    ///
    bool LoadDirSettings();
    ///
    bool LoadMeshSuffix();

    ///
    //nIniPrefServer* BeginINIPref(const Util::String& section);
    ///
    //void EndINIPref(nIniPrefServer* iniFIle);

protected:
    ///
    bool useSameDir;
    Util::String sceneDir;
    Util::String meshExportPath;
    Util::String animExportPath;

    /// @name export option variables.
    /// @{
    bool exportNormal;
    bool exportVertColor;
    bool exportUvs;
    bool exportTangent;

    bool exportHiddenNodes;
    bool exportAnimations;
    bool groupMeshes;

    /// mesh file extension '.n3d2' or 'nvx2'
    Util::String meshFileExtension;
    /// animation file extension '.nanim2' or 'nax2'
    Util::String animFileExtension;

    Util::String sceneFilename;

	bool useCharacter3SkinAnimator;

    /// animation sample rate.
    int sampleRate;
    /// maximum number of joint palette.
    int maxJointPaletteSize;
    /// weight threshold.
    float weightThreshold;
    /// flag for adding joint name.
    bool addJointName;
    /// geometry scaling value.
    float geomScale;
    /// @}

    /// @name assign and path variables.
    /// @{
    Util::String homeDir;
    Util::String projDir;
    Util::String binaryPath;

    Util::String animsAssign;
    Util::String gfxlibAssign;
    Util::String guiAssign;
    Util::String lightsAssign;
    Util::String meshesAssign;
    Util::String shadersAssign;
    Util::String texturesAssign;

    Util::String animsPath;
    Util::String gfxlibPath;
    Util::String guiPath;
    Util::String lightsPath;
    Util::String meshesPath;
    Util::String shadersPath;
    Util::String texturesPath;
    /// @}

    /// the script server class which should be used for saving.
    Util::String saveScriptServer; 
    /// 
    bool useIndivisualMesh;
    /// launch specified viewer.
    bool runViewer;
    /// preview mode. It ueses ram file to view exported model.
    //bool previewMode;
	ExportMode exportMode;
    /// default viewer.
    bool useDefaultViewer;
    /// verbose level of log message. 0: errors only, 1:warning, 2:low, 3:midium, 4:high
    int verboseLevel; 
    /// If it is present, overwrite any existing texture.
    bool overwriteExistTexture;

    /// @name
    /// @{
    /// static mesh file suffic
    Util::String meshSuffixStatic;
    /// skinned mesh file suffic
    Util::String meshSuffixSkinned;
    /// shadow mesh file suffix
    Util::String meshSuffixShadow;
    /// skinned shadow mesh file suffix
    Util::String meshSuffixSkinnedShadow;
    /// collision mesh file suffix
    Util::String meshSuffixCollision;
    /// particle2 mesh file suffix
    Util::String meshSuffixParticle2;
    /// swing mesh file suffix
    Util::String meshSuffixSwing;
    /// @}

private:
    nMaxOptions();
    nMaxOptions(const nMaxOptions&) {};

    static nMaxOptions* Singleton;

};
//-----------------------------------------------------------------------------
/**
    DO NOT FORGET TO CALL 'n_delete' for intance of nMaxOption 
    when application exit.
*/
inline
nMaxOptions*
nMaxOptions::Instance()
{
    // nMaxOption is not nRoot derived class, so we make its instance 
    // directly with n_new if it is not created before.
    if (0 == Singleton)
    {
        n_new(nMaxOptions);
    }

    return Singleton;
}
//-----------------------------------------------------------------------------
inline
bool nMaxOptions::UseSameDir()
{
    return this->useSameDir;
}
//-----------------------------------------------------------------------------
inline
const Util::String& nMaxOptions::GetSceneDir() const
{
    return this->sceneDir;
}
//-----------------------------------------------------------------------------
inline
const Util::String& nMaxOptions::GetMeshExportPath() const
{
    return this->meshExportPath;
}
//-----------------------------------------------------------------------------
inline
const Util::String& nMaxOptions::GetAnimExportPath() const
{
    return this->animExportPath;
}

//-----------------------------------------------------------------------------
inline
void 
nMaxOptions::SetSaveFileName(const Util::String& saveFileName)
{
    this->sceneFilename = saveFileName;
}
//-----------------------------------------------------------------------------
/**
    c:/nebula2/export/mywork.n2 -> mywork
*/
inline
Util::String 
nMaxOptions::GetSaveFileName() const
{
    Util::String str;

    str = this->sceneFilename;
    str = sceneFilename.ExtractFileName();
    str.StripFileExtension();

    return str;
}
//-----------------------------------------------------------------------------
inline
const Util::String& 
nMaxOptions::GetSaveFilePath() const
{
    return this->sceneFilename;
}
//-----------------------------------------------------------------------------
/**
    Group meshes to one master mesh file.
*/
inline
bool nMaxOptions::GroupMeshes()
{
    return this->groupMeshes;
}
//-----------------------------------------------------------------------------
inline
void 
nMaxOptions::SetSaveScriptServer(const char* serverName)
{
    this->saveScriptServer = serverName;
}
//-----------------------------------------------------------------------------
inline
const Util::String& 
nMaxOptions::GetSaveScriptServer()
{
    return this->saveScriptServer;
}
//-----------------------------------------------------------------------------
inline
int nMaxOptions::GetVerboseLevel() const
{
    return this->verboseLevel;
}
//-----------------------------------------------------------------------------
inline
void nMaxOptions::SetVerboseLevel(int level)
{
    this->verboseLevel = level;
}
//-----------------------------------------------------------------------------
inline
bool nMaxOptions::UseIndivisualMesh() const
{
    return this->useIndivisualMesh;
}
//-----------------------------------------------------------------------------
inline
void nMaxOptions::SetMeshFileType(const char* type)
{
    n_assert(type);
    this->meshFileExtension = type;
}
//-----------------------------------------------------------------------------
/**
    Return mesh file extension ".n3d2" or ".nvx2"
*/
inline
const Util::String& nMaxOptions::GetMeshFileType() const
{
    return this->meshFileExtension;
}
//-----------------------------------------------------------------------------
inline
void nMaxOptions::SetAnimFileType(const char* type)
{
    n_assert(type);
     this->animFileExtension = type;
}
//-----------------------------------------------------------------------------
/**
    Return animation file extension ".nanim2" or ".nax2"
*/
inline
const Util::String& nMaxOptions::GetAnimFileType() const
{
    return this->animFileExtension;
}
//-----------------------------------------------------------------------------
inline
void nMaxOptions::SetUseCharacter3SkinAnimator(bool b)
{
	this->useCharacter3SkinAnimator = b;
}
//-----------------------------------------------------------------------------
inline
bool nMaxOptions::GetUseCharacter3SkinAnimator() const
{
	return this->useCharacter3SkinAnimator;
}
//-----------------------------------------------------------------------------
inline
void nMaxOptions::SetMaxJointPaletteSize(int size)
{
    this->maxJointPaletteSize = size;
}
//-----------------------------------------------------------------------------
inline
int nMaxOptions::GetMaxJointPaletteSize()
{
    return this->maxJointPaletteSize;
}
//-----------------------------------------------------------------------------
inline
void nMaxOptions::SetWeightThreshold(float val)
{
    this->weightThreshold = val;
}
//-----------------------------------------------------------------------------
inline
float nMaxOptions::GetWeightThreshold() const
{
    return this->weightThreshold;
}
//-----------------------------------------------------------------------------
inline
void nMaxOptions::SetGeomScaleValue(float val)
{
    this->geomScale = val;
}
//-----------------------------------------------------------------------------
inline
float nMaxOptions::GetGeomScaleValue() const
{
    return this->geomScale;
}
//-----------------------------------------------------------------------------
inline
void nMaxOptions::SetExportMode(ExportMode m)
{
    this->exportMode = m;
}
//-----------------------------------------------------------------------------
inline
nMaxOptions::ExportMode 
nMaxOptions::GetExportMode() const
{
    return this->exportMode;
}
//-----------------------------------------------------------------------------
inline
bool nMaxOptions::UseDefaultViewer() const
{
    return this->useDefaultViewer;
}
//-----------------------------------------------------------------------------
inline
void nMaxOptions::SetUseDefaultViewer(bool use)
{
    this->useDefaultViewer = use;
}
//-----------------------------------------------------------------------------
inline
Util::String nMaxOptions::GetStaticMeshSuffix() const
{
    return this->meshSuffixStatic;
}
//-----------------------------------------------------------------------------
inline
Util::String nMaxOptions::GetSkinnedMeshSuffixt() const
{
    return this->meshSuffixSkinned;
}
//-----------------------------------------------------------------------------
inline
Util::String nMaxOptions::GetShadowMeshSuffix() const
{
    return this->meshSuffixShadow;
}
//-----------------------------------------------------------------------------
inline
Util::String nMaxOptions::GetSkinnedShadowMeshSuffix() const
{
    return this->meshSuffixSkinnedShadow;
}
//-----------------------------------------------------------------------------
inline
Util::String nMaxOptions::GetSwingMeshSuffix() const
{
    return this->meshSuffixSwing;
}
//-----------------------------------------------------------------------------
inline
Util::String nMaxOptions::GetCollisionMeshSuffix() const
{
    return this->meshSuffixCollision;
}
//-----------------------------------------------------------------------------
inline
Util::String nMaxOptions::GetParticle2MeshSuffix() const
{
    return this->meshSuffixParticle2;
}
//-----------------------------------------------------------------------------
#endif
