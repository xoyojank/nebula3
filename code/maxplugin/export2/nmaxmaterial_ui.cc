//-----------------------------------------------------------------------------
//  nmaxmaterial_ui.cc
//
//  Generate UI part of script in rollout of 3dsmax scripted plug-in
//  to handle Nebula2 custom material.
//
//  (c)2005 Kim, Hyoun Woo
//-----------------------------------------------------------------------------

#include "export2/nmax.h"
#include "util/nstring.h"
#include "export2/nmaxmaterial_ui.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

#if USE_ACTIVEX_ENVELOPECURVE_CTRL
// prefix of the envelopecurve activex control 
const char* particlePrefix = "nx";
#endif

//-----------------------------------------------------------------------------
/**
    Retrieves default value which descriped in 'def'.

    -14-Mar-06  kims  changed to get proper range for value of color picker.
*/
nString 
GetDefault(TiXmlElement* elemParam)
{
    nString paramType = elemParam->Attribute("type");
    nString defVal = elemParam->Attribute("def");

    nString defScript;

    if (paramType == "Int" || paramType == "Float")
    {
        defScript += defVal;
    }
    else
    if (paramType == "Bool")
    {
        int val = defVal.AsInt();

        if (val < 1)
            defScript += "false";
        else
            defScript += "true";
    }
    else
    if (paramType == "Color")
    {
        nArray<nString> tokens;
        int numValues = defVal.Tokenize(" ", tokens);

        defScript += "[";
        for (int i=0; i<numValues; i++)
        {
            // A Color value of 3dsmax are normally in the range 0.0 to 255.0 and 
            // Nebula uses 0.0 to 1.0 for its range.
            // so it needs to be converted to the proper range.
            nString tmp = tokens[i];
            float fval = tmp.AsFloat();
            tmp.Clear();
            tmp.SetFloat(fval * 255.0f);

            defScript += tmp;
            
            if (i < numValues - 1)
                defScript += ", ";
        }
        defScript += "]";
    }
    else
    if (paramType == "Texture" || 
        paramType == "BumpTexture" || 
        paramType == "CubeTexture" )
    {
    }
    else
    if (paramType == "Enum")
    {
        defScript += defVal;
    }
    else
    if (paramType == "Vector" || paramType == "ColorEnvelopeCurve")
    {
        nArray<nString> tokens;
        int numValues = defVal.Tokenize(" ", tokens);

        defScript += "#(";
        for (int i=0; i<numValues; i++)
        {
            defScript += tokens[i];
            if (i < numValues - 1)
                defScript += ", ";
        }
        defScript += ")";
    }
    else
    if( paramType == "EnvelopeCurve")
    {
        nArray<nString> tokens;
        int numValues = defVal.Tokenize(" ", tokens);
        nString minVal = elemParam->Attribute("min");
        nString maxVal = elemParam->Attribute("max");

        // revision modulation
        tokens[8].Format("%.1f", tokens[8].AsFloat() + 1.0f );

        defScript += "#(";
        for (int i=0; i<numValues; i++)
        {
            defScript += tokens[i];
            defScript += ", ";
        }
        
        defScript += minVal + ", ";
        defScript += maxVal;
        defScript += ")";
        
    }
    else
    {
        ;// unknown. return an empty string.
    }

    return defScript;
}

//-----------------------------------------------------------------------------
/**
    Convert string of enum values to items of dropdownlist UI.

    @param enums   string of enum values. e.g. enum = "None=1:CW:CCW"
    @param values  to contain each enum values string.
*/
static
void 
EnumToItem(const nString &enums, nArray<nString> &values)
{
    nArray<nString> tokens;
    int count = enums.Tokenize(":", tokens);

    nString str;
    for (int i=0; i<tokens.Size(); i++)
    {
        str = tokens[i].GetFirstToken("=");
        values.Append(str);
    }
}

//-----------------------------------------------------------------------------
/**
    Add script code to be sent to the viewer through IPC.
*/
static
nString
AddToolkitServerScriptForSpinner(const nString &shdName, const nString &shaderHandler, const nString &paramName)
{
    nString s, tmp;

    tmp.Format("\t\t\tif nIsConnectedIpc() do \n"); s += tmp;
    tmp.Format("\t\t\t(\n");                        s += tmp;
    tmp.Format("\t\t\t\tparam = %s.value as string\n", paramName.Get()); s += tmp;
    tmp.Format("\t\t\t\tnChangeShaderParameter \"%s\" \"%s\" \"%s\" param\n", shdName.Get(), shaderHandler.Get(), paramName.Get()); s += tmp;
    tmp.Format("\t\t\t)\n"); s += tmp;

    return s;
}

//-----------------------------------------------------------------------------
/**
    Add script code to be sent to the viewer through IPC.
*/
static
nString
AddToolkitServerScriptForCheckBox(const nString &shdName, const nString &shaderHandler, const nString &paramName)
{
    nString s, tmp;

    tmp.Format("\t\t\tif nIsConnectedIpc() do \n"); s += tmp;
    tmp.Format("\t\t\t(\n");                        s += tmp;
    tmp.Format("\t\t\t\tparam = %s.checked as string\n", paramName.Get()); s += tmp;
    tmp.Format("\t\t\t\tnChangeShaderParameter \"%s\" \"%s\" \"%s\" param\n", shdName.Get(), shaderHandler.Get(), paramName.Get()); s += tmp;
    tmp.Format("\t\t\t)\n"); s += tmp;

    return s;
}

