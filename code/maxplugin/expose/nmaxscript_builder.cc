//------------------------------------------------------------------------------
//  nmaxscript_builder.cc
//
//  (C)2004 Kim, Hyoun Woo.
//------------------------------------------------------------------------------
#include "base/nmaxlistener.h"
#include "kernel/ncmd.h"
#include "kernel/nkernelserver.h"

#include "expose/nmaxscriptbuilder.h"
#include "expose/nmaxscriptobjattr.h"
#include "expose/nmaxscriptparams.h"

static 
void MakeControl(nObjectAttribute&, nCmdProto*, nString&, nString&, nString&);
static 
void AddVariousParam(const nObjectAttribute::Command*, nCmd*, nString&, nString&, nString&);
static 
void ArgToAttr(const nObjectAttribute::Command*, nCmd*, nString&, nString&, nString&);

//------------------------------------------------------------------------------
/**
    FIXME: we could make a filter with xml file.
*/
bool FilterCmds(nCmdProto* cmdproto)
{
    static const char* rootcmds [22] = {
    "getrefcount",
    "getclass",
    "isa",
    "isinstanceof",
    "getcmds",
    "save",
    "saveas",
    "clone",
    "getparent",
    "gethead",
    "gettail",
    "getsucc",
    "getpred",
    "getchildren",
    "setname",
    "getname",
    "getfullname",
    "getrelpath",
    "find",
    "sort",
    "getinstancesize",
    "gettreesize"
    };

    const char* cmdname;
    for (int i=0; i<22; i++)
    {
        cmdname = cmdproto->GetName();
        if (strcmp(rootcmds[i], cmdname) == 0)
            return true;
    }
    
    return false;
}

//------------------------------------------------------------------------------
/**
    @param objPath the path of nebula object which to create UI of that
*/
char* nMaxScriptBuilder::BuildMaxScriptFromNebulaObject(const char* objPath)
{
    // find nebula object with given path of the object and
    // get pointer of nClass 
    nRoot* object = nKernelServer::Instance()->Lookup(objPath);

    nString strAttribute =  "attributes \"";
    strAttribute += "Nebula Object Editor";
    strAttribute += "\"\n(\n";

    //nString strParam     = "\tparameters main rollout:params\n\t(\n";
    //nString strRollout   = "\trollout params \"";
    //strRollout += object->GetName();
    //strRollout +=  " Custom Attributes\"\n\t(\n";
    nString strParam;
    nString strRollout;

    nString strSetupBody = "(\n\t\t";
  
    nString strOnCreate =  "\ton create do \n\t";
    nString strOnUpdate =  "\ton update do \n\t";

    nClass* cl = object->GetClass();

    nStack<nClass*> stackClass;
    stackClass.Push(cl);

    //nClass *superClass, *curClass;
    //superClass = curClass = cl;

    //while(superClass)
    //{
    //    superClass = curClass->GetSuperClass();
    //    if (superClass)
    //    {
    //        stackClass.Push(superClass);
    //    }

    //    curClass = superClass;
    //}
    nClass* cur = cl;
    nClass* next = 0;
    while(1)
    {
        next = cur->GetSuperClass();
        if (next)
        {
            stackClass.Push(next);
        }
        else
            break;

        cur = next;
    }


    while(!stackClass.IsEmpty())
    {
        nClass* clazz = stackClass.Pop();

        if (clazz)
        {
//--> begin recursive part
            //nString param     = "\tparameters main rollout:params\n\t(\n";
            nString param     = "\tparameters ";
            param += clazz->GetName();//main 
            param += "_main ";
            param += "rollout:";
            param += clazz->GetName();//params
            param += "\n\t(\n";

            //nString rollout   = "\trollout params \"";
            nString rollout   = "\trollout ";
            rollout += clazz->GetName();//params \"";
            rollout += " \"";
            rollout += object->GetName();
            //rollout +=  " Custom Attributes\"\n\t(\n";
            rollout += " ";
            rollout += clazz->GetName();
            rollout += "\"\n\t(\n";

            // read the xml file which of name is same to the object's classname.
            // e.g. if the classname is 'ntransformnode', find and read 'ntransformnode.xml'
            nObjectAttribute objAttr;
            
            nString xmlFile;
            xmlFile += "xml:";
            xmlFile += clazz->GetName();
            xmlFile += ".xml";

            objAttr.SetFilename(xmlFile);
               
            if (!objAttr.Open())
            {
                n_listener("nMaxScriptBuilder: %s.xml file does not exist.\n", clazz->GetName());
            }

            // get cmds list of the object 
            nHashList* lstCmdProto = clazz->GetCmdList();
            if (lstCmdProto != NULL)
            { 
                const char* cmd_proto;

                nCmdProto* cp;
                for (cp = (nCmdProto*)lstCmdProto->GetHead(); 
                    cp;
                    cp = (nCmdProto*)cp->GetSucc())
                { // per each of command

                    cmd_proto = cp->GetProtoDef();

                    // fileter nRoot's commands.
                    if (FilterCmds(cp))
                        continue;

                    if (cp->GetNumInArgs() > 0)
                    {
                        MakeControl(objAttr, cp, param, rollout, strSetupBody);
                    }
                    else
                    {
                        // given nebula object has no args
                        rollout += "\t\t";
                        //strRollout += "label NoArgsLabel \"There are no arguments for"
                        //        " this object\n\t\t\"";
                        rollout += "\n";
                    }
                }//end of for.
            }

            objAttr.Close();

            param   += "\n\t)";
            param   += "\n";
            rollout += "\n\t)";
            rollout += "\n";

            strParam += param;
            strRollout += rollout;
        }
    }
//--> end recursive part

    strSetupBody += "\n\t)";

    strAttribute += strParam + "\n\n";
    strAttribute += strRollout + "\n\n";
    strAttribute += strOnCreate + strSetupBody + "\n\n";
    strAttribute += strOnUpdate + strSetupBody + "\n\n";
    // )
    strAttribute += "\n)\n";

    const char* ret = strAttribute.Get();

    return strdup(ret);
}

