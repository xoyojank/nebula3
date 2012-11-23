//-----------------------------------------------------------------------------
/**
    @file nmaxexposecmds.cc

    (C) 2004 Kim, Hyoun Woo

    nMaxExpose is licensed under the terms of the Nebula License.
*/

#include "base/nmaxlistener.h"
#include "expose/nmaxexpose.h"


//static bool getInArgs(nCmd* cmd, Tab<FPValue*> *val);
//static void putOutSingleArg(nArg* arg, Tab<FPValue*>* out);
//static Tab<FPValue*>* putOutArgs(nCmd* cmd);

//-----------------------------------------------------------------------------
/**
    @class nNebulaInterface
    @ingroup NMaxExposeContrib

    @brief The implementation class.

*/
class nNebulaInterface : public INebulaInterface
{
public:
    //-- enum type of function ID --
    enum {
        fn_new,
        fn_delete,
        fn_dir,
        fn_sel,
        fn_lookup,
        fn_exists,
        fn_nprint,
        fn_call,
        fn_callv,
    };

private:
    DECLARE_DESCRIPTOR(nNebulaInterface)

    //-- function map --
    BEGIN_FUNCTION_MAP
        FN_2  (fn_new, TYPE_STRING, New, TYPE_STRING, TYPE_STRING);
        FN_1  (fn_delete, TYPE_BOOL, Delete, TYPE_STRING);
        FN_0  (fn_dir, TYPE_STRING_TAB_BV, Dir);
        FN_1  (fn_sel, TYPE_STRING, Sel, TYPE_STRING);
        FN_1  (fn_lookup, TYPE_STRING, Lookup, TYPE_STRING);
        FN_1  (fn_exists, TYPE_BOOL, Exists, TYPE_STRING);
        VFN_1 (fn_nprint, NPrint, TYPE_STRING);
        FN_2  (fn_call, TYPE_FPVALUE_TAB, Call, TYPE_STRING, TYPE_FPVALUE_TAB);
        FN_1  (fn_callv, TYPE_FPVALUE_TAB, CallV, TYPE_STRING);
    END_FUNCTION_MAP

    //-- function declaration --
    TCHAR* New (TCHAR* className, TCHAR* objectName);
    BOOL Delete(TCHAR* objectName);

    Tab<TCHAR*> Dir();
    TCHAR* Sel_(TCHAR* objectName, bool setCwd);
    TCHAR* Sel(TCHAR* objectName);
    TCHAR* Lookup (TCHAR* objectName);

    BOOL Exists(TCHAR* objectName);
    void NPrint(TCHAR* msg);

    Tab<FPValue*>* Call(TCHAR* command, Tab<FPValue*> *val);
    Tab<FPValue*>* CallV(TCHAR* command);

};

//-----------------------------------------------------------------------------
/**
    Interface definitions.
*/
static nNebulaInterface nebulaInterfaceFP (
    N_MAX_SCRIPT_INTERFACE_ID, _T("foo"), 0, &maxScriptInterfaceClassDesc2, FP_CORE,//FP_CORE + FP_MIXIN,

    // 'new'
    nNebulaInterface::fn_new, _T("New"), 0, TYPE_STRING, 0, 2,
        _T("classname"), 0, TYPE_STRING,
        _T("objectname"), 0, TYPE_STRING,

    // 'delete'
    nNebulaInterface::fn_delete, _T("Delete"), 0, TYPE_BOOL, 0, 1,
        _T("objectname"), 0, TYPE_STRING,

    // 'dir'
    nNebulaInterface::fn_dir, _T("Dir"), 0, TYPE_STRING_TAB_BV, 0, 0,

    // 'sel'
    nNebulaInterface::fn_sel, _T("Sel"), 0, TYPE_STRING, 0, 1,
        _T("objectname"), 0, TYPE_STRING,

    // 'lookup'
    nNebulaInterface::fn_lookup, _T("Lookup"), 0, TYPE_STRING, 0, 1,
        _T("objectname"), 0, TYPE_STRING,

    // 'exists'
    nNebulaInterface::fn_exists, _T("Exists"), 0, TYPE_BOOL, 0, 1,
        _T("objectname"), 0, TYPE_STRING,

    // 'nprint'
    nNebulaInterface::fn_nprint, _T("nprint"), 0, TYPE_VOID, 0, 1,
        _T("message"), 0, TYPE_STRING,

    // 'call'
    nNebulaInterface::fn_call, _T("Call"), 0, TYPE_FPVALUE_TAB, 0, 2,
        _T("command"), 0, TYPE_STRING,
        _T("inargs"), 0, TYPE_FPVALUE_TAB,

    // 'callv'
    nNebulaInterface::fn_callv, _T("CallV"), 0, TYPE_FPVALUE_TAB, 0, 1,
        _T("command"), 0, TYPE_STRING,

    end
);

