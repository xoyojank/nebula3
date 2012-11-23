//------------------------------------------------------------------------------
//  nmaxscript_objattr.cc
//
//  (C)2004 Kim, Hyoun Woo.
//------------------------------------------------------------------------------
#include "base/nmaxlistener.h"
#include "kernel/nfileserver2.h"
#include "expose/nmaxscriptobjattr.h"
#include "tinyxml/tinyxml.h"

//------------------------------------------------------------------------------
/**
*/
nObjectAttribute::nObjectAttribute() :
    commandList(32),
    isOpen(false)
{
}

//------------------------------------------------------------------------------
/**
*/
nObjectAttribute::~nObjectAttribute()
{
    this->ClearCommandList();
}

//------------------------------------------------------------------------------
/**
    @param name xml filename which to be opened.
                The name of the xml file is identical to the nebula object name.
                e.g. ntransformnode -> ntransformnode.xml
*/
void nObjectAttribute::SetFilename(const nString& name)
{
    this->filename = name;
}

//------------------------------------------------------------------------------
/**
    Open specified xml file and parse it.

    To parse xml file, the name of the xml file should be specified first 
    with calling nObjectAttribute::SetFilename().

    @return If there's no xml file to parse, return false.
*/
bool nObjectAttribute::Open()
{
    n_assert(!this->isOpen);
    n_assert(!this->filename.IsEmpty());
    n_assert(this->commandList.IsEmpty());

    if (!this->ParseXmlFile())
    {
        return false;
    }
    this->isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void nObjectAttribute::Close()
{
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
    Remove all added command hash nodes from hash list.
*/
void nObjectAttribute::ClearCommandList()
{
    bool isEmpty;
    while ((!(isEmpty = this->commandList.IsEmpty())))
    {
        Command *actCommand = (Command*)this->commandList.GetHead();
        Command *nextCommand;
        do
        {
            nextCommand = (Command*)actCommand->GetSucc();

            actCommand->Remove();
            n_delete(actCommand);

            actCommand = nextCommand;
        } while (actCommand);
    }
}

//------------------------------------------------------------------------------
/**
    Parsing given xml file. 

    The name of the xml file is identical to the nebula object name.
    e.g. ntransformnode -> ntransformnode.xml
*/
bool nObjectAttribute::ParseXmlFile()
{

    nString mangledPath = nFileServer2::Instance()->ManglePath(this->filename);
    TiXmlDocument doc(mangledPath.Get());
    if (!doc.LoadFile())
    {
        //n_error("nObjectAttribute: Failed to open XML file '%s'", this->filename.Get());
        return false;
    }

    TiXmlHandle docHandle(&doc);
    TiXmlElement* elmRenderPath = docHandle.FirstChildElement("NebulaObject").Element();
    n_assert(elmRenderPath);

    this->name = elmRenderPath->Attribute("name");

    TiXmlElement* elmPass;
    for (elmPass = elmRenderPath->FirstChildElement("Command");
         elmPass != 0;
         elmPass = elmPass->NextSiblingElement("Command"))
    {
        Command* newCmd = n_new(Command);
        // set name
        newCmd->SetName(elmPass->Attribute("name"));
        // set type
        newCmd->StringToType(elmPass->Attribute("type"));

        // set minimum value of the range.
        nString strMin = elmPass->Attribute("min");
        if (!strMin.IsEmpty())
        {
            float min = (float)atof(strMin.Get());
            newCmd->SetMin(min);
        }

        // set maximum value of the range.
        nString strMax = elmPass->Attribute("max");
        if (!strMax.IsEmpty())
        {
            float max = (float)atof(strMax.Get());
            newCmd->SetMax(max);
        }

        this->commandList.AddTail(newCmd);
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
nObjectAttribute::Command::Command() : 
    fMin(-3.40E38f),
    fMax(3.40E38f)
{
}

//------------------------------------------------------------------------------
/**
*/
nObjectAttribute::Command::~Command()
{
}

//------------------------------------------------------------------------------
/**
    Convert a given string to the specific type of command parameter.

    @param str string which parsed from element of xml file.
*/
void nObjectAttribute::Command::StringToType(const char* str)
{
    if (str)
    {
        if (0 == strcmp("float", str))
            this->type = Float;
        else
        if (0 == strcmp("bool", str))
            this->type = Bool;
        else
        if (0 == strcmp("string", str))
            this->type = String;
        else
        if (0 == strcmp("int", str))
            this->type = Int;
        else
        if (0 == strcmp("vector2", str))
            this->type = Vector2;
        else
        if (0 == strcmp("vector3", str))
            this->type = Vector3;
        else
        if (0 == strcmp("vector4", str))
            this->type = Vector4;
        else
        if (0 == strcmp("matrix3", str))
            this->type = Matrix3;
        else
        if (0 == strcmp("matrix4", str))
            this->type = Matrix4;
        else
        if (0 == strcmp("rgb", str))
            this->type = RGB;
        else
        if (0 == strcmp("rgba", str))
            this->type = RGBA;
        else
        if (0 == strcmp("various", str))
            this->type = Various;
        else
            n_listener("Invalid type of string: %s\n", str);
    }
    else
    {
        n_listener("Type for command %s does not exist.\n", this->GetName());
    }
}
