//------------------------------------------------------------------------------
//  nmaxvieweroptions.h
//
//  (C)2005 Kim, Hyoun Woo
//------------------------------------------------------------------------------
#ifndef N_MAXVIEWEROPTIONS_H
#define N_MAXVIEWEROPTIONS_H
//------------------------------------------------------------------------------
/**
    @class nMaxViewerOptions
    @ingroup NebulaMaxExport2Contrib

    Specifies viewer options of Nebula2 3DSMax Toolkit.

    @note
    A custom viewer should have '-view' command-line argument to view exported
    data.
*/
//#include "util/Util::String.h"
#include "util/string.h"

//------------------------------------------------------------------------------
class nMaxViewerOptions
{
public:
    enum ViewerType 
    {
        Default = 0,
        Custom
    };

    /// constructor
    nMaxViewerOptions();
    /// destructor
    virtual ~nMaxViewerOptions();

    /// Read viewer options from .ini file($3dsmax/plugcfg/nmaxtoolbox.ini)
    bool Read();

    /// Specifies viewer type.
    void SetViewerType (ViewerType type);
    /// Retrieves viewer type.
    ViewerType GetVIewerType() const;
    /// Specifies preview mode.
    void SetPreview (bool use);
    /// True, if we use preview mode.
    bool IsPreview() const;
    /// Retrives viewer arguments.
    Util::String GetArguments() const;
    /// Specifies executable path.
    void SetExecutable(const char* path);
    /// Retrieves executable path.
    const char* GetExecutable() const;
    /// Specifies scene filename which to load into the viewer.
    void SetSceneFileName(const char* scenename);
    /// Retrieves scene filename.
    const char* GetSceneFileName() const;
    /// Specifies scene dir
    void SetSceneDir(const Util::String& sceneDir);
    /// Get scene path
    Util::String GetScenePath() const;
protected:
    bool ReadDefaultViewerOptions(const Util::String &iniFilename, const Util::String &sectionName);
    bool ReadCustomViewerOptions(const Util::String &iniFilename, const Util::String &sectionName);

protected:
    /// viewer type.
    ViewerType viewerType;
    /// if it is set, we use previewer and do not save anything on a disk. 
    bool usePreview;
    /// arguments which to be pass to the viewer.
    Util::String arguments;
    /// executable path.
    Util::String executable;
    /// scene filename.
    Util::String sceneFilename;
    /// scene dir
    Util::String sceneDir;
};
//------------------------------------------------------------------------------
inline
void 
nMaxViewerOptions::SetViewerType(ViewerType type)
{
    this->viewerType = type;
}
//------------------------------------------------------------------------------
inline
nMaxViewerOptions::ViewerType 
nMaxViewerOptions::GetVIewerType() const
{
    return this->viewerType;
}
//------------------------------------------------------------------------------
inline
bool 
nMaxViewerOptions::IsPreview() const
{
    return this->usePreview;
}
//------------------------------------------------------------------------------
inline
void 
nMaxViewerOptions::SetPreview (bool use)
{
    this->usePreview = use;
}
//------------------------------------------------------------------------------
inline
Util::String 
nMaxViewerOptions::GetArguments() const
{
    return this->arguments;
}
//------------------------------------------------------------------------------
inline
void 
nMaxViewerOptions::SetExecutable(const char* path)
{
    this->executable = path;
}
//------------------------------------------------------------------------------
inline
const char* 
nMaxViewerOptions::GetExecutable() const
{
    return this->executable.Get();
}
//------------------------------------------------------------------------------
inline
void nMaxViewerOptions::SetSceneFileName(const char* scenename)
{
    this->sceneFilename = scenename;
}
//------------------------------------------------------------------------------
inline
const char* nMaxViewerOptions::GetSceneFileName() const
{
    return this->sceneFilename.Get();
}
//------------------------------------------------------------------------------
inline
void
nMaxViewerOptions::SetSceneDir(const Util::String& sceneDir)
{
    this->sceneDir = sceneDir;
}

//------------------------------------------------------------------------------
#endif
