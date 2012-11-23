//------------------------------------------------------------------------------
//  n2converterapp.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "n2converterapp.h"
#include "io/textwriter.h"

namespace Toolkit
{
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
N2ConverterApp::N2ConverterApp() :
    logResources(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
N2ConverterApp::ParseCmdLineArgs()
{
    if (ToolkitApp::ParseCmdLineArgs())
    {
        this->n2Converter.SetForceFlag(this->args.GetBoolFlag("-force"));
        this->n2Converter.SetBinaryFlag(!this->args.GetBoolFlag("-xml"));
        this->n2Converter.SetVerbose(this->args.GetBoolFlag("-verbose"));
        this->category = this->args.GetString("-cat", "");
        this->filename = this->args.GetString("-file", "");
        this->logResources = this->args.GetBoolFlag("-logresources");
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
N2ConverterApp::SetupProjectInfo()
{
    if (ToolkitApp::SetupProjectInfo())
    {
        this->n2Converter.SetPlatform(this->platform);
        this->n2Converter.SetSrcDir(this->projectInfo.GetAttr("N2ConverterSrcDir"));
        this->n2Converter.SetDstDir(this->projectInfo.GetAttr("N2ConverterDstDir"));
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
N2ConverterApp::ShowHelp()
{
    n_printf("Nebula3 N2 model file converter.\n"
             "(C) Radon Labs GmbH 2008.\n"
             "-help         -- display this help\n"
             "-platform     -- select platform (win32, xbox360, wii, ps3)\n"
             "-waitforkey   -- wait for key when complete\n"
             "-force        -- force export (don't check time stamps)\n"             
             "-cat          -- select specific category\n"
             "-file         -- select specific file (also needs -cat)\n"
             "-xml          -- output XML files (default is binary)\n"
             "-logresources -- create resource log file in proj directory\n"
             "-verbose      -- display status info\n");
}

//------------------------------------------------------------------------------
/**
*/
void
N2ConverterApp::Run()
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

    // perform conversion
    this->n2Converter.Setup();
    bool result = true;
    if (this->category.IsValid())
    {
        if (this->filename.IsValid())
        {
            result = this->n2Converter.ConvertFile(this->logger, this->category, this->filename);
        }
        else
        {
            result = this->n2Converter.ConvertCategory(this->logger, this->category);
        }
    }
    else
    {
        result = this->n2Converter.ConvertAll(this->logger);
    }
    
    // log resources?
    if (this->logResources)
    {
        Ptr<Stream> stream = IoServer::Instance()->CreateStream("proj:n2conv_resourcelog.txt");
        Ptr<TextWriter> writer = TextWriter::Create();
        writer->SetStream(stream);
        if (writer->Open())
        {
            writer->WriteLines(this->n2Converter.GetUsedResources());
            writer->Close();
        }
    }
    this->n2Converter.Discard();
}

} // namespace Toolkit