//-----------------------------------------------------------------------------
FPInterfaceDesc* nMaxExpose::GetDesc()
{
    return &nebulaInterfaceFP;
}

//-----------------------------------------------------------------------------
/**
    e.g.
    nebula2.new "nshapenode" "/usr/scene"

    @param className     nebula class name of object to create.
    @param objectName    string of object's name.

    @return
*/
TCHAR* nNebulaInterface::New(TCHAR *className, TCHAR *objectName)
{
    TCHAR* result = _T("");

    if (!className)
    {
        n3_listener ("NULL classname.\n");
        return _T("");
    }
    if (!objectName)
    {
        n3_listener ("NULL objectname.\n");
        return _T("");
    }

    //nKernelServer* kernelServer = nKernelServer::Instance();

    //const char* class_name  = _T(className);
    //const char* object_name = _T(objectName);

    //nRoot* o = kernelServer->NewNoFail(class_name, object_name);
    //if (o)
    //{
    //    result = const_cast<char*>(o->GetName ());
    //}
    //else
    //    n3_listener ("NEW: Unable to create a NebulaObject or type '%s'\n", class_name);

    return result;
}

//-----------------------------------------------------------------------------
/**
    Delete an object by object name.

    @param objectName 'path' of object which to be deleted.
    @return Return true on success otherwise false.
*/
BOOL nNebulaInterface::Delete(TCHAR* objectName)
{
    const char* objName = const_cast<char*>(_T(objectName));
    //nRoot* o = nKernelServer::Instance()->Lookup(objName);

    //if (o)
    //{
    //    o->Release();
    //}
    //else
    //{
    //    n3_listener("Could not lookup object '%s'\n", objName);
    //    return FALSE;
    //}

    return TRUE;
}

//-----------------------------------------------------------------------------
/**
    Display objects in the current graph path.

    @return string array of object name. The string should be returned by value,
            if not, there's **system exception** on maxscript. (Use TYPE_STRING_TAB_BR)

*/
Tab<TCHAR*> nNebulaInterface::Dir()
{
    Tab<TCHAR*> val;

    //nRoot* cwd = nKernelServer::Instance()->GetCwd();
    //if (cwd)
    //{
    //    //n3_listener ("[");

    //    nRoot* o;
    //    int at = 0;
    //    // Display contents of path acquired from GetCwd()
    //    for (o=cwd->GetHead(); o; o=o->GetSucc())
    //    {
    //        char* name = const_cast<char*>(o->GetName());
    //        val.Insert (at++, 1, &_T(name));

    //        //n3_listener ("'%s' ", name);
    //    }

    //    //n3_listener ("]\n");
    //}

    return val;
}

