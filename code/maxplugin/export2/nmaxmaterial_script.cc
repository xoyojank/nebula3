//-----------------------------------------------------------------------------
//  nmaxmaterial_script.cc
//
//  (C)2005 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "maxscrpt/maxscrpt.h"
#include "maxscrpt/parser.h"
#include "maxscrpt/definsfn.h"

#include "export2/nmaxmaterial_shd.h"

//-----------------------------------------------------------------------------
/**
    Max script extension to make Nebula custom material scripted plugin.

    @note
        Be sure that the script should be evaluated at once when the 3dsmax startup.
*/
Value* nCreateCustomMaterialPlugin_cf(Value** arg_list, int count)
{
    one_typed_value_local(Value* result);

    check_arg_count(openFile, 0, count);
    //check_arg_count(openFile, 1, count);
    //type_check(arg_list[0], String, "nSetCustomMaterial shaderName");

    // we get shader name from the given paramter.
    //char* shdname = arg_list[0]->to_string();

    // Generate scripted material plugin then execute the script.
    // It make it be shown custom Nebula material in material editor.
    //bool ret = nSetCustomMaterial(shdname);
    bool ret = EvalCustomMaterialPlugin();
    if (false == ret)
        return &undefined;

    return &ok;
}

def_visible_primitive(nCreateCustomMaterialPlugin, "nCreateCustomMaterialPlugin");