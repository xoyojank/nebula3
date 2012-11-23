//------------------------------------------------------------------------------
//  nmaxscript_callables.cc
//
//  (C)2004 Kim, Hyoun Woo.
//------------------------------------------------------------------------------
#include "expose/nmaxscriptbuilder.h"

#include "maxscrpt/maxscrpt.h"
#include "maxscrpt/parser.h"
#include "maxscrpt/definsfn.h"

//-----------------------------------------------------------------------------
/**
    Scripter-Callable function.
*/
Value* get_maxscript_def_source_cf(Value** arg_list, int count)
{
    one_typed_value_local(Value* result);

    check_arg_count(openFile, 1, count);
    type_check(arg_list[0], String, "GetScriptFromNebulaObject objectpath");

    char* objpath = arg_list[0]->to_string();

    Value* result;

    char* ret = nMaxScriptBuilder::BuildMaxScriptFromNebulaObject(objpath);

    if(strlen(ret) )
    {
        char* buf = new char[strlen(ret) + 1];
        sprintf(buf, "%s", (const char*)ret);
        result = new String(buf);

        //free(ret);
        //delete[] buf;
    }
    else
    {
        result = &undefined;
    }

    return_value(result);
}

//-----------------------------------------------------------------------------
//  function registering macro for Scripter-Callable function.
//-----------------------------------------------------------------------------
def_visible_primitive(get_maxscript_def_source, "GetScriptFromNebulaObject");

//-----------------------------------------------------------------------------
//  EOF
//-----------------------------------------------------------------------------

