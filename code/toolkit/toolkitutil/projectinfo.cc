//------------------------------------------------------------------------------
//  projectinfo.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "projectinfo.h"
#include "io/ioserver.h"
#include "io/stream.h"
#include "io/xmlreader.h"
#include "system/win32/win32registry.h"
#include "system/win32/win32environment.h"

namespace ToolkitUtil
{
using namespace Util;
using namespace IO;
using namespace System;

//------------------------------------------------------------------------------
/**
*/
ProjectInfo::ProjectInfo() :
    defPlatform(Platform::InvalidPlatform),
    curPlatform(Platform::InvalidPlatform)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ProjectInfo::~ProjectInfo()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ProjectInfo::HasAttr(const Util::String& attrName) const
{
    n_assert(attrName.IsValid());
    n_assert(Platform::InvalidPlatform != this->curPlatform);
    if (this->platformAttrs.Contains(this->curPlatform))
    {
        return this->platformAttrs[this->curPlatform].Contains(attrName);
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
const Util::String&
ProjectInfo::GetAttr(const Util::String& attrName) const
{
    n_assert(attrName.IsValid());
    n_assert(Platform::InvalidPlatform != this->curPlatform);
    if (!this->platformAttrs[this->curPlatform].Contains(attrName))
    {
        n_error("ProjectInfo: attr '%s' does not exists for platform '%s' in projectinfo.xml!",
            attrName.AsCharPtr(), Platform::ToString(this->curPlatform).AsCharPtr());
    }
    return this->platformAttrs[this->curPlatform][attrName];
}

//------------------------------------------------------------------------------
/**
*/
bool
ProjectInfo::IsValid() const
{
    return (this->platformAttrs.Size() > 0);
}

//------------------------------------------------------------------------------
/**
*/
ProjectInfo::Result
ProjectInfo::Setup()
{
    n_assert(!this->IsValid());

    // setup project and toolkit path assign
    String projPath = this->QueryProjectPathFromRegistry();
    if (projPath.IsEmpty())
    {
        return NoProjPathInRegistry;
    }
    String toolkitPath = this->QueryToolkitPathFromRegistry();
    n_assert(toolkitPath.IsValid());
    AssignRegistry::Instance()->SetAssign(Assign("proj", projPath));
    AssignRegistry::Instance()->SetAssign(Assign("toolkit", toolkitPath));

    // parse project info XML file
    Result res = ProjectInfo::Success;
    if (IoServer::Instance()->FileExists("proj:projectinfo.xml"))
    {
        res = this->ParseProjectInfoFile();
    }
    else
    {
        return NoProjectInfoFile;
    }
    return res;
}

//------------------------------------------------------------------------------
/**
*/
void
ProjectInfo::Discard()
{
    this->platformAttrs.Clear();
}

//------------------------------------------------------------------------------
/**
    Parse the projectinfo.xml file which is expected in the project root 
    directory.
*/
ProjectInfo::Result
ProjectInfo::ParseProjectInfoFile()
{
    n_assert(!this->IsValid());

    Ptr<Stream> stream = IoServer::Instance()->CreateStream(URI("proj:projectinfo.xml"));
    Ptr<XmlReader> xmlReader = XmlReader::Create();
    xmlReader->SetStream(stream);
    if (xmlReader->Open())
    {
        // check if it's a valid project info file
        if (!xmlReader->HasNode("/Nebula3/Project"))
        {
            return ProjectFileContentInvalid;
        }
        xmlReader->SetToNode("/Nebula3/Project");
        n_assert(xmlReader->HasAttr("defaultPlatform"));
        this->defPlatform = Platform::FromString(xmlReader->GetString("defaultPlatform"));
        
        // for each platform...
        if (xmlReader->SetToFirstChild("Platform")) do
        {
            // setup a new set of platform attributes
            n_assert(xmlReader->HasAttr("name"));
            Platform::Code platform = Platform::FromString(xmlReader->GetString("name"));
            n_assert(!this->platformAttrs.Contains(platform));
            Dictionary<String,String> emptyDict;
            this->platformAttrs.Add(platform, emptyDict);

            // load attributes
            if (xmlReader->SetToFirstChild("Attr")) do
            {
                n_assert(xmlReader->HasAttr("name"));
                n_assert(xmlReader->HasAttr("value"));
                String attrName = xmlReader->GetString("name");
                String attrValue = xmlReader->GetString("value");
                if (this->platformAttrs[platform].Contains(attrName))
                {
                    n_error("projectinfo.xml: multiple definitions of attr '%s' in platform section '%s'!\n",
                        attrName.AsCharPtr(), Platform::ToString(platform).AsCharPtr());
                }
                this->platformAttrs[platform].Add(attrName, attrValue);
            }
            while (xmlReader->SetToNextChild("Attr"));
        }
        while (xmlReader->SetToNextChild("Platform"));
        return Success;
    }
    else
    {
        return CouldNotOpenProjectInfoFile;
    }
}

//------------------------------------------------------------------------------
/**
    Query the project path from the registry, the registry key is set
    by the "Nebula2 Toolkit For Maya". If no key is found, the method
    will return "home:".
*/
String
ProjectInfo::QueryProjectPathFromRegistry()
{
    String projDirectory;
    if (Win32Registry::Exists(Win32Registry::CurrentUser, "Software\\RadonLabs\\Toolkit", "project"))
    {
        projDirectory = Win32Registry::ReadString(Win32Registry::CurrentUser, "Software\\RadonLabs\\Toolkit", "project");
    }
    else
    {
        projDirectory = "home:";
    }
    return projDirectory;
}

//------------------------------------------------------------------------------
/**
    Query the toolkit path from the registry, the registry key is set
    by the "Nebula2 Toolkit For Maya". If no key is found, the method
    will return "home:".
*/
String
ProjectInfo::QueryToolkitPathFromRegistry()
{
    String toolkitDirectory;
    if (Win32Registry::Exists(Win32Registry::CurrentUser, "Software\\RadonLabs\\Toolkit", "path"))
    {
        toolkitDirectory = Win32Registry::ReadString(Win32Registry::CurrentUser, "Software\\RadonLabs\\Toolkit", "path");
    }
    else
    {
        toolkitDirectory = "home:";
    }
    return toolkitDirectory;
}

//------------------------------------------------------------------------------
/**
*/
String
ProjectInfo::GetErrorString(Result res) const
{
    switch (res)
    {
        case ProjectInfo::NoProjPathInRegistry:
            return String("ERROR: no Nebula toolkit entries found in registry!");
        case ProjectInfo::CouldNotOpenProjectInfoFile:
            return String("ERROR: could not open projectinfo.xml file in project directory!");
        case ProjectInfo::ProjectFileContentInvalid:
            return String("ERROR: content of projectinfo.xml is invalid!");
        case ProjectInfo::NoProjectInfoFile:
            return String("ERROR: No projectinfo.xml file found in current project!");
        default:
            return String("ERROR: unknown error from ProjectInfo object!\n");
    }
}

//------------------------------------------------------------------------------
/**
    Interprets an attribute value as a path which may contain registry
    or environment aliases in the following form:

    $(reg:HKEY_LOCAL_MACHINE/bla/blub)/bla/bla
    $(env:HOME_DIR)/bla/bla
*/
String
ProjectInfo::GetPathAttr(const String& attrName) const
{
    String attrVal = this->GetAttr(attrName);
    if ((attrVal.Length() > 0) && (attrVal[0] == '$'))
    {
        // need to resolve env or reg variable
        IndexT varStartIndex = attrVal.FindCharIndex('(');
        IndexT varEndIndex   = attrVal.FindCharIndex(')');
        n_assert(varStartIndex == 1);
        n_assert(varEndIndex > varStartIndex);
        String varString = attrVal.ExtractRange(varStartIndex + 1, (varEndIndex - varStartIndex) - 1);

        // registry or environment variable?
        if (String::MatchPattern(varString, "reg:*"))
        {
            // treat as registry key
            String regString = varString.ExtractToEnd(4);
            Array<String> regTokens = regString.Tokenize("/");
            n_assert(regTokens.Size() > 2);
            Win32Registry::RootKey rootKey = Win32Registry::AsRootKey(regTokens[0]);
            String regKeyName = regTokens.Back();
            regTokens.EraseIndex(0);
            regTokens.EraseIndex(regTokens.Size() - 1);
            String regPath = String::Concatenate(regTokens, "\\");

            // read actual registry value
            String regValue = Win32Registry::ReadString(rootKey, regPath, regKeyName);

            // build actual file name
            String retValue = regValue;
            retValue.Append(attrVal.ExtractToEnd(varEndIndex + 1));
            return retValue;
        }
        else if (String::MatchPattern(varString, "env:*"))
        {
            // an environment variable
            String envString = varString.ExtractToEnd(4);
            if (Win32Environment::Exists(envString))
            {
                String envValue = Win32Environment::Read(envString);
                String retValue = envValue;
                retValue.Append(attrVal.ExtractToEnd(varEndIndex + 1));
                return retValue;
            }
        }
    }
    
    // fallthrough: format error, return string as is
    return attrVal;
}

} // namespace ToolkitUtil