//-----------------------------------------------------------------------------
/**
    Add script code to be sent to the viewer through IPC.
*/
static
nString
AddToolkitServerScriptForVector4Spinner(const nString &shdName, const nString &shaderHandler, const nString &paramName)
{
    nString s, tmp;

    tmp.Format("\t\t\tif nIsConnectedIpc() do \n"); s += tmp;
    tmp.Format("\t\t\t(\n");                        s += tmp;
    tmp.Format("\t\t\t\tparam = %s0.value as string\n", paramName.Get()); s += tmp;
    tmp.Format("\t\t\t\tparam += \" \"\n"); s += tmp;
    tmp.Format("\t\t\t\tparam += %s01.value as string\n", paramName.Get()); s += tmp;
    tmp.Format("\t\t\t\tparam += \" \"\n"); s += tmp;
    tmp.Format("\t\t\t\tparam += %s012.value as string\n", paramName.Get()); s += tmp;
    tmp.Format("\t\t\t\tparam += \" \"\n"); s += tmp;
    tmp.Format("\t\t\t\tparam += %s0123.value as string\n", paramName.Get()); s += tmp;
    tmp.Format("\t\t\t\tnChangeShaderParameter \"%s\" \"%s\" \"%s\" param\n", shdName.Get(), shaderHandler.Get(), paramName.Get()); s += tmp;
    tmp.Format("\t\t\t)\n"); s += tmp;

    return s;
}

//-----------------------------------------------------------------------------
/**
    Add script code to be sent to the viewer through IPC.
*/
static
nString
AddToolkitServerScriptForDropdownList
(const nString &shdName, const nString &shaderHandler, const nString &paramName)
{
    nString s, tmp;

    tmp.Format("\t\t\tif nIsConnectedIpc() do \n"); s += tmp;
    tmp.Format("\t\t\t(\n");                        s += tmp;
    tmp.Format("\t\t\t\tparam = %s.selection as string\n", paramName.Get()); s += tmp;
    tmp.Format("\t\t\t\tnChangeShaderParameter \"%s\" \"%s\" \"%s\" param\n", shdName.Get(), shaderHandler.Get(), paramName.Get()); s += tmp;
    tmp.Format("\t\t\t)\n"); s += tmp;

    return s;
}


//-----------------------------------------------------------------------------
/**
    Add spinner UI script to rollout clause.

    Example:
    @verbatim
    spinner RenderPri "Render Priority" align:#left fieldwidth:36 range:[-100,100,0]
    @endverbatim
*/
nString 
AddSpinner(const nString &shdName, const nString &shaderHandler, bool isFloat, TiXmlElement* elemParam)
{
    nString paramName  = elemParam->Attribute("name");  // UI control name.
    nString caption    = elemParam->Attribute("label"); // UI caption 
    nString defaultVal = elemParam->Attribute("def");   // default value of the ui.

    nString uiScript;
    nString tmp;

    tmp.Format("\t\tspinner %s \"%s\" align:#left fieldwidth:36 ", paramName.Get(), caption.Get());
    uiScript += tmp;

    // if the element has spinner UI, there's min/max attributes in xml element.
    nString min = elemParam->Attribute("min");
    nString max = elemParam->Attribute("max");

    tmp.Format("range:[%s, %s, %s] ",
        min.Get(),
        max.Substitute("+", "").Get(),    // max value has '+' sign, so we substitute it to empty char.
        GetDefault(elemParam).Get()); uiScript += tmp;
    
    tmp.Format("type:%s", isFloat ? "#float" : "#integer"); uiScript += tmp;

    uiScript += "\n";

    tmp.Format("\t\ton %s changed val do\n", paramName.Get()); uiScript += tmp;
    tmp.Format("\t\t(\n"); uiScript += tmp;
    uiScript += AddToolkitServerScriptForSpinner(shdName, shaderHandler, paramName);
    tmp.Format("\t\t)\n"); uiScript += tmp;

    return uiScript;
}

//-----------------------------------------------------------------------------
/**
    Add checkbox UI script to rollout clause.

    Example:
    @verbatim
    checkbox LockViewer "Lock To Viewer" align:#left checked:false
    @endverbatim
*/
nString 
AddCheckBox(const nString &shdName, const nString &shaderHandler, TiXmlElement* elemParam)
{
    nString paramName = elemParam->Attribute("name");
    nString caption = elemParam->Attribute("label");
    nString defaultVal = elemParam->Attribute("def");

    nString uiScript;
    nString tmp;

    tmp.Format("\t\tcheckbox %s \"%s\" align:#left ", paramName.Get(), caption.Get());
    uiScript += tmp;

    uiScript += "checked:";
    uiScript += GetDefault(elemParam);

    uiScript += "\n";

    tmp.Format("\t\ton %s changed val do\n", paramName.Get());    uiScript += tmp;
    tmp.Format("\t\t(\n");    uiScript += tmp;
    uiScript += AddToolkitServerScriptForCheckBox(shdName, shaderHandler, paramName);
    tmp.Format("\t\t)\n");    uiScript += tmp;

    return uiScript;
}