//-----------------------------------------------------------------------------
/**
    Move within the current graph path.
    Called in Sel and Lookup function.

    @see nNebulaInterface::Sel
    @see nNebulaInterface::Lookup
*/
TCHAR* nNebulaInterface::Sel_(TCHAR* objectName, bool setCwd)
{
    const char* objName = const_cast<char*>(_T(objectName));
    if (*objName == '\0')
    {
        n3_listener ("NULL object name is used.\n");
        return _T("");
    }

    //nRoot* o = nKernelServer::Instance()->Lookup(_T(objectName));
    //if (o)
    //{
    //    if (setCwd)
    //        nKernelServer::Instance()->SetCwd(o);
    //}
    //else
    //{
    //    n3_listener("SEL: Unable to select path (perhaps it doesn't exist)\n");
    //    return _T("");
    //}

    //// return current selected object's name.
    TCHAR* ret;
    //if (setCwd)
    //    ret = _T(const_cast<char*>(nKernelServer::Instance()->GetCwd()->GetName()));
    //else
    //    ret = _T(const_cast<char*>(o->GetName ()));

    return ret;
}

//-----------------------------------------------------------------------------
/**
    Set the current working directory of the NOH to the specified path.

    @param 'path' of object whici to select. Selected object is specified for
            current working directory.
    @return current selected object name on success and empty string on failure.
*/
TCHAR* nNebulaInterface::Sel (TCHAR* objectName)
{
    return Sel_(objectName, true);
}

//-----------------------------------------------------------------------------
/**
    Return object name if there's the object in given path.

    @param  'path' of object whici to select. Not likely 'sel' command, selected
            object is not specified for current working directory.
    @return current selected object name on success and empty string on failure.
*/
TCHAR* nNebulaInterface::Lookup (TCHAR* objectName)
{
    return Sel_(objectName, false);
}
//-----------------------------------------------------------------------------
/**
    Implements the 'exists' command.

    @param objectName 'path' of the object which to check that object is exist
                      or not.
    @return Return true if the object is exist or false it is not.
*/
BOOL nNebulaInterface::Exists(TCHAR* objectName)
{
    const char* objName = const_cast<char*>(_T(objectName));
    if (*objName == '\0')
    {
        n3_listener ("NULL object name is used.\n");
        return FALSE;
    }

    //nRoot* o = nKernelServer::Instance()->Lookup(objectName);
    //if (o)
    //{
    //    return TRUE;
    //}

    return FALSE;
}

//-----------------------------------------------------------------------------
/**
    Put string out on Nebula's log file(if it exist) and listener.
    @param msg
*/
void nNebulaInterface::NPrint(TCHAR* msg)
{
    char* text = _T(msg);
    n_printf ("%s", text);

    n3_listener("%s\n", text);
}

//-----------------------------------------------------------------------------
/**
    Function for void argument type of command.
    e.g. nebula2.setv ".getcmds"
    same result: nebula2.set ".getcmds" #()

    @note
    maxscript FnPub system doesn't resolve function which has same function
    name but different for it's arguments ,if both functions have same script name.
    3dsmax only recognizes the first registered function if both of function have
    same scripter name.

    @param command Nebula script command
    @return out-arguments. It's array type of maxscript.
*/
Tab<FPValue*>* nNebulaInterface::CallV(TCHAR* command)
{
    Tab<FPValue*>* result = NULL;
    return Call (command, result);
}