//------------------------------------------------------------------------------
/**
*/
static
void MakeControl(nObjectAttribute& objAttr, nCmdProto* cmdProto, nString& param, nString& rollout, nString& setupbody)
{
    nCmd* cmd = cmdProto->NewCmd();

    // make a label of this command. (this label will be put into rollout section)
    //nCmdProto* proto = cmd->GetProto();
    nString commandName = cmdProto->GetName();
    rollout += "\t\t";
    rollout += "label " + commandName + "Label \"" + commandName + 
        "\" align:#left offset:[0,6]\n\t\t";

    const nObjectAttribute::Command* ncmd = 0;
    nObjectAttribute::ArgType t;

    if (objAttr.IsOpen())
    {
        ncmd = objAttr.Find(commandName.Get());
        t = ncmd->GetType();
    }
    else
    {
        // if we can't open any existing xml file,
        // treat it as 'various' type.
        t = nObjectAttribute::Various;
    }

    cmd->Rewind();

    nArg *arg;

    switch(t)
    {
    case nObjectAttribute::Float:
        arg = cmd->In();
        AddFloatParam(ncmd, cmd, arg, param, rollout, setupbody);
        break;
    case nObjectAttribute::Bool:
        arg = cmd->In();
        AddBoolParam(ncmd, cmd, arg, param, rollout, setupbody);
        break;
    case nObjectAttribute::String:
        arg = cmd->In();
        AddStringParam(ncmd, cmd, arg, param, rollout, setupbody);
        break;
    case nObjectAttribute::Int:
        arg = cmd->In();
        AddIntParam(ncmd, cmd, arg, param, rollout, setupbody);
        break;
    case nObjectAttribute::Vector2:
        //arg = cmd->In();
        AddVector2Param(ncmd, cmd, param, rollout, setupbody);
        break;
    case nObjectAttribute::Vector3:
        //arg = cmd->In();
        AddVector3Param(ncmd, cmd, param, rollout, setupbody);
        break;
    case nObjectAttribute::Vector4:
        //arg = cmd->In();
        AddVector4Param(ncmd, cmd, param, rollout, setupbody);
        break;
    case nObjectAttribute::Matrix3:
        break;
    case nObjectAttribute::Matrix4:
        break;
    case nObjectAttribute::RGB:
        AddRGBParam(ncmd, cmd, param, rollout, setupbody);
        break;
    case nObjectAttribute::RGBA:
        AddRGBAParam(ncmd, cmd, param, rollout, setupbody);
        break;
    case nObjectAttribute::Various:
        AddVariousParam(ncmd, cmd, param, rollout, setupbody);
        break;
    default:
        n_listener("Unknown type in %s.xml file: %s command.\n", 
            objAttr.GetName().Get(), commandName.Get());
        break;
    }

    param += "\n";
    rollout += "\n";

    cmdProto->RelCmd(cmd);
}

//------------------------------------------------------------------------------
/**
*/
static
void AddVariousParam(const nObjectAttribute::Command* ncmd, nCmd* cmd, 
    nString& param, nString& rollout, nString& setupbody)
{
    n_assert(cmd);

    int uiSetupBodyLength;
    for(int i=0; i<cmd->GetNumInArgs(); i++)
    {
        uiSetupBodyLength = setupbody.Length();

        ArgToAttr(ncmd, cmd, param, rollout, setupbody);

        param   += "\n";
        rollout += "\n";

        if (uiSetupBodyLength < setupbody.Length())
            setupbody += "\n\t\t";
    }
}

//------------------------------------------------------------------------------
/**
*/
static
void ArgToAttr(const nObjectAttribute::Command* ncmd, nCmd* cmd, 
   nString& param, nString& rollout, nString& setupbody)
{
    n_assert(cmd);

    nArg *arg = cmd->In();
    nArg::Type type = arg->GetType();

    switch (type)
    {
    case nArg::Void:
        break;
    case nArg::Int:
        AddIntParam(ncmd, cmd, arg, param, rollout, setupbody);
        break;
    case nArg::Float:
        AddFloatParam(ncmd, cmd, arg, param, rollout, setupbody);
        break;
    case nArg::String:
        AddStringParam(ncmd, cmd, arg, param, rollout, setupbody);
        break;
    case nArg::Bool:
        AddBoolParam(ncmd, cmd, arg, param, rollout, setupbody);
        break;
    //case nArg::Object:
    //    break;
    //case nArg::List:
    //    break;
    //case nArg::Float4:
    //    //AddFloat4Param();
    //    break;
    //case nArg::Matrix44:
    //    //AddMatrix4Param();
    //    break;
    default:
        //FIXME: should support NULL type rollout? (for unknown type of inargs)
        break;
    }
}