//-----------------------------------------------------------------------------
/**
    Add dropdownlist UI script to rollout clause.

    Example:
    @verbatim
    dropdownlist CullMode "Cull Mode" align:#left width:100 items:#("None","CW","CCW") selection:2
    @endverbatim
*/
nString 
AddDropdownList(const nString &shdName, const nString &shaderHandler, TiXmlElement* elemParam)
{
    nString paramName = elemParam->Attribute("name");
    nString caption = elemParam->Attribute("label");

    nString uiScript;
    nString tmp;

    tmp.Format("\t\tdropdownlist %s \"%s\" align:#left width:100 ", paramName.Get(), caption.Get());
    uiScript += tmp;

    // we assume that if we have dropdownlist, the type of the element is 'Enum'.
    // if the ui is 'dropdownlist', it needs items.

    nString enums = elemParam->Attribute("enum");

    nArray<nString> values;

    EnumToItem(enums, values);

    uiScript += "items:#(";

    for (int i=0;i<values.Size(); i++)
    {
        uiScript += "\"";
        uiScript += values[i];
        uiScript += "\"";

        if (i < values.Size() - 1)
            uiScript += ",";
    }

    uiScript += ")";
    uiScript += " ";
    
    // add default value. 
    uiScript += "selection:";
    uiScript += GetDefault(elemParam);
    uiScript += " ";

    uiScript += "\n";

    tmp.Format("\t\ton %s changed val do\n", paramName.Get());    uiScript += tmp;
    tmp.Format("\t\t(\n");    uiScript += tmp;
    uiScript += AddToolkitServerScriptForDropdownList(shdName, shaderHandler, paramName);
    tmp.Format("\t\t)\n");    uiScript += tmp;

    return uiScript;
}

//-----------------------------------------------------------------------------
/**
    Add colorpicker UI script to rollout clause.

    Example:
    @verbatim
    colorpicker MatDiffuse "Diffuse Color" align:#left alpha:true color:[1.0, 1.0, 1.0, 1.0]
    @endverbatim
*/
nString 
AddColorPicker(TiXmlElement* elemParam)
{
    nString paramName = elemParam->Attribute("name");
    nString caption = elemParam->Attribute("label");

    nString uiScript;
    nString tmp;

    tmp.Format("\t\tcolorpicker %s \"%s\" align:#left ", paramName.Get(), caption.Get());
    uiScript += tmp;

#if MAX_RELEASE >= 6000
    uiScript += "alpha:true";
    uiScript += " ";
#endif

    // default value.
    uiScript += "color:";
    uiScript += GetDefault(elemParam);
    uiScript += " ";

    // 3dsmax5 orlower versions does not support specifyng alpha in ColorPicker.
    // So additional spinner UI for handling alpha value.
#if MAX_RELEASE < 6000
    uiScript += "across:2 ";

    uiScript += "\n";
    uiScript += "spinner ";
    nString uiName; 
    uiName.Format("%sAlpha",paramName.Get());
    uiScript += uiName;
    uiScript += " ";
    uiScript += "Alpha"; //caption
    uiScript += " ";
    uiScript += "type:#float"; //always float type for alpha
    uiScript += " ";
    uiScript += "align:#left";
    uiScript += " ";
    uiScript += "fieldwidth:36";
#endif

    uiScript += "\n";

    return uiScript;
}

//-----------------------------------------------------------------------------
/**
    Add mapbutton UI script to rollout clause.

    Example:
    @verbatim
    mapbutton DiffMap0 "Texture Map" align:#left width:150
    @endverbatim
*/
nString 
AddMapButton(TiXmlElement* elemParam, nString defaultVal)
{
    nString paramName = elemParam->Attribute("name");
    nString caption = elemParam->Attribute("label");

    nString uiScript;
    nString tmp;

    nString dummyLabel = paramName;
    dummyLabel += "_";
    uiScript += AddLabel(dummyLabel, caption, 2, true);

    tmp.Format("\t\tmapbutton %s \"%s\" ", paramName.Get(), caption.Get());
    uiScript += tmp;

    // add default map property if the default value is given.
    if (!defaultVal.IsEmpty())
    {
        uiScript += "map:";
        uiScript += "\"" + defaultVal + "\"";
        uiScript += " ";
    }
    uiScript += "align:#left";
    uiScript += " ";
    uiScript += "width:150";
    uiScript += "\n";

    return uiScript;
}

//-----------------------------------------------------------------------------
/**
    Add four spinner UI script to rollout clause for 'Vector' type.

    -02-Nov-06  kims Changed to correctly handle 'Vectory' type. Thank ZHANG Zikai for the patch.

    Example:
    @verbatim
    label TexGenS "Texture Scale " align:#left across:5
    spinner TexGenS0 align:#left fieldwidth:36
    spinner TexGenS01 align:#left fieldwidth:36
    spinner TexGenS012 align:#left fieldwidth:36
    spinner TexGenS0123 align:#left fieldwidth:36
    @endverbatim
*/
nString 
AddVector4Spinner(const nString &shdName, const nString &shaderHandler, TiXmlElement* elemParam)
{
    nString paramName  = elemParam->Attribute("name");  // UI control name.
    nString caption    = elemParam->Attribute("label"); // UI caption 
    nString def        = elemParam->Attribute("def"); // UI caption 

    nString uiScript, tmp;

    // insert dummy label which has parameters name for its UI name.
    caption += " ";
    uiScript += AddLabel(paramName, caption, 5);

    nString name, label;
    nArray<nString> defs;
    int count = def.Tokenize(" \t\n", defs);
    if (count != 4)
    {
        n_maxlog(Error, "Failed to load default values of Vector parameter '%s', using [0, 0, 0, 0]", paramName);
        defs.Clear();
        defs.Append("0");
        defs.Append("0");
        defs.Append("0");
        defs.Append("0");
    }

    name = paramName;
    // create four spinner for vector type.
    for (int i=0; i<4; i++)
    {
        name.AppendInt(i);
        tmp.Format("\t\tspinner %s align:#left fieldwidth:36 range:[-100000, 100000, %s]\n", name.Get(), defs[i].Get()); uiScript += tmp;
    }

    tmp.Format("\t\tfn update_%s =\n", paramName.Get()); uiScript += tmp;
    tmp.Format("\t\t(\n"); uiScript += tmp;
    uiScript += AddToolkitServerScriptForVector4Spinner(shdName, shaderHandler, paramName);
    tmp.Format("\t\t)\n"); uiScript += tmp;

    name = paramName;
    for (int i=0; i<4; i++)
    {
        name.AppendInt(i);
        tmp.Format("\t\ton %s changed val do\n", name.Get()); uiScript += tmp;
        tmp.Format("\t\t(\n");    uiScript += tmp;
        tmp.Format("\t\t\tupdate_%s()\n", paramName.Get()); uiScript += tmp;
        tmp.Format("\t\t)\n");    uiScript += tmp;
    }

    return uiScript;
}