//-----------------------------------------------------------------------------
/**
    e.g. nebula2.set "/usr/scene/object.setpos" #(100.0, 20.0, 0.0)

    FPValue* (TYPE_FPVALUE) can only hold same type of argument in it's Tab<>.
    So, for using variant type of arguements, we need Tab<FPValue*>*

    @param command Nebula script command
    @param val in-arguments. It should be array type of maxscript.
    @return out-arguments. It's array type of maxscript.
*/
Tab<FPValue*>* nNebulaInterface::Call(TCHAR* command, Tab<FPValue*> *val)
{
    Tab<FPValue*>* result = 0;

    //char cmd[N_MAXPATH];
    //char *commandName;
    //char *objectName;
    //char *dot;

    //char* tempcmd = _T(command);

    //if (tempcmd)
    //{
    //    strcpy(cmd, tempcmd);
    //    dot = strchr(cmd, '.');

    //    while (dot && ((dot[1] == '.') || (dot[1] == '/')))
    //        dot = strchr(++dot, '.');

    //    if (dot)
    //    {
    //        *dot = '\0';
    //        objectName = cmd;
    //        if (objectName == dot)
    //            objectName = NULL;
    //        commandName = dot + 1;
    //    }
    //    else
    //    {
    //        objectName = NULL;
    //        commandName = cmd;
    //    }

    //    if (*commandName != '\0')
    //    {
    //        //find object to invoke command on
    //        nRoot *o;
    //        if (objectName)
    //            o = nKernelServer::Instance()->Lookup(objectName);
    //        else
    //            o = nKernelServer::Instance()->GetCwd();

    //        if (o)
    //        {
    //            // From a Nebula Cmd object from command string
    //            nClass *cl = o->GetClass();
    //            nCmdProto *cmdProto = (nCmdProto*)cl->FindCmdByName(commandName);

    //            if (cmdProto)
    //            {
    //                // Invoke the command
    //                nCmd *ncmd = cmdProto->NewCmd();
    //                n_assert (ncmd);

    //                if (!getInArgs(ncmd, val))
    //                {
    //                    n3_listener ("Broken input args, object '%s', command '%s'\n",
    //                                o->GetName(), commandName);
    //                }
    //                else
    //                if (o->Dispatch(ncmd))
    //                {
    //                    // let object handle the command
    //                    result = putOutArgs(ncmd);
    //                }
    //                else
    //                {
    //                    n3_listener ("Dispatch error, object '%s', command '%s'\n",
    //                                o->GetName(), commandName);
    //                }
    //                // In any case, cleanup the cmd object
    //                cmdProto->RelCmd(ncmd);
    //            }
    //            else
    //            {
    //                // the object doesn't know about the command
    //                n3_listener ("Unknown command, object '%s', command '%s'\n",
    //                            o->GetName(), commandName);
    //            }
    //        }
    //        else
    //        {
    //            n3_listener ("Unable to acruire current object.\n");
    //        }
    //    }
    //    else
    //    {
    //        n3_listener ("No command.\n"); // command is NULL
    //    }
    //}
    //else
    //{
    //    // Unable to acquire command string.
    //    n3_listener ("Unable to acquire command string.\n");
    //}

    return result;
}

////-----------------------------------------------------------------------------
///**
//    Processing in-arguments of script command.
//
//    @param cmd
//    @param val
//*/
//static
//bool getInArgs(nCmd* cmd, Tab<FPValue*>* val)
//{
//    //FIXME: check here or not?
//    if (!val)
//    {
//        // SetV called Set, so the val argument is invalid.
//        // There's no argument so we just pass true.
//        return true;
//    }

    //// Get number of arguments of this command.
    //int numArgs = cmd->GetNumInArgs();

    //if (numArgs == val->Count())
    //{
    //    // read out arguments
    //    FPValue* v;
    //    nArg *arg;
    //    cmd->Rewind();
    //    for (int argIndex=0; argIndex<numArgs; argIndex++)
    //    {
    //        v = *(val->Addr(argIndex));

    //        bool argOk = false;
    //        arg = cmd->In();
    //        switch (arg->GetType())
    //        {
    //        case nArg::Type::Int:
    //            {
    //                if (v->type != TYPE_INT)
    //                    n3_listener("WARNING: argument '%d' and command argument [int] is diffferent type.\n", argIndex);

    //                int n = v->i;
    //                arg->SetI(n);

    //                argOk = true;
    //            }
    //            break;
    //        case nArg::Type::Float:
    //            {
    //                if (v->type != TYPE_FLOAT)
    //                    n3_listener("WARNING: argument '%d' and command argument [string] is diffferent type.\n", argIndex);

    //                float f = v->f;
    //                arg->SetF(f);

    //                argOk = true;
    //            }
    //            break;

    //        case nArg::Type::String:
    //            {
    //                if (v->type != TYPE_STRING)
    //                    n3_listener("WARNING: argument '%d' and command argument [string] is diffferent type.\n", argIndex);

    //                TCHAR* str = v->s;
    //                arg->SetS(_T(str));

    //                argOk = true;
    //            }
    //            break;

    //        case nArg::Type::Bool:
    //            {
    //                if (v->type != TYPE_BOOL)
    //                    n3_listener("WARNING: argument '%d' and command argument [bool] is diffferent type.\n", argIndex);

    //                bool b = v->b;
    //                arg->SetB(b);

    //                argOk = true;
    //            }
    //            break;

    //        case nArg::Type::Object:
    //            n3_listener ("Object values aren't acceptable in arguments.\n");
    //            argOk = false;
    //            break;

    //        case nArg::Type::List:
    //            n3_listener ("List values aren't acceptable in arguments.\n");
    //            argOk = false;
    //            break;

    //        case nArg::Type::Void:
    //            break;
    //        }// end of switch

    //        if (!argOk)
    //            return false;
    //    }

    //    return true;
    //}
