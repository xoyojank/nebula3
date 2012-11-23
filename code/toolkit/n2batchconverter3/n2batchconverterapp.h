#pragma once
//------------------------------------------------------------------------------
/**
    @class Toolkit::N2BatchConverterApp
  
    Batch converter to convert .n2 files and their resources (meshes, textures,
    anims) into their N3 file formats. This works like the n2converter3 tool,
    but additionally converts the resources used by the converted .n2 files.
    
    (C) 2009 Radon Labs GmbH
*/
#include "toolkitutil/toolkitapp.h"
#include "toolkitutil/n2batchconverter.h"

//------------------------------------------------------------------------------
namespace Toolkit
{
class N2BatchConverterApp : public ToolkitUtil::ToolkitApp
{
public:
    /// run the application
    virtual void Run();

private:
    /// parse command line arguments
    virtual bool ParseCmdLineArgs();
    /// setup project info object
    virtual bool SetupProjectInfo();
    /// print help text
    virtual void ShowHelp();

    ToolkitUtil::N2BatchConverter n2BatchConverter;
    Util::String category;
    Util::String filename;
};

} // namespace Toolkit
//------------------------------------------------------------------------------

    