#pragma once
//------------------------------------------------------------------------------
/**
    @class Toolkit::N2ConverterApp
    
    N2->N3 model file converter application class.
    
    (C) 2008 Radon Labs GmbH
*/
#include "toolkitutil/toolkitapp.h"
#include "toolkitutil/n2converter.h"

//------------------------------------------------------------------------------
namespace Toolkit
{
class N2ConverterApp : public ToolkitUtil::ToolkitApp
{
public:
    /// constructor
    N2ConverterApp();
    /// run the application
    virtual void Run();

private:
    /// parse command line arguments
    virtual bool ParseCmdLineArgs();
    /// setup project info object
    virtual bool SetupProjectInfo();
    /// print help text
    virtual void ShowHelp();

    ToolkitUtil::N2Converter n2Converter;
    Util::String category;
    Util::String filename;
    bool logResources;
};

} // namespace Toolkit
//------------------------------------------------------------------------------
