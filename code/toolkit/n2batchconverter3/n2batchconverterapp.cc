//------------------------------------------------------------------------------
//  n2batchconverterapp.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "n2batchconverterapp.h"

namespace Toolkit
{

//------------------------------------------------------------------------------
/**
*/
bool
N2BatchConverterApp::ParseCmdLineArgs()
{
    if (ToolkitApp::ParseCmdLineArgs())
    {
        this->n2BatchConverter.SetForceFlag(this->args.GetBoolFlag("-force"));
        this->n2BatchConverter.SetVerbose(this->args.GetBoolFlag("-verbose"));
        this->category = this->args.GetString("-cat", "");
        this->filename = this->args.GetString("-file", "");
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
N2BatchConverterApp::SetupProjectInfo()
{
    if (ToolkitApp::SetupProjectInfo())
    {
        this->n2BatchConverter.SetPlatform(this->platform);
        if (!this->n2BatchConverter.CheckRequiredProjectInfoAttrs(this->projectInfo))
        {
            this->logger.Error("projectinfo.xml does not contain all required attributes!\n");
            return false;
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
N2BatchConverterApp::ShowHelp()
{
    n_printf("Nebula3 N2->N3 batch converter (converts .n2 files and their resources).\n"
             "(C) Radon Labs GmbH 2010.\n"
             "-help         -- display this help\n"
             "-platform     -- select platform (win32, xbox360, wii, ps3)\n"
             "-waitforkey   -- wait for key when complete\n"
             "-force        -- force export (don't check time stamps)\n"
             "-cat          -- select specific category\n"
             "-file         -- select specific file (also needs -cat)\n"
             "-verbose      -- display status info\n");
}

//------------------------------------------------------------------------------
/**
*/
void
N2BatchConverterApp::Run()
{
    // parse command line args
    if (!this->ParseCmdLineArgs())
    {
        return;
    }

    // setup the project info object
    if (!this->SetupProjectInfo())
    {
        return;
    }

    ToolkitUtil::TextureAttrTable table;
    table.Setup("proj:work/textures/batchattributes.xml");
    this->n2BatchConverter.SetExternalTextureAttrTable(&table);

    // perform conversion
    this->n2BatchConverter.Setup(this->logger, this->projectInfo);
    bool result = true;
    if (this->category.IsValid())
    {
        if (this->filename.IsValid())
        {
            result = this->n2BatchConverter.ConvertFile(this->category, this->filename);
        }
        else
        {
            result = this->n2BatchConverter.ConvertCategory(this->category);
        }
    }
    else
    {
        result = this->n2BatchConverter.ConvertAll();
    }
    this->n2BatchConverter.Discard();
}

} // namespace ToolkitUtil