//-----------------------------------------------------------------------------
/**
    Add label UI script to rollout clause.

    Example:
    @verbatim
    label dummy "" align:#left across:1
    @endverbatim
*/
nString 
AddLabel(const nString &uiname, const nString &caption, int across, bool addDot)
{
    nString uiScript;
    nString tmp;

    tmp.Format("\t\tlabel %s \"%s", uiname.Get(), caption.Get());
    uiScript += tmp;

    if (addDot)
    {
        uiScript += "..................................................";
    }

    tmp.Format("\" align:#left across:");
    uiScript += tmp;

    nString strAcross;
    strAcross.AppendInt(across);
    uiScript += strAcross;

    uiScript += "\n";

    return uiScript;
}

//-----------------------------------------------------------------------------
/**
    Add texture directory setting button.

    Example:
    @verbatim
    button btnFldBumpMap0 "texture:"align:#right width:150
    on btnFBumpMap0 pressed do
    (
        mapPath = getSavePath caption:"Select path of Wave Bump Map"
        if mapPath != undefined then 
        (
            dirSettingBumpMap0  = mapPath
            edtFldBumpMap0.text = mapPath
        )
    )
    @endverbatim
*/
nString
AddSetDirDlg(TiXmlElement* elemParam)
{
    nString uiname  = elemParam->Attribute("name");
    nString caption = elemParam->Attribute("label");

    nString uiScript;
    nString tmp;

    nString uiPrefix = "Fld";

    // script code for 'edittext' control
    tmp.Format("\t\tedittext edt%s%s \"Dest Folder : \"  align:#left fieldWidth:180 across:2 \n", uiPrefix.Get(), uiname.Get());
    uiScript += tmp;

    // script code for 'button' control
    tmp.Format("\t\tbutton btn%s%s \"<<\" align:#right width:40 height:18\n", uiPrefix.Get(), uiname.Get());
    uiScript += tmp;

    // button handler scrpt code
    tmp.Format("\t\ton btn%s%s pressed do\n\t\t(\n", uiPrefix.Get(), uiname.Get());
    uiScript += tmp;
    tmp.Format("\t\t\tinclude \"nebula2/utilz.ms\"\n");
    uiScript += tmp;
    tmp.Format("\t\t\tprojDir = nGetProjDir();\n");
    uiScript += tmp;
    tmp.Format("\t\t\ttexAssign = nebula2.call \"/sys/servers/file2.manglepath\" \"textures:\"\n");
    uiScript += tmp;
    tmp.Format("\t\t\ttexAssign = nReplaceSlash texAssign[1]\n");
    uiScript += tmp;
    tmp.Format("\t\t\tmapPath = getSavePath initialDir:texAssign caption:\"Select a directory where the %s to be exported.\"\n", caption.Get());
    uiScript += tmp;
    tmp.Format("\t\t\tif mapPath != undefined then\n");
    uiScript += tmp;
    tmp.Format("\t\t\t(\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\tmapPath = nReplaceSlash mapPath\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\tfindIndex = findString mapPath texAssign\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\tif findIndex != undefined  then \n");
    uiScript += tmp;
    tmp.Format("\t\t\t\t(\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\t\tsubDir = substring mapPath (texAssign.count + 2) mapPath.count\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\t\texportDir = \"textuers:\" + subDir\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\t\tedt%s%s.text = exportDir\n", uiPrefix.Get(), uiname.Get());
    uiScript += tmp;
    tmp.Format("\t\t\t\t\tdirSetting%s = exportDir\n", uiname.Get());
    uiScript += tmp;
    tmp.Format("\t\t\t\t)\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\telse\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\t(\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\t\tformat \"(%%) is not subdirectory of (%%)\\n\" mapPath texAssign\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\t)\n");
    uiScript += tmp;
    tmp.Format("\t\t\t)\n");
    uiScript += tmp;
    tmp.Format("\t\t)\n");
    uiScript += tmp;

    // edit handler scrpt code
    tmp.Format("\t\ton edt%s%s entered newstr do\n", uiPrefix.Get(), uiname.Get());
    uiScript += tmp;
    tmp.Format("\t\t(\n");
    uiScript += tmp;
    tmp.Format("\t\t\tinclude \"nebula2/utilz.ms\"\n");
    uiScript += tmp;
    tmp.Format("\t\t\n");
    uiScript += tmp;
    tmp.Format("\t\t\tprojDir = nGetProjDir();\n");
    uiScript += tmp;
    tmp.Format("\t\t\t-- NOTE: nebula2.call returns array type\n");
    uiScript += tmp;
    tmp.Format("\t\t\ttexAssign = nebula2.call \"/sys/servers/file2.manglepath\" \"textures:\"\n");
    uiScript += tmp;
    tmp.Format("\t\t\ttexAssign = nReplaceSlash texAssign[1]\n");
    uiScript += tmp;
    tmp.Format("\t\t\n");
    uiScript += tmp;
    tmp.Format("\t\t\tmapPath = nebula2.call \"/sys/servers/file2.manglepath\" newstr\n");
    uiScript += tmp;
    tmp.Format("\t\t\tmapPath = mapPath[1]\n");
    uiScript += tmp;
    tmp.Format("\t\t\tfindIndex = findString mapPath texAssign\n");
    uiScript += tmp;
    tmp.Format("\t\t\tif findIndex != undefined  then \n");
    uiScript += tmp;
    tmp.Format("\t\t\t(\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\tsubDir = substring mapPath (texAssign.count + 2) mapPath.count\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\texportDir = \"textures:\" + subDir\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\tedt%s%s.text = exportDir\n", uiPrefix.Get(), uiname.Get());
    uiScript += tmp;
    tmp.Format("\t\t\t\tdirSetting%s = exportDir\n", uiname.Get());
    uiScript += tmp;
    tmp.Format("\t\t\t)\n");
    uiScript += tmp;
    tmp.Format("\t\t\telse\n");
    uiScript += tmp;
    tmp.Format("\t\t\t(\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\tformat \"(%%) is not subdirectory of (%%)\\n\" mapPath texAssign\n");
    uiScript += tmp;
    tmp.Format("\t\t\t\tedt%s%s.text = \"\"\n", uiPrefix.Get(), uiname.Get());
    uiScript += tmp;
    tmp.Format("\t\t\t\tdirSetting%s = \"\"\n", uiname.Get());
    uiScript += tmp;
    tmp.Format("\t\t\t)\n");
    uiScript += tmp;
    tmp.Format("\t\t)\n");
    uiScript += tmp;

    return uiScript;
}

