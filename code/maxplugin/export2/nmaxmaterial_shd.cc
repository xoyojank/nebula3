//-----------------------------------------------------------------------------
//  nmaxmaterial_shd.cc
//
//  Generate 3dsmax scripted plug-in to handle Nebula2 custom material.
//
//  (c)2005 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "export2/nmax.h"
#include "base/nmaxlistener.h"
#include "export2/nmaxscriptcall.h"
#include "export2/nmaxutil.h"
#include "util/nstring.h"
#include "export2/nmaxmaterial_ui.h"
#include "export2/nmaxoptions.h"

#include "kernel/nkernelserver.h"
#include "kernel/nfileserver2.h"
#include "kernel/nfile.h"
#include "misc/niniprefserver.h"
#include "util/nstring.h"
#include "tinyxml/tinyxml.h"
#include "gfx2/nshaderstate.h"

#if USE_ACTIVEX_ENVELOPECURVE_CTRL
// prefix of the envelopecurve activex control 
extern const char* particlePrefix;
#endif

/// contains shader name which to be filtered out.
static nArray<nString> shaderFilterArray;

//-----------------------------------------------------------------------------
/**
    Find the full path of shaders.xml file.

    The reason we do not directly call nMaxOption::GetHomePath() is that 
    nMaxOption is mostly used for exporting. 
    It is singleton class so calling that here makes hard to handle and destroy 
    its instance.

    @param file file to retrieve the path
*/
static
nString GetShaderXmlPath(nString file)
{
    nString shdxml;

    nString iniFilename;
    iniFilename += GetCOREInterface()->GetDir(APP_PLUGCFG_DIR);
    iniFilename += "\\";
    iniFilename += N_MAXEXPORT_INIFILE;

    nFileServer2* fileServer = nFileServer2::Instance();

    // check the .ini file exist in 3dsmax plugin directory.
    if (!fileServer->FileExists(iniFilename))
    {
		// try to find the .ini file in the 'scripts\nebula' dirtectory. e.g.) d:\3dsmax\Scripts\nebula2
		iniFilename = "";
        iniFilename += GetCOREInterface()->GetDir(APP_SCRIPTS_DIR);
        iniFilename += "\\";
		iniFilename += "nebula2\\";
        iniFilename += N_MAXEXPORT_INIFILE;
        if (!fileServer->FileExists(iniFilename))
        {
            // .ini file does not exist in '/plugcfg' directory.
            n_message("%s file does not exist in '$3dsmax/plugcfg' directory.\n", 
                N_MAXEXPORT_INIFILE);

            // return a empty string.
            return shdxml;
		}
    }

    nString homeDir;

    // read 'home' path from existing .ini flle in '$3dsmax/plugcfg' directory.
    nIniPrefServer* iniFile = (nIniPrefServer*)nKernelServer::Instance()->New("niniprefserver", "/iniprefsrv");
    iniFile->SetFileName(iniFilename);
    iniFile->SetSection("GeneralSettings");
    homeDir = iniFile->ReadString("HomeDir");
    iniFile->Release();

    if (homeDir.IsEmpty())
    {
        n_message("Home directory does not exist in %s file.\n", iniFilename.Get());

        // return a empty string.
        return shdxml;
    }

    shdxml += homeDir;
    shdxml += "\\";
    //shdxml += "data\\shaders\\shaders.xml";
    shdxml += "data\\shaders\\";
    shdxml += file;

    if (!nFileServer2::Instance()->FileExists(shdxml))
    {
        n_message("File %s does not exist.\n", shdxml.Get());

        // make the string to be empty then return.
        shdxml += "";
        return shdxml;
    }

    return shdxml;
}

//-----------------------------------------------------------------------------
/**
    Retrieves parameter type based on the given shader type.

    -02-Nov-06  kims  Replaced point4 to floattab to retrieve valid values from 3dsmax control.
                      Thank ZHANG Zikai for the patch.

    @param shdType shader type which described in xml file.
    @return string which represent parameter type. 
*/
static
nString GetParameterType(const nString &shdType)
{
    if (shdType == "Int")
        return "#integer";
    else
    if (shdType == "Bool")
        return "#boolean";
    else
    if (shdType == "Float")
        return "#float";
    else
    if (shdType == "String")
        return "#string";
    else
    if (shdType == "Enum")
        return "#integer";
    else
    if (shdType == "Color")
#if MAX_RELEASE >= 6000
        return "#frgba"; // we use rgba, for 3dsmax6 or higher
#else
        return "#color"; // 3dsmax5 or lower version does not support alpha in colorpicker.
#endif
    else
    if (shdType == "Vector")
//#if MAX_RELEASE >= 6000
//        return "#point4";
//#else
        return "#floatTab tabSize:4 tabSizeVariable:false";
//#endif
    else
    if (shdType == "Texture")
        return "#texturemap";
    else
    if (shdType == "BumpTexture")
        return "#texturemap";
    else
    if (shdType == "CubeTexture")
        return "#texturemap";
    else
    if (shdType == "EnvelopeCurve")
        //return "#maxObject";
#if USE_ACTIVEX_ENVELOPECURVE_CTRL
        return "#floatTab tabSize:11 tabSizeVariable:false";
#else
        return "#floatTab tabSize:9 tabSizeVariable:false";
#endif
    else
    if (shdType == "ColorEnvelopeCurve" )
#if USE_ACTIVEX_ENVELOPECURVE_CTRL
        // add 2 params(min,max)
        return "#floatTab tabSize:14 tabSizeVariable:false";
#else
        return "#floatTab tabSize:12 tabSizeVariable:false";
#endif
    else
    {
        return "<<unknown>>";
    }
}