//
//    return false;
//}

////-----------------------------------------------------------------------------
///**
//    Processing out-arguments of script command.
//
//    @param cmd
//*/
//static
//Tab<FPValue*>* putOutArgs(nCmd* cmd)
//{
//    //HACK: it's the only way we can return array of FPValue values.
//    //      there's is no way to retrun by value.
//    Tab<FPValue*>* result = new Tab<FPValue*>;
//
//    FPValue val;
//    nArg* arg;
//
//    int numArgs = cmd->GetNumOutArgs();
//    cmd->Rewind();
//
//    for (int i=0; i<numArgs; i++)
//    {
//        arg = cmd->Out();
//
//        putOutSingleArg (arg, result);
//    }
//
//    return result;
//}

////-----------------------------------------------------------------------------
///**
//    Process arguments which to return.
//
//    @param arg
//    @param out
//*/
//static
//void putOutSingleArg(nArg* arg, Tab<FPValue*>* out)
//{
//    switch (arg->GetType())
//    {
//    case nArg::Type::Int:
//        {
//            FPValue* result = new FPValue((ParamType2)TYPE_INT, arg->GetI());
//            out->Append(1, &result);
//        }
//        break;
//
//    case nArg::Type::Float:
//        {
//            FPValue* result = new FPValue((ParamType2)TYPE_FLOAT, arg->GetF());
//            out->Append(1, &result);
//        }
//        break;
//
//    case nArg::Type::String:
//        {
//            FPValue* result = new FPValue(TYPE_STRING, const_cast<char*>(arg->GetS()));
//            out->Append (1, &result);
//        }
//        break;
//
//    case nArg::Type::Bool:
//        {
//            FPValue* result = new FPValue((ParamType2)TYPE_BOOL, arg->GetB());
//
//            out->Append(1, &result);
//        }
//        break;
//
//    case nArg::Type::Object:
//        {
//            // FIXME: return object name for the object type
//            //        there's no way to return nebula-object type at this moment.
//
//            nString s;
//            nRoot* o = (nRoot*)arg->GetO();
//            if (o)
//            {
//                s = o->GetFullName();
//            }
//            else
//                s = "";
//
//            FPValue* result = new FPValue;
//            result->type    = TYPE_STRING;
//
//            //HACK: FPValue type doesn't allocate memory for string type
//            //      so, we should allocate memory explicitly.
//            result->s = ::strdup(s.Get());
//            out->Append(1, &result);
//        }
//        break;
//
//    case nArg::Type::List:
//        {
//            nArg *outArgs, *a;
//            int count;
//
//            count = arg->GetL(outArgs);
//            a = outArgs;
//
//            for (int i=0; i<count; i++)
//            {
//                putOutSingleArg(a, out);
//                a++;
//            }
//        }
//        break;
//
//    case nArg::Type::Void:
//        break;
//    }
//
//    //return result;
//}
//-----------------------------------------------------------------------------
// EOF
//-----------------------------------------------------------------------------