//-----------------------------------------------------------------------------
/**
*/
static
nString AddVectorEnvelopeCurveEventHandler(nString &paramName)
{
    nString s;
#if USE_ACTIVEX_ENVELOPECURVE_CTRL
    nString tmp;
    int i, index;
    for (i = 0, index = 1; i < 4; i++, index += 3)
    {
        tmp.Format("\t\t\t%s[%d] = (bit.and 0xff (%s%s.GetColor %d) ) / 255.0\n", paramName.Get(), index + 0, particlePrefix, paramName.Get(), i);
        s += tmp;
        tmp.Format("\t\t\t%s[%d] = (bit.and 0xff (bit.shift (%s%s.GetColor %d) -8)) / 255.0\n", paramName.Get(), index + 1, particlePrefix, paramName.Get(), i);
        s += tmp;
        tmp.Format("\t\t\t%s[%d] = (bit.and 0xff (bit.shift (%s%s.GetColor %d) -16)) / 255.0\n", paramName.Get(), index + 2, particlePrefix, paramName.Get(), i);
        s += tmp;
    }

    tmp.Format("\t\t\t%s[%d] = %s%s.GetPos 1 \n", paramName.Get(), index + 0, particlePrefix, paramName.Get());  s += tmp;
    tmp.Format("\t\t\t%s[%d] = %s%s.GetPos 2 \n", paramName.Get(), index + 1, particlePrefix, paramName.Get());  s += tmp;
    
#else
    nString tmp;
    int i, index;
    for (i = 0, index = 1; i < 4; i++, index += 3)
    {
        tmp.Format("\t\t\t%s[%d] = %s_v%d.color.r / 255.0\n", paramName.Get(), index + 0, paramName.Get(), i);  s += tmp;
        tmp.Format("\t\t\t%s[%d] = %s_v%d.color.g / 255.0\n", paramName.Get(), index + 1, paramName.Get(), i);  s += tmp;
        tmp.Format("\t\t\t%s[%d] = %s_v%d.color.b / 255.0\n", paramName.Get(), index + 2, paramName.Get(), i);  s += tmp;
    }
    tmp.Format("\t\t\t%s[%d] = %s_p1.value\n", paramName.Get(), index + 0, paramName.Get());  s += tmp;
    tmp.Format("\t\t\t%s[%d] = %s_p2.value\n", paramName.Get(), index + 1, paramName.Get());  s += tmp;

#endif
    return s;
}