//-----------------------------------------------------------------------------
/**
    Retrieves UI from the given type.
    @note 
        EnvelopeCurve and ColorEnvelopeCurve are ignored.

    The followings are parameters type and its corresponding UI control.

      -# Int - spinner
      -# Float - spinner
      -# color - ColorPicker
      -# Enum - dropdownlist
      -# Texture - mapbutton
      -# BumpTexture - mapbutton
      -# CubeTexture - mapbutton
      -# Vector - spinner
      -# EnvelopeCurve - nmaxenvelopecurve custom control
      -# ColorEnvelopeCurve - nmaxenvelopecurve custom control
      -# Unknown - label

    @param type 'type' value of xml element.

    -17-Aug-06  kims Changed to add texutre directory setting button.
*/
static
nString GetUIFromType(TiXmlElement* elemParam, const nString &shdName, const nString &shaderHandler, const nString &type)
{
    if (type == "Int")
        return AddSpinner(shdName, shaderHandler, false, elemParam);
    else
    if (type == "Bool")
        return AddCheckBox(shdName, shaderHandler, elemParam);
    else
    if (type == "Float")
        return AddSpinner(shdName, shaderHandler, true, elemParam);
    else
    if (type == "Color")
        return AddColorPicker(elemParam);
    else
    if (type == "Enum")
        return AddDropdownList(shdName, shaderHandler, elemParam);
    else
    if (type == "Texture")
    {
        nString ret;

        // Caution: The plugin max script code is always regenerated and
        //          executed at the start time of 3dsmax so it is useless to 
        //          specifying any initial value to a control or parameter block.
        ret += AddMapButton(elemParam);
        ret += AddSetDirDlg(elemParam);
        return ret;
    }
    else
    if (type == "BumpTexture")
    {
        nString ret;

        // Caution: The plugin max script code is always regenerated and
        //          executed at the start time of 3dsmax so it is useless to 
        //          specifying any initial value to a control or parameter block.
        ret += AddMapButton(elemParam);
        ret += AddSetDirDlg(elemParam);
        return ret;
    }
    else
    if (type == "CubeTexture")
    {
        nString ret;

        // Caution: The plugin max script code is always regenerated and
        //          executed at the start time of 3dsmax so it is useless to 
        //          specifying any initial value to a control or parameter block.
        ret += AddMapButton(elemParam);
        ret += AddSetDirDlg(elemParam);
        return ret;
    }
    else
    if (type == "Vector")
        return AddVector4Spinner(shdName, shaderHandler, elemParam);
    else
    if (type == "EnvelopeCurve" || type == "ColorEnvelopeCurve")
        return AddEnvelopeCurve(shdName, shaderHandler, elemParam);
    else
    {
        nString uiScript;
        uiScript += "\t\t";
        uiScript += "label Unknown \"Unknown\"\n";

        return uiScript;
    }
}

//-----------------------------------------------------------------------------
/**
    The followinig maxscript is generated:
    @verbatim
    param = "" 
    param += (val.r/255.0) as string 
    param += " "
    param += (val.g/255.0) as string
    param += " "
    param += (val.b/255.0) as string
    param += " "
    param += (val.a/255.0) as string
    param += " "
    nChangeShaderParameter "Standard" "common" "MatSpecular" param
    @endverbatim
*/
static
nString GetStringForColorPicker(const nString &shdName, const nString &type, const nString &paramName)
{
    nString str;

    nString indent = "\t\t\t\t";

    str += indent + "param = \"\" \n";
    str += indent + "param += (val.r/255.0) as string \n";
    str += indent + "param += \" \"\n";
    str += indent + "param += (val.g/255.0) as string\n";
    str += indent + "param += \" \"\n";
    str += indent + "param += (val.b/255.0) as string\n";
    str += indent + "param += \" \"\n";
    str += indent + "param += (val.a/255.0) as string\n";
    str += indent + "param += \" \"\n";

    str += indent + "nChangeShaderParameter ";
    str += "\""; str += shdName; str += "\""; //e.g. "Standard"
    str += " ";
    str += "\""; str += type; str += "\"";
    str += " ";
    str += "\""; str += paramName; str += "\""; // e.g. "MatDiffuse"
    str += " ";
    str += "param\n";

    return str;
}

//-----------------------------------------------------------------------------
/**
    The followinig maxscript is generated:
    @verbatim
    nChangeShaderParameter "Standard" "common" "DiffMap0" val.filename
    @endverbatim
*/
static
nString GetStringForMapButton(const nString &shdName, const nString &type, const nString &paramName)
{
    nString script;
    nString tmp;

    tmp.Format("\t\t\t\tnChangeShaderParameter \"%s\" \"%s\" \"%s\" val.filename\n", 
        shdName.Get(), type.Get(), paramName.Get());
    script += tmp;

    return script;
}

//-----------------------------------------------------------------------------
/**
    The followinig maxscript is generated:
    @verbatim
    param = "" 
    param += val as string 
    nChangeShaderParameter "Standard" "common" "MatEmissiveIntensity" param
    @endverbatim
*/
static
nString GetStringForDefault(const nString &shdName, const nString &type, const nString &paramName)
{
    nString script;
    nString tmp;

    tmp.Format("\t\t\t\tparam = \"\" \n");
    script += tmp;

    tmp.Format("\t\t\t\tparam += val as string \n");
    script += tmp;

    tmp.Format("\t\t\t\tnChangeShaderParameter \"%s\" \"%s\" \"%s\" param\n", 
        shdName.Get(), type.Get(), paramName.Get());
    script += tmp;

    return script;
}

