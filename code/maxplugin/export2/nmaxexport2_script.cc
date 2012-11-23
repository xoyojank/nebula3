//-----------------------------------------------------------------------------
//  nmaxexport2_script.cc
//
//  (C)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include <maxscrpt/maxscrpt.h>
#include <maxscrpt/parser.h>
#include <maxscrpt/definsfn.h>

#include "export2/nmax.h"
#include "base/nmaxlistener.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxdirdlg.h"

#include "export2/nmaxoptionscriptext.h"

//-----------------------------------------------------------------------------
/**
    max script which exports the given node.

      - 1st in-arg - filename to save the node.
      - 2nd in-arg - node to export.
*/
Value* nExportNode_cf(Value** arg_list, int count)
{
    check_arg_count(nExportScene, 3, count);

    // filename which to used for saving a scene.
    char* filename = arg_list[0]->to_string();
    if (!filename)
    {
        n_listener("Invalid filename.\n");
        return &undefined;
    }

    // node which to export
    INode* node = arg_list[1]->to_node();
    if (!node)
    {
        n_listener("Invalid node.\n");
        return &undefined;
    }

    int previewmode = arg_list[2]->to_int();

    Interface* intf = GetCOREInterface();

    if (!ExportScene(filename, intf, node, previewmode))
    {
        n_listener("Failed to export scene.\n");
        return &undefined;
    }

    return &ok;
}

//-----------------------------------------------------------------------------
def_visible_primitive(nExportNode, "nExportNode");

//-----------------------------------------------------------------------------
/**
    max script interface to export a scene. (export whole scene)
*/
Value* nExportScene_cf(Value** arg_list, int count)
{
    check_arg_count(nExportScene, 2, count);

    char* filename = arg_list[0]->to_string();
    int previewmode = arg_list[1]->to_int();

    Interface* intf = GetCOREInterface();

    if (!ExportScene(filename, intf, 0, previewmode))
    {
        n_listener("Failed to export scene.\n");
        return &undefined;
    }

    return &ok;
}

//-----------------------------------------------------------------------------
/**
    function registering macro for nExportScene_cf
*/
def_visible_primitive(nExportScene, "nExportScene");

//-----------------------------------------------------------------------------
/**
    max script interface to show directory setting dialog up with macro script.
*/
Value* nShowDirSettingDlg_cf(Value** arg_list, int count)
{
    // defined in nmaxdirdlg.cc
    ShowDirSettingDlg();

    return &ok;
}

//-----------------------------------------------------------------------------
/**
    function registering macro for nShowDirSettingDlg_cf.
*/
def_visible_primitive(nShowDirSettingDlg, "nShowDirSettingDlg");

//-----------------------------------------------------------------------------
/**
    Retrieves project directory name.
*/
Value* nGetProjDir_cf(Value** arg_list, int count)
{
    one_typed_value_local(Value* result);
    Value* result = &undefined;

    const char* projDir = GetProjDir();

    int len;
    if (len = strlen(projDir))
    {
        char* buf = new char[len + 1];
        sprintf(buf, "%s", projDir);
        result = new String(buf);
    }
    else
    {
        result = &undefined;
    }

    return_value(result);
}

//-----------------------------------------------------------------------------
/**
    function registering macro for nGetPorjDir_cf.
*/
def_visible_primitive(nGetProjDir, "nGetProjDir");


//-----------------------------------------------------------------------------
/**
    @verbatim
    nChangeShaderParameter ('Standard', 'common', 'MatDiffuse', '1.0, 0.0, 0.0, 1.0')
    @endvebatim
*/
Value* nChangeShaderParameter_cf(Value** arg_list, int count)
{
    check_arg_count(nChangeShaderParameter, 4, count);

	char* toolkitShaderName = arg_list[0]->to_string();
	char* shaderHandling    = arg_list[1]->to_string();
	char* paramID           = arg_list[2]->to_string();
	char* value             = arg_list[3]->to_string();

	if (!nChangeShaderParameter(toolkitShaderName, shaderHandling, paramID, value))
    {
        return &false_value;
    }

	return &true_value;
}

def_visible_primitive(nChangeShaderParameter, "nChangeShaderParameter");

//-----------------------------------------------------------------------------
/**
    @verbatim
    nconnectipc "localhost" "nviewer"
    @endvebatim
*/
Value* nConnectIpc_cf(Value** arg_list, int count)
{
    check_arg_count(nConnectIpc, 2, count);

	char* host = arg_list[0]->to_string();
	char* port = arg_list[1]->to_string();

    if (!nConnectIpc(host, port))
    {
        return &false_value;
    }

    return &true_value;
}

def_visible_primitive(nConnectIpc, "nConnectIpc");

//-----------------------------------------------------------------------------
/**
    @verbatim
    ndisconnectipc()
    @endvebatim
*/
Value* nDisconnectIpc_cf(Value** arg_list, int count)
{
    one_typed_value_local(Value* result);
    //Value* result = &undefined;
    
    nDisConnectIpc();

    return &ok;
}

def_visible_primitive(nDisconnectIpc, "nDisconnectIpc");

//-----------------------------------------------------------------------------
/**
    @verbatim
nsendmsgipc "/sys/servers/mayatoolkit.changemayashaderparameter \"Standard\" \"common\" \"MatDiffuse\" \".2, .2, .5, .6\""
    @endvebatim

*/
Value* nSendMsgIpc_cf(Value** arg_list, int count)
{
    check_arg_count(nSendIpc, 1, count);

	char* msg = arg_list[0]->to_string();

    if (!nSendMsgIpc(msg))
    {
        return &false_value;
    }

    return &true_value;
}

def_visible_primitive(nSendMsgIpc, "nSendMsgIpc");

//-----------------------------------------------------------------------------
/**
*/
Value* nIsConnectedIpc_cf(Value** arg_list, int count)
{
    one_typed_value_local(Value* result);

    if (!nIsConnectedIpc())
        return &false_value;

    return &true_value;
}

def_visible_primitive(nIsConnectedIpc, "nIsConnectedIpc");