//-----------------------------------------------------------------------------
/**
*/
static
nString AddEnvelopeCurveEventHandler(nString &paramName)
{
    nString s;

    // add min, max param
#if USE_ACTIVEX_ENVELOPECURVE_CTRL
    s += "\t\t\t"; s += paramName; s += "[1]"; s += " = "; s += particlePrefix; s += paramName; s += ".GetValue 0\n";
    s += "\t\t\t"; s += paramName; s += "[2]"; s += " = "; s += particlePrefix; s += paramName; s += ".GetValue 1\n";
    s += "\t\t\t"; s += paramName; s += "[3]"; s += " = "; s += particlePrefix; s += paramName; s += ".GetValue 2\n";
    s += "\t\t\t"; s += paramName; s += "[4]"; s += " = "; s += particlePrefix; s += paramName; s += ".GetValue 3\n";

    s += "\t\t\t"; s += paramName; s += "[5]"; s += " = "; s += particlePrefix; s += paramName; s += ".GetPos 1 \n";
    s += "\t\t\t"; s += paramName; s += "[6]"; s += " = "; s += particlePrefix; s += paramName; s += ".GetPos 2 \n";

    /*
    s += "\t\t\t"; s += paramName; s += "[7]"; s += " = "; s += particlePrefix; s += paramName; s += ".Frequency\n";
    s += "\t\t\t"; s += paramName; s += "[8]"; s += " = "; s += particlePrefix; s += paramName; s += ".Amplitude\n";
    s += "\t\t\t"; s += paramName; s += "[9]"; s += " = "; s += particlePrefix; s += paramName; s += ".ModulationFunc\n";
    s += "\t\t\t"; s += paramName; s += "[10]"; s += " = "; s += particlePrefix; s += paramName; s += ".Min\n";
    s += "\t\t\t"; s += paramName; s += "[11]"; s += " = "; s += particlePrefix; s += paramName; s += ".Max\n";
    */

    nString tmp;
    tmp.Format("\t\t\t%s[7] = %s_freq.value\n", paramName.Get(), paramName.Get());  s += tmp;
    tmp.Format("\t\t\t%s[8] = %s_ampl.value\n", paramName.Get(), paramName.Get());  s += tmp;
    tmp.Format("\t\t\t%s[9] = %s_modulation.selection\n", paramName.Get(), paramName.Get());  s += tmp;
    tmp.Format("\t\t\t%s[10] = %s_min.value\n", paramName.Get(), paramName.Get());  s += tmp;
    tmp.Format("\t\t\t%s[11] = %s_max.value\n", paramName.Get(), paramName.Get());  s += tmp;
#else
    nString tmp;
    tmp.Format("\t\t\t%s[1] = %s_v0.value\n", paramName.Get(), paramName.Get());  s += tmp;
    tmp.Format("\t\t\t%s[2] = %s_v1.value\n", paramName.Get(), paramName.Get());  s += tmp;
    tmp.Format("\t\t\t%s[3] = %s_v2.value\n", paramName.Get(), paramName.Get());  s += tmp;
    tmp.Format("\t\t\t%s[4] = %s_v3.value\n", paramName.Get(), paramName.Get());  s += tmp;

    tmp.Format("\t\t\t%s[5] = %s_p1.value\n", paramName.Get(), paramName.Get());  s += tmp;
    tmp.Format("\t\t\t%s[6] = %s_p2.value\n", paramName.Get(), paramName.Get());  s += tmp;

    tmp.Format("\t\t\t%s[7] = %s_freq.value\n", paramName.Get(), paramName.Get());  s += tmp;
    tmp.Format("\t\t\t%s[8] = %s_ampl.value\n", paramName.Get(), paramName.Get());  s += tmp;
    tmp.Format("\t\t\t%s[9] = %s_modulation.selection\n", paramName.Get(), paramName.Get());  s += tmp;
#endif
    return s;
}

//-----------------------------------------------------------------------------
/**
    Add script code to be sent to the viewer through IPC.
*/
static
nString AddToolkitServerScriptForEnvelopeCurve(const nString &shdName, const nString &shaderHandler, nString &paramName)
{
    nString s;

    s += "\t\t\t"; s += "if nIsConnectedIpc() do \n";
    s += "\t\t\t"; s += "(\n";

    s += "\t\t\t\t"; s += "param = \"\" \n";

    // add params(min,max)
    nString tmp;
    for (int i = 1; i <= 11; i++)
    {
        tmp.Format("\t\t\t\tparam += %s[%d] as string\n", paramName.Get(), i); s += tmp;
        tmp.Format("\t\t\t\tparam += \" \"\n"); s += tmp;
    }

    s += "\n";

    s += "\t\t\t\t"; s += "nChangeShaderParameter "; 
    s += "\""; s += shdName; s += "\" ";
    s += "\""; s += shaderHandler; s+= "\" ";
    s += "\""; s += paramName; s += "\" ";
    s += "param \n";

    s += "\t\t\t"; s += ")\n";

    return s;
}

//-----------------------------------------------------------------------------
/**
    Add script code to be sent to the viewer through IPC.
*/
static
nString AddToolkitServerScriptForColorEnvelopeCurve(const nString &shdName, const nString &shaderHandler, nString &paramName)
{
    nString s;
    nString tmp;
    int i;

    tmp.Format("\t\t\tif nIsConnectedIpc() do \n"); s += tmp;
    tmp.Format("\t\t\t(\n");                        s += tmp;
    tmp.Format("\t\t\t\tparam = \"\" \n");          s += tmp;

    for (i = 1; i <= 14; i++)
    {
        tmp.Format("\t\t\t\tparam += %s[%d] as string\n", paramName.Get(), i); s += tmp;
        tmp.Format("\t\t\t\tparam += \" \"\n"); s += tmp;
    }

    tmp.Format("\n"); s += tmp;

    tmp.Format("\t\t\t\tnChangeShaderParameter \"%s\" \"%s\" \"%s\" param\n", shdName.Get(), shaderHandler.Get(), paramName.Get());  s += tmp;
    tmp.Format("\t\t\t)\n"); s += tmp;

    return s;
}