//-----------------------------------------------------------------------------
/**
    Generate max script code for a event which occurred a value of a control is
    changed.

    The following maxscript is one of an example which might be generated:
    @verbatim
    on RenderPri changed val do 
    (
        nChangeShaderParameter "Standard", "common", "RenderPri", "1.0 1.0 1.0 1.0"
    )
    @endverbatim

    -17-Aug-06  kims Added color picker to support MatAmbient.
*/
static
nString GetIpcEventHandler(const nString &shdName, const nString &paramName)
{
    nString handler;
    nString tmp;

    handler += "\t\t\tif nIsConnectedIpc() do\n";
    handler += "\t\t\t(\n";

    //color picker
    if (paramName == "MatDiffuse"  ||
        paramName == "MatEmissive" ||
        paramName == "MatSpecular" ||
        paramName == "MatAmbient") 
    {
        handler += GetStringForColorPicker(shdName, "common", paramName);
    }
    else
    if (paramName == "MatEmissiveIntensity" ||
        paramName == "MatSpecularPower"     ||
        paramName == "BumpScale")
    {
        handler += GetStringForDefault(shdName, "common", paramName);
    }
    else
    if (paramName == "AlphaSrcBlend" ||
        paramName == "AlphaDstBlend")
    {
        handler += GetStringForDefault(shdName, "common", paramName);
    }
    //texture map
    else
    if (strstr(paramName.Get(), "DiffMap") ||
        strstr(paramName.Get(), "BumpMap") ||
        strstr(paramName.Get(), "CubeMap") )
    {
        handler += GetStringForMapButton(shdName, "common", paramName);
    }
    else
    {
        tmp.Format("\t\t\t\tprint \"Unknown material type: shader name: %s parameter name: %s\"\n", 
            shdName.Get(), paramName.Get());
        handler += tmp;
    }

    handler += "\t\t\t)\n";

    return handler;
}

//-----------------------------------------------------------------------------
/**
    Retrieves event handler for each parameters.

    @note
        Because we use custom attribute for plugin's parameters,
        we cannot directly access its parameters of superclass with 'delegate'
        local variable. Instead of doing that, we get current material in the
        material editor then access its delegate like this:
    @verbatim
    on MapAmbient set val do
    (
        -- get the current material in the material editor.
        -- this is the material which currently selected material in the slot.
        curMaterial = medit.GetCurMtl()

        -- now, we can access the ambient value of the superclass.
        curMaterial.delegate.ambient = val
    )
    @endverbatim

    @param paramName parameter name which in parameter block.
    @return string which to be appended for parameter's event handler.
*/
static
nString GetEventHandler(const nString &shdName, const nString &paramName)
{
    bool validParam = true;
    nString handler;

    handler += "\t\ton ";
    handler += paramName;
    handler += " set val do \n";
    handler += "\t\t(\n";
    nShaderState::Param param = nShaderState::StringToParam(paramName.Get());
    switch(param)
    {
    case nShaderState::MatAmbient:
        handler += "\t\t\tOn" + paramName + "Changed val\n";
        break;

    case nShaderState::MatDiffuse:
        handler += "\t\t\tOn" + paramName + "Changed val\n";
        break;

    case nShaderState::MatSpecular:
        handler += "\t\t\tOn" + paramName + "Changed val\n";
        break;

    case nShaderState::MatEmissive:
        break;
    case nShaderState::MatEmissiveIntensity:
        break;
    case nShaderState::MatSpecularPower:
        break;
    case nShaderState::AlphaSrcBlend:
    case nShaderState::AlphaDstBlend:
        break;

    case nShaderState::DiffMap0:
    case nShaderState::DiffMap1:
    case nShaderState::DiffMap2:
    case nShaderState::DiffMap3:
    case nShaderState::DiffMap4:
    case nShaderState::DiffMap5:
    case nShaderState::DiffMap6:
    case nShaderState::DiffMap7:
        handler += "\t\t\tOn" + paramName + "Changed val\n";
        break;

    case nShaderState::BumpMap0:
    case nShaderState::BumpMap1:
    case nShaderState::BumpMap2:
    case nShaderState::BumpMap3:
        handler += "\t\t\tOn" + paramName + "Changed val\n";
        break;

    case nShaderState::CubeMap0:
    case nShaderState::CubeMap1:
    case nShaderState::CubeMap2:
    case nShaderState::CubeMap3:
        handler += "\t\t\tOn" + paramName + "Changed val\n";
        break;

    case nShaderState::SpecMap0:
    case nShaderState::SpecMap1:
    case nShaderState::SpecMap2:
    case nShaderState::SpecMap3:
        handler += "\t\t\tOn" + paramName + "Changed val\n";
        break;

    default:
    case nShaderState::InvalidParameter:
        // put empty handler.
        validParam = false;
        break;
    }

    if (validParam)
    {
        handler += GetIpcEventHandler(shdName, paramName);
    }

    handler += "\t\t) \n";

    return handler;
}