//-----------------------------------------------------------------------------
/**
    Add nmaxenvelopecurve custom control to the rollout.

*/
nString AddEnvelopeCurve(const nString &shdName, const nString &shaderHandler, TiXmlElement* elemParam)
{
    nString paramName  = elemParam->Attribute("name");  // UI control name.
    nString caption    = elemParam->Attribute("label"); // UI caption 
    nString defaultVal = elemParam->Attribute("def");   // default value of the ui.

    nString uiScript;
    nString tmp;

    // begin of group control
    tmp.Format("\t\tgroup \"%s\"\n\t\t(\n", caption.Get());
    uiScript += tmp;

#if USE_ACTIVEX_ENVELOPECURVE_CTRL

    // use color picker due to that we don't have envelope color curve control yet.
    if (paramName == "ParticleRGB")
    {
        tmp.Format("\t\tactiveXControl %s%s \"{EDA6DBCD-A8BB-4709-AB92-40181C0C58CE}\" height:60 \n", particlePrefix, paramName.Get());
        uiScript += tmp;
    }
    else
    {
        nArray<nString> defValues;
        int count = defaultVal.Tokenize(" \t\n", defValues);
        // default min, max value
        nString minVal = elemParam->Attribute("min"); 
        nString maxVal = elemParam->Attribute("max");

        n_assert(count == 9 || count == 8);
        // FIXME: check the default values' type
        nArray<float> defFloatValues;
        for (int i = 0; i < 8; i++)
        {
            defFloatValues.Append(atof(defValues[i].Get()));
        }
        int modulation = 1;
        if (count == 9)
        {
            modulation = atoi(defValues[8].Get()) + 1;
        }

        // nmaxenvelopecurve control.
        tmp.Format("\t\t\tactiveXControl %s%s \"{EDA6DBCD-A8BB-4709-AB92-40181C0C58CE}\" height:130 width:180\n", particlePrefix, paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\t\tspinner %s_max \"Max\" type:#float range:[-10000.0, 10000.0, %f] scale:0.1 align:#right offset:[0, -135] width:50 height:16 fieldwidth:50\n", paramName.Get(), maxVal.AsFloat());
        uiScript += tmp;
        tmp.Format("\t\t\tspinner %s_min \"Min\" type:#float range:[-10000.0, 10000.0, %f] scale:0.1 align:#right offset:[0, -2] width:50 height:16 fieldwidth:50\n", paramName.Get(), minVal.AsFloat());
        uiScript += tmp;
        tmp.Format("\t\t\tspinner %s_freq \"Frequency\" type:#float range:[0, 10000.0, %f] scale:0.01 align:#right offset:[0, -2] width:50 height:16 fieldwidth:50\n", paramName.Get(), defFloatValues[6]);
        uiScript += tmp;
        tmp.Format("\t\t\tspinner %s_ampl \"Amplitude\" type:#float range:[0, 10000.0, %f] scale:0.01 align:#right offset:[0, -2] width:50 height:16 fieldwidth:50\n", paramName.Get(), defFloatValues[7]);
        uiScript += tmp;
        tmp.Format("\t\t\tlabel %s_modulation_label \"Function\" align:#right offset:[-55, 2] width:50 height:16\n", paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\t\tdropdownList %s_modulation items:#(\"sine\", \"cosine\") selection:%d align:#right offset:[0, -25] width:60 height:16\n", paramName.Get(), modulation);
        uiScript += tmp;
    }
#else
    nArray<nString> defValues;
    int count = defaultVal.Tokenize(" \t\n", defValues);
    if (paramName == "ParticleRGB")
    {
        float defP1 = .3f, defP2 = .7f;
        // n_assert(count == 16);
        tmp.Format("\t\t\tlabel %s_l1 \"Values\" align:#left offset:[0, 0] width:64 height:13 across:4\n", paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\t\tspinner %s_p1 \"p1\" type:#float range:[0, 1, %f] scale:0.0001 align:#left offset:[0, 0] width:64 height:16 fieldwidth:45\n", paramName.Get(), defP1);
        uiScript += tmp;
        tmp.Format("\t\t\tspinner  %s_p2 \"p2\" type:#float range:[0, 1, %f] scale:0.0001 align:#left offset:[0, 0] width:64 height:16 fieldwidth:45\n", paramName.Get(), defP2);
        uiScript += tmp;
        tmp.Format("\t\t\tlabel %s_dummy \"\"\n", paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\t\tcolorPicker %s_v0 \"v0\" align:#left offset:[0, 0] width:64 height:16 across:4\n", paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\t\tcolorPicker %s_v1 \"v1\" align:#left offset:[0, 0] width:64 height:16\n", paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\t\tcolorPicker %s_v2 \"v2\" align:#left offset:[0, 0] width:64 height:16\n", paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\t\tcolorPicker %s_v3 \"v3\" align:#left offset:[0, 0] width:64 height:16\n", paramName.Get());
        uiScript += tmp;
    }
    else
    {
        n_assert(count == 9 || count == 8);
        // FIXME: check the default values' type
        nArray<float> defFloatValues;
        for (int i = 0; i < 8; i++)
        {
            defFloatValues.Append(atof(defValues[i].Get()));
        }
        int modulation = 1;
        if (count == 9)
        {
            modulation = atoi(defValues[8].Get()) + 1;
        }

        tmp.Format("\t\t\tlabel %s_l1 \"Values\" align:#left offset:[0, 0] width:64 height:13 across:4\n", paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\t\tspinner %s_p1 \"p1\" type:#float range:[0, 1, %f] scale:0.0001 align:#left offset:[0, 0] width:64 height:16 fieldwidth:45\n", paramName.Get(), defFloatValues[4]);
        uiScript += tmp;
        tmp.Format("\t\t\tspinner  %s_p2 \"p2\" type:#float range:[0, 1, %f] scale:0.0001 align:#left offset:[0, 0] width:64 height:16 fieldwidth:45\n", paramName.Get(), defFloatValues[5]);
        uiScript += tmp;
        tmp.Format("\t\t\tlabel %s_dummy \"\"\n", paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\t\tspinner %s_v0 \"v0\" type:#float range:[0, 10000.0, %f] scale:0.0001 align:#left offset:[0, 0] width:64 height:16 fieldwidth:45 across:4\n", paramName.Get(), defFloatValues[0]);
        uiScript += tmp;
        tmp.Format("\t\t\tspinner %s_v1 \"v1\" type:#float range:[0, 10000.0, %f] scale:0.0001 align:#left offset:[0, 0] width:64 height:16 fieldwidth:45\n", paramName.Get(), defFloatValues[1]);
        uiScript += tmp;
        tmp.Format("\t\t\tspinner %s_v2 \"v2\" type:#float range:[0, 10000.0, %f] scale:0.0001 align:#left offset:[0, 0] width:64 height:16 fieldwidth:45\n", paramName.Get(), defFloatValues[2]);
        uiScript += tmp;
        tmp.Format("\t\t\tspinner %s_v3 \"v3\" type:#float range:[0, 10000.0, %f] scale:0.0001 align:#left offset:[0, 0] width:64 height:16 fieldwidth:45\n", paramName.Get(), defFloatValues[3]);
        uiScript += tmp;
        tmp.Format("\t\t\tspinner %s_freq \"Frequency\" type:#float range:[0, 10000.0, %f] scale:0.0001 align:#left offset:[0, 0] width:64 height:16 fieldwidth:60\n", paramName.Get(), defFloatValues[6]);
        uiScript += tmp;
        tmp.Format("\t\t\tspinner %s_ampl \"Amplitude\" type:#float range:[0, 10000.0, %f] scale:0.0001 align:#left offset:[0, 0] width:64 height:16 fieldwidth:60\n", paramName.Get(), defFloatValues[7]);
        uiScript += tmp;
        tmp.Format("\t\t\tdropdownList %s_modulation \"ModulationFunc\" align:#left offset:[144, -42] width:112 height:41 items:#(\"sine\", \"cosine\") selection:%d\n", paramName.Get(), modulation);
        uiScript += tmp;
    }
#endif

    // end of group control
    uiScript += "\t\t";
    uiScript += ")\n";

#if USE_ACTIVEX_ENVELOPECURVE_CTRL
    if (paramName == "ParticleRGB")
    {
        // add activex contorl event handler
        tmp.Format("\t\ton %s%s OnChangedValue do\n", particlePrefix, paramName.Get());
        uiScript += tmp;
    }
    else
    {
     
        tmp.Format("\t\tfn update_%s=\n", paramName.Get());
        uiScript += tmp;
    }

#else
    // add controls event handlers
    tmp.Format("\t\tfn update_%s=\n", paramName.Get());
    uiScript += tmp;
#endif
    uiScript += "\t\t";
    uiScript += "(\n";

    //add event handler which specifies control's values to parameters block.
    if (paramName == "ParticleRGB")
    {
        uiScript += AddVectorEnvelopeCurveEventHandler(paramName);
        uiScript += AddToolkitServerScriptForColorEnvelopeCurve(shdName, shaderHandler, paramName);
    }
    else
    {
        uiScript += AddEnvelopeCurveEventHandler(paramName);

        // NOTE: we could add this on parameters block clause,
        //       but it looks better to add this place for the activex control
        //       cause it has its own event handler.
        uiScript += AddToolkitServerScriptForEnvelopeCurve(shdName, shaderHandler, paramName);
    }

    uiScript += "\t\t";
    uiScript += ")\n";

#if USE_ACTIVEX_ENVELOPECURVE_CTRL

    // use color picker due to that we don't have envelope color curve control yet.
    if (paramName == "ParticleRGB")
    {
        // 
    }
    // curve envelop
    else
    {
        // add activex contorl event handler
        tmp.Format("\t\ton %s%s OnChangedValue do ( update_%s(); )\n", particlePrefix, paramName.Get(), paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\ton %s_freq changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\ton %s_ampl changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\ton %s_modulation selected i do ( update_%s();)\n", paramName.Get(), paramName.Get() );
        uiScript += tmp;
        tmp.Format("\t\ton %s_min changed val do ( if val > %s_max.value then %s_min.value = %s_max.value; update_%s(); )\n", 
            paramName.Get(), paramName.Get(), paramName.Get(), paramName.Get(), paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\ton %s_max changed val do ( if val < %s_min.value then %s_max.value = %s_min.value; update_%s(); )\n", 
            paramName.Get(), paramName.Get(), paramName.Get(), paramName.Get(), paramName.Get());
        uiScript += tmp;
    }

#else
    if (paramName == "ParticleRGB")
    {
        tmp.Format("\t\ton %s_v0 changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\ton %s_v1 changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\ton %s_v2 changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\ton %s_v3 changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;

        tmp.Format("\t\ton %s_p1 changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\ton %s_p2 changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;
    }
    else
    {
        tmp.Format("\t\ton %s_v0 changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\ton %s_v1 changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\ton %s_v2 changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\ton %s_v3 changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;

        tmp.Format("\t\ton %s_p1 changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\ton %s_p2 changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;

        tmp.Format("\t\ton %s_freq changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\ton %s_ampl changed val do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;
        tmp.Format("\t\ton %s_modulation selected i do update_%s()\n", paramName.Get(), paramName.Get());
        uiScript += tmp;
    }
#endif

    return uiScript;
}