//-----------------------------------------------------------------------------
/**
    Generate script plug-in script.(part of 'param' element in xml)
    
    -02-Nov-06  kims  Changd to 'Vector' type correctly. Thank ZHANG Zikai for the patch.
                      
    @param name 'name' attribute in param element
                 The followings are known names:
                 'name', 'label', 'type', 'gui', 'export', 'min', 'max', 'def', 'enum'
*/
static
void GenerateScript(TiXmlElement* elemParam, nString& shdName, nString& strParamBlock, 
                    nString &strRollout, nString &openEvent, nString &closeEvent, nString &createEvent)
{
    nString tmp;

    // start to generate script for param block clause.

    // parameter name in parameter block.
    nString paramName = elemParam->Attribute("name");
    // parameter type
    nString paramType = elemParam->Attribute("type");
    nString shaderHandler = "common";
    int hasGui;
    elemParam->Attribute("gui", &hasGui);

    // not a common shader handler
    if (nShaderState::StringToParam(paramName.Get()) == nShaderState::InvalidParameter)
    {
        if ("Particle2" == shdName )
        {
            shaderHandler = "particle2";
        }
        else if ("Leaf" == shdName || "Tree" == shdName)
        {
            shaderHandler = "swing";
        }
        else
        {
            shaderHandler = "";
        }
    }

#if 0
    if (paramType == "ColorEnvelopeCurve")
    {
        n_assert(hasGui);
        tmp.Format("\t\t%s_v0 type:%s ui:%s_v0\n", paramName.Get(), GetParameterType("Color").Get(), paramName.Get());
        strParamBlock += tmp;
        tmp.Format("\t\t%s_v1 type:%s ui:%s_v1\n", paramName.Get(), GetParameterType("Color").Get(), paramName.Get());
        strParamBlock += tmp;
        tmp.Format("\t\t%s_v2 type:%s ui:%s_v2\n", paramName.Get(), GetParameterType("Color").Get(), paramName.Get());
        strParamBlock += tmp;
        tmp.Format("\t\t%s_v3 type:%s ui:%s_v3\n", paramName.Get(), GetParameterType("Color").Get(), paramName.Get());
        strParamBlock += tmp;

        tmp.Format("\t\t%s_p1 type:%s ui:%s_p1\n", paramName.Get(), GetParameterType("Float").Get(), paramName.Get());
        strParamBlock += tmp;
        tmp.Format("\t\t%s_p2 type:%s ui:%s_p2\n", paramName.Get(), GetParameterType("Float").Get(), paramName.Get());
        strParamBlock += tmp;

        strRollout += GetUIFromType(elemParam, shdName, shaderHandler, paramType);

        // append event handler.
        strParamBlock += GetEventHandler(shdName, paramName, );
    }
    else
#endif
    {
        strParamBlock += "\t\t";
        strParamBlock += paramName;
        strParamBlock += " ";

        strParamBlock += "type:";
        strParamBlock += GetParameterType(paramType);
        strParamBlock += " ";

        // if the type is one of any texture types, specifies false to this parameter.
        // default setting is 'true' on animatable parameter.
        if (strstr(paramType.Get(), "Texture"))
        {
            //strParamBlock += "animatable:false";
            strParamBlock += "animatable:true";
            strParamBlock += " ";
        }

        // specify default value of the parameter.
        if (paramType == "Vector" || paramType == "ColorEnvelopeCurve")
        {
            // we don't need default values for 'vector' and 'ColorEnvelopeCurve' type 
            // those are types uses floattab.
            ;
        }
        else
        if (paramType == "EnvelopeCurve")
        {
            /*
            nString defaultValue = GetDefault(elemParam);
            if (!defaultValue.IsEmpty())
            {
                strParamBlock += "default:";
                strParamBlock += defaultValue;
                strParamBlock += " ";
            }
            */
        }
        else
        {
            nString defaultValue = GetDefault(elemParam);
            if (!defaultValue.IsEmpty())
            {
                strParamBlock += "default:";
                strParamBlock += defaultValue;
                strParamBlock += " ";
            }
        }

        // if there's gui, we create ui in the given rollout.
        if (hasGui)
        {
            if (paramType == "EnvelopeCurve")
            {
                // we do not combine ui if the parameter is envelopecurve or colorenvelopecurve due to 
                // it is hard to bind those control type with parameters block.
#if USE_ACTIVEX_ENVELOPECURVE_CTRL
                // openEvent
				int i = 1;
                tmp.Format("\t\t\t%s_modulation.selection = %s[9];\n", paramName.Get(), paramName.Get(), paramName.Get() );    openEvent += tmp;
                tmp.Format("\t\t\tparams = \"\"\n");    openEvent += tmp;
                for( i = 1; i<6; ++i )
                {
                    tmp.Format("\t\t\tparams += (%s[%d] as string) + \", \"\n", paramName.Get(), i);    openEvent += tmp;
                }
                tmp.Format("\t\t\tparams += (%s[%d] as string)\n", paramName.Get(), i);    openEvent += tmp;
                tmp.Format("\t\t\t%s%s.InitCurve params\n", particlePrefix, paramName.Get() );    openEvent += tmp;

                // createEvent
                nString defaultValue = GetDefault(elemParam);
                tmp.Format("\t\t\t%s_def = %s", paramName.Get(), defaultValue.Get()); createEvent +=tmp;
                tmp.Format("\t\t\tfor i = 1 to %s_def.count do\n", paramName.Get()); createEvent +=tmp;
                tmp.Format("\t\t\t(\n"); createEvent +=tmp;
                tmp.Format("\t\t\t\t%s[i] = %s_def[i]\n", paramName.Get(), paramName.Get()); createEvent +=tmp;
                tmp.Format("\t\t\t)\n"); createEvent +=tmp;

                // link ui
                strParamBlock += "ui:";
                nString name = paramName;
                nString activeXName = particlePrefix + paramName + ", ";
                strParamBlock += "(" + activeXName + activeXName + activeXName + activeXName + activeXName + activeXName;
                strParamBlock += name + "_freq, ";
                strParamBlock += name + "_ampl, ";
                strParamBlock += name + "_modulation, ";
                strParamBlock += name + "_min, ";
                strParamBlock += name + "_max";
                strParamBlock += ")";
#else
                // 'ui' name should be same as parameter name.
                strParamBlock += "ui:";
                nString name = paramName;
                strParamBlock += "(";
                strParamBlock += name + "_v0, ";
                strParamBlock += name + "_v1, ";
                strParamBlock += name + "_v2, ";
                strParamBlock += name + "_v3, ";

                strParamBlock += name + "_p1, ";
                strParamBlock += name + "_p2, ";

                strParamBlock += name + "_freq, ";
                strParamBlock += name + "_ampl, ";
                strParamBlock += name + "_modulation";
                strParamBlock += ")";
#endif
            }
            else
            if (paramType == "ColorEnvelopeCurve")
            {
#if USE_ACTIVEX_ENVELOPECURVE_CTRL
                // open event
				int i = 1;
                tmp.Format("\t\t\tparams = \"\"\n");    openEvent += tmp;
                for( i = 1; i< 14; ++i )
                {
                    tmp.Format("\t\t\tparams += (%s[%d] as string) + \", \"\n", paramName.Get(), i);    openEvent += tmp;
                }
                tmp.Format("\t\t\tparams += (%s[%d] as string)\n", paramName.Get(), i);    openEvent += tmp;
                tmp.Format("\t\t\t%s%s.InitColorCurve params\n", particlePrefix, paramName.Get() );    openEvent += tmp;

                // createEvent
                tmp.Format("\t\t\tfor i = 1 to initColorCurve.count do\n"); createEvent +=tmp;
                tmp.Format("\t\t\t(\n"); createEvent +=tmp;
                tmp.Format("\t\t\t\t%s[i] = initColorCurve[i]\n", paramName.Get()); createEvent +=tmp;
                tmp.Format("\t\t\t)\n"); createEvent +=tmp;
#else
                // use color picker due to that we don't have envelope color curve control yet.
                tmp.Format("\t\t\t%s_v0.color = (color (%s[1]*255) (%s[2]*255) (%s[3]*255))\n", paramName.Get(), paramName.Get(), paramName.Get(), paramName.Get()); openEvent += tmp;
                tmp.Format("\t\t\t%s_v1.color = (color (%s[4]*255) (%s[5]*255) (%s[6]*255))\n", paramName.Get(), paramName.Get(), paramName.Get(), paramName.Get()); openEvent += tmp;
                tmp.Format("\t\t\t%s_v2.color = (color (%s[7]*255) (%s[8]*255) (%s[9]*255))\n", paramName.Get(), paramName.Get(), paramName.Get(), paramName.Get()); openEvent += tmp;
                tmp.Format("\t\t\t%s_v3.color = (color (%s[10]*255) (%s[11]*255) (%s[12]*255))\n", paramName.Get(), paramName.Get(), paramName.Get(), paramName.Get()); openEvent += tmp;
                tmp.Format("\t\t\t%s_p1.value = %s[13]\n", paramName.Get(), paramName.Get()); openEvent += tmp;
                tmp.Format("\t\t\t%s_p2.value = %s[14]\n", paramName.Get(), paramName.Get()); openEvent += tmp;
#endif
            }
            else
            {
                // 'ui' name should be same as parameter name.
                strParamBlock += "ui:";
                // combine the 4 spinner into a parameter
                if (paramType == "Vector")
                {
                    // names should be same as the spinners
                    // see AddVector4Spinner
                    nString name = paramName;
                    strParamBlock += "(";
                    name.AppendInt(0);
                    strParamBlock += name;
                    strParamBlock += ", ";
                    name.AppendInt(1);
                    strParamBlock += name;
                    strParamBlock += ", ";
                    name.AppendInt(2);
                    strParamBlock += name;
                    strParamBlock += ", ";
                    name.AppendInt(3);
                    strParamBlock += name;
                    strParamBlock += ")";
                }
                else
                {
                    strParamBlock += paramName;
                }
            }
            strParamBlock += " \n";

            strRollout += GetUIFromType(elemParam, shdName, shaderHandler, paramType);
        }

        // append event handler.
        strParamBlock += GetEventHandler(shdName, paramName);

        // if it is texture type, add destination directory setting value.
        if (strstr(paramType.Get(), "Texture") && hasGui)
        {
            //HACK: 'dirSetting' is used again in AddSetDirDlg() function and nMaxMaterial::GetNebulaMaterial().
            //      So, if you change the string, the string in AddSetDirDlg() also should be changed.
            //      The directory parameter has "" for default string. It is absolutely necessary in Max6.
            //      Without that, the plug-in with the panel have those contorls does not correctly work.

            tmp.Format("\t\tdirSetting%s type:#string default:\"\" ui:edtFld%s\n", 
                paramName.Get(), paramName.Get());
            strParamBlock += tmp;
        }
    }
}

//-----------------------------------------------------------------------------
/**
    Parse each of the elements in xml and generate parameter block and rollout.

    @param elemShader element of the given xml
    @param strParamBlock parameter block script which to be generated.
    @param strRollout rollout script which to be generated.
    @param openEvent
    @param closeEvent
*/
static
void ParseParams(TiXmlElement* elemShader, nString& shdName, nString& strParamBlock, 
                 nString &strRollout, nString &openEvent, nString &closeEvent, nString &createEvent)
{
    nString tmp;

    nString shdType = elemShader->Attribute("shaderType");
    nString meshType = elemShader->Attribute("meshType");

    // Get .shader name.
    nString effectFileName = elemShader->Attribute("file");

    // 'Shader' parameter in parameter block.
    tmp.Format("\t\tShader type:#string default:\"%s\" animatable:false\n", effectFileName.Get());
    strParamBlock += tmp;

    TiXmlElement* elemParam = elemShader->FirstChild("param")->ToElement();
    for (elemParam; elemParam; elemParam = elemParam->NextSiblingElement())
    {
        GenerateScript(elemParam, shdName, strParamBlock, strRollout, openEvent, closeEvent, createEvent);
    }
}

//-----------------------------------------------------------------------------
/**
    Filter of the shader in filter array.
*/
static
bool DoFilter(const nString &str)
{
    for (int i=0; i<shaderFilterArray.Size(); i++)
    {
        if (shaderFilterArray[i] == str)
            return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
/**
    Add script plugin event handlers.
*/
static
nString AddPluginEventHandlers()
{
    nString script;
    nString tmp;

    // 'create' plugin event handlers
    tmp.Format("\ton create do\n\t(\n\t\tOnPluginCreate()\n\t)\n");
    script += tmp;

    // 'postCreate' plugin event handlers
    tmp.Format("\ton postCreate do\n\t(\n\t\tOnPluginPostCreate()\n\t)\n");
    script += tmp;

    // 'load' plugin event handlers
    tmp.Format("\ton load do\n\t(\n\t\tOnPluginLoad()\n\t)\n");
    script += tmp;

    // 'postLoad' plugin event handlers
    tmp.Format("\ton postLoad do\n\t(\n\t\tOnPluginPostLoad()\n\t)\n");
    script += tmp;

    return script;
}

//-----------------------------------------------------------------------------
/**
    Retrieve all shader names from xml file (e.g. shader.xml)
*/
TiXmlHandle GetShaderFromDataBase(TiXmlHandle& xmlHandle, const nString &shader)
{
    // retrieves all shader names in xml file.
    TiXmlElement* child = xmlHandle.FirstChild("NebulaShaderDatabase").FirstChild("shader").Element();
    for (child; child; child=child->NextSiblingElement())
    {
        // get shader name.
        nString name = child->Attribute("file");
        if (name == shader)
        {
            return child;
        }
    }
    return TiXmlHandle(0);
}

//-----------------------------------------------------------------------------
/**
    Retrieve parameter from given shader.
*/
TiXmlHandle GetParamFromShader(TiXmlHandle& shader, const nString &param)
{
    TiXmlElement* child = shader.FirstChild("param").Element();
    for (child; child; child=child->NextSiblingElement())
    {
        nString name = child->Attribute("name");
        if (name == param)
            return TiXmlHandle(child);
    }
    return TiXmlHandle(0);
}

//-----------------------------------------------------------------------------
/**
    Retrieve shader name which to be filtered out.
    The shader names are listed in the filter.xml file which can be found 
    under the same directory with shader.xml file.
*/
bool GetShaderFilter()
{
    // Get the full path of '$nebula/data/shaders/filter.xml' file.
    nString filterXmlFilePath = GetShaderXmlPath("filter.xml");
    if (filterXmlFilePath.IsEmpty())
    {
        n_listener("Cannot find filter.xml file.\n");
        return false;
    }

    TiXmlDocument xmlDoc;

    // Load the shaders.xml file
    if (!xmlDoc.LoadFile(filterXmlFilePath.Get()))
    {
        n_listener("Filed to load %s.\n", filterXmlFilePath.Get());
        return false;
    }

    TiXmlHandle xmlHandle(&xmlDoc);
    TiXmlElement* child;

    shaderFilterArray.Clear();

    child = xmlHandle.FirstChild("NebulaShaderFilter").FirstChild("shader").Element();
    for (child; child; child=child->NextSiblingElement())
    {
        // get shader name.
        nString name = child->Attribute("name");

        shaderFilterArray.Append(name);
    }

    return true;
}

//-----------------------------------------------------------------------------
/**
    max script expose function which to generate material script plugin based on 
    '$nebula2/data/shaders/shaders.xml' file.

    The following max script generate nebula2 custom material plugin. 
    @verbatim
    nCreateCustomMaterialPlugin() 
    @endverbatim

    @note
    Do not call this script explicitly when 3DS Max running.
    This function should be called at once when 3DS Max start up.
    If not, all previously specified materials in the slot will be reset.

    The generated file, "n2materialplugin.ms" can be found in the 
    '$3dsmax/scripts/' directory. Use it for the purpose of test or debugging.
    It is only available on the debug version of plugin.

    @return true if we success to generate scripted plug-in.
*/
bool EvalCustomMaterialPlugin()
{
    // Get shader name which to be filted out.
    if (!GetShaderFilter())
    {
        n_listener("Cannot find filter.xml file.\n");
        shaderFilterArray.Clear();
    }

    // Get the full path of '$nebula/data/shaders/shaders.xml' file.
    nString shdXmlFilePath = GetShaderXmlPath(nString("shaders.xml"));
    if (shdXmlFilePath.IsEmpty())
    {
        n_listener("Cannot find shaders.xml file.\n");
        return false;
    }

    TiXmlDocument xmlDoc;

    // Load the shaders.xml file
    if (!xmlDoc.LoadFile(shdXmlFilePath.Get()))
    {
        n_listener("Filed to load %s.\n", shdXmlFilePath.Get());
        return false;
    }

    TiXmlHandle xmlHandle(&xmlDoc);

    TiXmlElement* child;

    // array for shader name of shader element.
    nArray<nString> shaderArray;
    shaderArray.Append("None");
   
    nArray<nString> custAttribNameArray;
    custAttribNameArray.Append("undefined");

    nArray<nString> effectFileArray;
    effectFileArray.Append("'None'");

    // array for containing generated custom attribute script.
    nArray<nString> custAttribArray;
    nString tmp;

    // retrieves all shader names in xml file.
    child = xmlHandle.FirstChild("NebulaShaderDatabase").FirstChild("shader").Element();
    for (child; child; child=child->NextSiblingElement())
    {
        // get shader name.
        nString name = child->Attribute("name");

        // if the given shader name is found in the shader filter,
        // the plugin does not generate script code for the shader.
        if (DoFilter(name))
            continue;

        nString caName = name;

        // get shader name.
        nString effectFile;
        effectFile += "'";
        effectFile += child->Attribute("file");
        effectFile += "'";
        effectFileArray.Append(effectFile);

        shaderArray.Append(name);

        nString custattrib, custattribName;
        
        // we append 'ca' prefix to avoid name confliction.
        // e.g. "Standard' and "Ocean' are already defined keyword in max script.
        custattribName +="ca";
        custattribName += nMaxUtil::CorrectName(name);
        custAttribNameArray.Append(custattribName);

        tmp.Format("%s = attributes \"%s\"\n(\n", custattribName.Get(), caName.Get());
        custattrib += tmp;

        // parameter block and rollout clause.
        nString paramBlock, rollout, openEvent, closeEvent, createEvent;

        nString shdName = nMaxUtil::CorrectName(name);

        nString rolloutName = "r" + shdName; //e.g. "rStandard"

        tmp.Format("\tparameters %s rollout:%s\n\t(\n", shdName.Get(), rolloutName.Get());
        paramBlock += tmp;

        // begin rollout
        tmp.Format("\trollout %s \"%s Parameters\"\n", rolloutName.Get(), name.Get());
        rollout += tmp;

        rollout += "\t(\n";

        // parse each param elements in shader element.
        ParseParams(child, caName, paramBlock, rollout, openEvent, closeEvent, createEvent);

        paramBlock += "\t)\n";

        //if (!openEvent.IsEmpty())
        {
            tmp.Format("\t\ton %s open do\n\t\t(\n", rolloutName.Get());
            rollout += tmp;
 
            rollout += openEvent;

            //HACK: ugly hardcoding, we assume if the rollout name contains "Particle",
            //       it uses activex control and enables accelerators.
            if (strstr(rolloutName.Get(), "Particle"))
            {
                tmp.Format("\t\t\tenableAccelerators = false\n");
                rollout += tmp;
            }

            rollout += "\t\t)\n";
        }

        //if (!closeEvent.IsEmpty())
        {
            tmp.Format("\t\ton %s close do\n\t\t(\n", rolloutName.Get());
            rollout += tmp;

            rollout += closeEvent;

            if (strstr(rolloutName.Get(), "Particle"))
            {
                tmp.Format("\t\t\tenableAccelerators = true\n");
                rollout += tmp;
            }

            rollout += "\t\t)\n";
        }

        rollout += "\t)\n";;

        if( !createEvent.IsEmpty() )
        {
            tmp.Format("\t\ton create do\n\t\t(\n");
            rollout += tmp;
            rollout += "\t\t\tinitCurve = #(0.0, 0.3, 0.7, 1.0, 0.2, 0.8, 0.0, 0.0, 0.0, 0.0, 1.0)\n";
            rollout += "\t\t\tinitColorCurve = #(1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.2, 0.8)\n";
            rollout += createEvent;
            rollout += "\t\t)\n";
        }

        custattrib += paramBlock;
        custattrib += rollout;

        custattrib += "\n)\n";

        custAttribArray.Append(custattrib);
    }

    int i;
    nString script;

    script += "--------------------------------------------------------------------------------\n";
    script += "--  The Nebula2 3dsmax Toolkit MaxScript code \n";
    script += "--  This file was automatically generated and executed when 3dsmax started. \n";
    script += "--------------------------------------------------------------------------------\n";

    // define 'materialhandlers.ms' including script.
    script += "include \"nebula2\\materialhandlers.ms\"\n\n";

    // add custom attributes.
    for (i=0; i<custAttribArray.Size(); i++)
    {
        script += custAttribArray[i];
        script += "\n";
    }

    // script plugin header
    const int version = 100;

    nString strVer;
    strVer.AppendInt(version);

    script += "plugin material Nebula2Material\n";
    script += "\tname:\"Nebula2\"";
    script += "\n";
    script += "\tclassID:#(";
    script += "0x6b5ae640";
    script += ", ";
    script += "0x319f53b8";
    script += ")\n";
    script += "\textends:Standard\n";
    script += "\treplaceUI:true\n";
    script += "\tversion:";
    script += strVer; // version
    script += "\n(\n";

    // script plugin body.

    // local variable : array of customAttributes
    script += "\tlocal customAttributes = #(";
    for (i=0; i<custAttribNameArray.Size(); i++)
    {
        script += custAttribNameArray[i];

        if (i < custAttribNameArray.Size() - 1)
            script += ",";
    }
    script += ")\n";

    // local variable : array of shader types
    script += "\tlocal effectFileNames = #(";
    for (i=0; i<effectFileArray.Size(); i++)
    {
        script += "\"";
        script += effectFileArray[i];
        script += "\"";
        if (i < effectFileArray.Size() - 1)
            script += ",";
    }
    script += ")\n";

    // parameter block
    script += "\tparameters SelectShader rollout:main \n"; 
    script += "\t(\n"; 
    script += "\t\tparamCurShader type:#integer default:1 animatable:false \n"; 
    script += "\t\tparamEffectFile type:#string default:\"'None'\" animatable:false ui:effectFileName \n"; 
    script += "\t)\n";

    nString mainRollout;

    mainRollout += "\trollout main ";
    mainRollout += "\"Nebula2 Shader Types\"\n";
    mainRollout += "\t(\n";

    // insert dropdownlist script for shader selection to rollout clause.
    nString uiShdName = "Shader";
    mainRollout += "\t\t";
    mainRollout += "dropdownlist ";
    mainRollout += uiShdName;
    mainRollout += " ";

    mainRollout += "\"Select Shader\"";
    mainRollout += " ";

    mainRollout += "width:160";
    mainRollout += " ";

    mainRollout += "items:#(";
    for (i=0; i<shaderArray.Size(); i++)
    {
        mainRollout += "\"";
        mainRollout += shaderArray[i];
        mainRollout += "\"";

        if (i < shaderArray.Size() - 1)
            mainRollout += ",";
    }
    mainRollout += ") ";
    mainRollout += "selection:1";
    mainRollout += "\n";
    // end of dropdownlist script for shader.

    mainRollout += "\t\tlabel effectFileName \"Effect File: 'None'\" align:#left \n";

    mainRollout += "\t\ton " + uiShdName + " " + "selected i do \n";
    mainRollout += "\t\t(\n";
    mainRollout += "\t\t\tcurMaterial = medit.GetCurMtl() \n";

    mainRollout += "\t\t\tif customAttributes[paramCurShader] != undefine do \n";
    mainRollout += "\t\t\t(\n";
    mainRollout += "\t\t\t\tnumCustAttr = custAttributes.count curMaterial \n";
    mainRollout += "\t\t\t\tfor i = 1 to numCustAttr do\n";
    mainRollout += "\t\t\t\t(\n";
    mainRollout += "\t\t\t\t\tattrDef = custAttributes.getdef curMaterial i \n";
    mainRollout += "\t\t\t\t\tif attrDef != undefined and attrDef.name == customAttributes[paramCurShader].name do \n";
    mainRollout += "\t\t\t\t\t(\n";
    mainRollout += "\t\t\t\t\t\tcustAttributes.delete curMaterial i \n";
    mainRollout += "\t\t\t\t\t)\n";
    mainRollout += "\t\t\t\t)\n"; // end of for
    mainRollout += "\t\t\t)\n";

    mainRollout += "\t\t\tif customAttributes[i] != undefined then \n";
    mainRollout += "\t\t\t(\n";
    mainRollout += "\t\t\t\tcustAttributes.add curMaterial customAttributes[i] \n";
    mainRollout += "\t\t\t)\n";
    mainRollout += "\t\t\tShader.selection = i \n";
    mainRollout += "\t\t\tparamCurShader = i \n";
    mainRollout += "\t\t\teffectFileName.caption = \"Effect File: \" + effectFileNames[i] \n";
    mainRollout += "\t\t\tparamEffectFile = effectFileNames[i]\n";
    mainRollout += "\t\t)\n";

    mainRollout += "\t\ton main open do \n";
    mainRollout += "\t\t(\n";
    mainRollout += "\t\t\tShader.selection = paramCurShader \n";
    mainRollout += "\t\t\teffectFileName.caption = \"Effect File: \" + paramEffectFile\n";
    mainRollout += "\t\t)\n";

    mainRollout += "\t)\n";

    script += mainRollout;

    script += AddPluginEventHandlers();

    script += "\n)";

    nString scriptsPath;
    scriptsPath += GetCOREInterface()->GetDir(APP_SCRIPTS_DIR);
#ifdef _DEBUG
    nString sampleFile;
    sampleFile += scriptsPath;
    sampleFile += "\\";
    sampleFile += "n2materialplugin.ms";

    nFile* file = nFileServer2::Instance()->NewFileObject();
    if (file->Open(sampleFile.Get(), "w"))
    {
        file->PutS(script);
        file->Close();
        file->Release();
    }
#endif

    // execute the generated script.
    if (!nMaxScriptCall(script.Get()))
    {
        n_listener("Failed to evaluate the generated script.\n");
        n_listener("See '$3dsmax/scripts/materialplugintemp.ms' for generated script.\n");

        // if we failed to evaluate the given script, put it to 3dsmax script directory

        nString debugFilePath;
        debugFilePath += scriptsPath;
        debugFilePath += "\\";
        debugFilePath += "materialplugintemp.ms";

        nFile* file = nFileServer2::Instance()->NewFileObject();
        if (file->Open(debugFilePath.Get(), "w"))
        {
            file->PutS(script);
            file->Close();
            file->Release();
        }

        // open 3dsmax script editor with the script which failed to be evaluated
        // for the purpose of debugging it.
        n_openeditor(debugFilePath.Get());

        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
/**
    Retrieve default value which is defined in shader database file(shader.xml)
    It is needed when the toolkit set default value if the parameter has not any value.

    e.g) The toolkit specifies "nobump.dds" if there is no bump map is specified 
    in the material editor when it exports.
*/
bool GetDefaultValueFromDataBase(const nString &shader, const nString &param, nString &outvalue)
{
    // Get the full path of '$nebula/data/shaders/shaders.xml' file.
    nString shdXmlFilePath = GetShaderXmlPath("shaders.xml");
    if (shdXmlFilePath.IsEmpty())
    {
        n_listener("Cannot find shaders.xml file.\n");
        return false;
    }

    TiXmlDocument xmlDoc;

    // Load the shaders.xml file
    if (!xmlDoc.LoadFile(shdXmlFilePath.Get()))
    {
        n_listener("Filed to load %s.\n", shdXmlFilePath.Get());
        return false;
    }

    TiXmlHandle xmlHandle(&xmlDoc);
    TiXmlElement* pElem = GetParamFromShader(GetShaderFromDataBase(xmlHandle, shader), param).Element();
    if (pElem == 0)
        return false;

    const char *attr = pElem->Attribute("def");
    if (attr == 0)
        return false;
    
    outvalue = attr;

    return true;
}
