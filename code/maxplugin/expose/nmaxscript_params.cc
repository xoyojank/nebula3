//------------------------------------------------------------------------------
//  nmaxscript_params.cc
//
//  (C)2004 Kim, Hyoun Woo.
//------------------------------------------------------------------------------
#include "util/nstring.h"
#include "kernel/ncmd.h"

#include "expose/nmaxscriptparams.h"

//-----------------------------------------------------------------------------
/**
    create 'spinner' control to edit float value.

    @param strVarName
    @param strCaption
    @param fVal
    @param fMin
    @param fMax
    @param bAcross
    @param uiAcrossCnt
    @param uiFieldWidth
    @param bOffset
*/
nString CreateFloatUI(nString strVarName, nString strCaption, float fVal, 
    float fMin, float fMax, bool bAcross = false, 
    unsigned int uiAcrossCnt = 1, unsigned int uiFieldWidth = 70, 
    bool bOffset = true)
{
    // we dont need caption for the control.
    // script command will be palced at the start of parsing of the 
    // inarguments instead of the caption.
    
    nString strDef = "spinner ";
    strDef += strVarName + " \"" + strCaption + "\" ";
    
    strDef += "type:#float ";
    strDef += "range:[";
    strDef.AppendFloat(fMin);
    strDef += " , ";
    strDef.AppendFloat(fMax);
    strDef += " , ";
    strDef.AppendFloat(fVal);
    strDef += "]";

    if (bAcross)
    {
        strDef += " across:";
        strDef.AppendInt(uiAcrossCnt);
    }
    strDef += " align:#left fieldwidth:"; 
    strDef.AppendInt(uiFieldWidth);
     
    if (bOffset)
        strDef += " offset:[0,6]";
     
    return strDef;
}
//-----------------------------------------------------------------------------
/**
    Add a single float type of parameter to custom attribute.

    @param objCmd This argument can be NULL.
*/
void AddFloatParam(const nObjectAttribute::Command* objCmd,
    nCmd* cmd, nArg* arg, nString& param, nString& rollout, nString& setupbody)
{
    n_assert(cmd);
    n_assert(arg);

    nString strAttrName = cmd->GetProto()->GetName();
    strAttrName.AppendInt(cmd->GetInArgIndex());

    nString strAttrUIName = strAttrName + "UI";
    strAttrUIName.AppendInt(cmd->GetInArgIndex());

    float fVal= arg->GetF();
    
    param += "\t\t";
    param += strAttrName + " type:#float default:";
    param.AppendFloat(fVal);

    param += " ui:";
    param += strAttrUIName;

    float fMax = 3.40E38f;
    float fMin = -3.40E38f;

    if (objCmd)
    {
        fMax = objCmd->GetMax();
        fMin = objCmd->GetMin();
    }
    
    rollout += CreateFloatUI(strAttrUIName, "", fVal, fMin, fMax);
}

//-----------------------------------------------------------------------------
/**
    Add 'vector2' type of parameter to custom attribute.
*/
void AddVector2Param(const nObjectAttribute::Command* objCmd,
    nCmd* cmd, nString& param, nString& rollout, nString& setupbody)
{
    n_assert(cmd);

    nString strAttrName = cmd->GetProto()->GetName();

    nString strAttrUIName = strAttrName + "UI";

    vector2 val;
    val.x = cmd->In()->GetF();
    val.y = cmd->In()->GetF();

    float max = 3.40E38f;
    float min = -3.40E38f;
    if (objCmd)
    {
        max = objCmd->GetMax();
        min = objCmd->GetMin();
    }

    param += strAttrName + " type:#floatTab tabSize:2 "
        "default:-339999995214436420000000000000000000000.000000 "
        "tabSizeVariable:true";
    param += " ui:(";

    uint uiSize = 2;
    for (unsigned int uj = 0; uj < uiSize; uj++)
    {
        strAttrUIName = strAttrName + "UI_";
        strAttrUIName.AppendInt(uj);

        param += strAttrUIName;

        if (uj != uiSize - 1)
            param += " , ";

        float fVal;

        if (uj == 0)
        {
            fVal = val.x;
            rollout += CreateFloatUI(strAttrUIName, "x: ",fVal, min, max, true, 2, 70, false);
        }
        else
        {
            fVal = val.y;
            rollout += CreateFloatUI(strAttrUIName, "y: ",fVal, min, max, false, 1, 70, false);
        }

        nString strSetupName = strAttrName + "["; 
        strSetupName.AppendInt(uj + 1);
        strSetupName += "]";

        setupbody += "if " + strSetupName + 
            " == -339999995214436420000000000000000000000.000000 then ";
        setupbody += strSetupName + " = ";
        setupbody.AppendFloat(fVal);
        setupbody += "\n\t\t";

        rollout += "\n\t\t";
    }

    param += ")";
}

//-----------------------------------------------------------------------------
/**
    Add 'vector3' type of parameter to custom attribute.
*/
void AddVector3Param(const nObjectAttribute::Command* objCmd,
    nCmd* cmd, nString& param, nString& rollout, nString& setupbody)
{
    n_assert(cmd);
     
    nString strAttrName = cmd->GetProto()->GetName();
    nString strAttrUIName = strAttrName + "UI";

    vector3 val;
    val.x = cmd->In()->GetF();
    val.y = cmd->In()->GetF();
    val.z = cmd->In()->GetF();

    float max = 3.40E38f;
    float min = -3.40E38f;
    if (objCmd)
    {
        max = objCmd->GetMax();
        min = objCmd->GetMin();
    }

    param += strAttrName + " type:#floatTab tabSize:3 "
        "default:-339999995214436420000000000000000000000.000000 "
        "tabSizeVariable:true";
    
    param += " ui:(";
    uint uiSize = 3;

    for (unsigned int uj = 0; uj < uiSize; uj++)
    {
        strAttrUIName = strAttrName + "UI_";
        strAttrUIName.AppendInt(uj);

        param += strAttrUIName;

        if (uj != uiSize -1)
            param += " , ";

        float fVal;

        if (uj == 0)
        {
            fVal = val.x;
            rollout += CreateFloatUI(strAttrUIName, "x: ",fVal, min, max, true, 3, 70, false);
        }
        else if (uj == 1)
        {
            fVal = val.y;
            rollout += CreateFloatUI(strAttrUIName, "y: ",fVal, min, max, false, 1, 70, false);
        }
        else
        {
            fVal = val.z;
            rollout += CreateFloatUI(strAttrUIName, "z: ",fVal, min, max, false, 1, 70, false);
        }
        
        nString strSetupName = strAttrName + "[";
        strSetupName.AppendInt(uj + 1);
        strSetupName +=  "]";

        setupbody += "if " + strSetupName + 
            " == -339999995214436420000000000000000000000.000000 then ";
        setupbody += strSetupName + " = ";
        setupbody.AppendFloat(fVal);
        setupbody += "\n\t\t";
        
        rollout += "\n\t\t";
    }
    
    param += ")";
}

//-----------------------------------------------------------------------------
/**
    Add 'vector4' type of parameter to custom attribute.
*/
void AddVector4Param(const nObjectAttribute::Command* objCmd,
    nCmd* cmd, nString& param, nString& rollout, nString& setupbody)
{
    n_assert(cmd);
    //n_assert(arg);

    float* pfVal = new float[4];
    pfVal[0] =  pfVal[1] =  pfVal[2] =  pfVal[3] =  0.0f;

    float max = 3.40E38f;
    float min = -3.40E38f;
    
    if (objCmd)
    {
        max = objCmd->GetMax();
        min = objCmd->GetMin();
    }

    //const nFloat4& val = cmd->In()->GetF4();
    vector4 val;
    val.x = cmd->In()->GetF();
    val.y = cmd->In()->GetF();
    val.z = cmd->In()->GetF();
    val.w = cmd->In()->GetF();

    pfVal[0] = val.x;
    pfVal[1] = val.y;
    pfVal[2] = val.z;
    pfVal[3] = val.w;

    nString strAttrName = cmd->GetProto()->GetName();
    nString strName = "";
    nString strAttrUIName = strAttrName + "UI";
    param += strAttrName + " type:#floatTab tabSize:4 "
        "default:-339999995214436420000000000000000000000.000000 "
        "tabSizeVariable:true";
    
    param += " ui:(";

    uint uiSize = 4;
    for (uint uj = 0; uj < uiSize; uj++)
    {
        strAttrUIName = strAttrName + "UI_";
        strAttrUIName.AppendInt(uj);

        param += strAttrUIName;
        
        if (uj != uiSize -1)
            param += " , ";

        if (uj == 0)
        {
            strName = "x: ";
            rollout += CreateFloatUI(strAttrUIName, strName, pfVal[uj], min, max, true, 4, 52, false);
        }
        else
        {
            if (uj == 1)
                strName = "y: ";
            if (uj == 2)
                strName = "z: ";
            if (uj == 3)
                strName = "w: ";

            rollout += CreateFloatUI(strAttrUIName, strName, pfVal[uj], min, max, false, 1, 52, false);
        }

        nString strSetupName = strAttrName + "[";
        strSetupName.AppendInt(uj + 1);
        strSetupName += "]";

        setupbody += "if " + strSetupName + 
            " == -339999995214436420000000000000000000000.000000 then ";
        setupbody += strSetupName + " = ";
        setupbody.AppendFloat(pfVal[uj]);
        setupbody += "\n\t\t";

        rollout += "\n\t\t";
    }

    param += ")";
    delete [] pfVal;
}

//-----------------------------------------------------------------------------
/**
    Add 'Matrix4' type of parameter to custom attribute.
*/
void AddMatrix4Param(const nObjectAttribute::Command* objCmd,
    nCmd* cmd, nString& param, nString& rollout, nString& setupbody)
{
    float fMin = -3.40E38f;
    float fMax = 3.40E38f;
    
    matrix44 val (cmd->In()->GetM44());

    nString strAttrName = cmd->GetProto()->GetName();
    nString strAttrUIName = strAttrName + "UI";
    param += strAttrName + " type:#floatTab tabSize:16 "
        "default:-339999995214436420000000000000000000000.000000 "
        "tabSizeVariable:true";
    
    param += " ui:(";
    unsigned int uiSize = 4;
    nString strName = "";
    
    for (unsigned int ui = 0; ui < uiSize; ui++)
    {
        for (unsigned int uj = 0; uj < uiSize; uj++)
        {
            strAttrUIName = strAttrName + "UI_";
            strAttrUIName.AppendInt(ui); strAttrUIName += "_";
            strAttrUIName.AppendInt(uj);

            if (ui == uiSize - 1 && uj == uiSize - 1)
                param += strAttrUIName;
            else
                param += strAttrUIName + ", ";

            float fVal = val.m[ui][uj];
            
            strName = "(";
            strName.AppendInt(ui); strName += ", ";
            strName.AppendInt(uj); strName += "): ";

            if (uj == 0)
            {
                rollout += CreateFloatUI(strAttrUIName, "", fVal, fMin, fMax, true, 4, 52, false);
            }
            else 
            {
                rollout += CreateFloatUI(strAttrUIName, "", fVal, fMin, fMax, false, 1, 52, false);
            }
            
            nString strSetupName = strAttrName + "[";
            strSetupName.AppendInt(ui*uiSize + uj + 1); 
            strSetupName += "]";

            setupbody += "if " + strSetupName + 
                " == -339999995214436420000000000000000000000.000000 then ";
            setupbody += strSetupName + " = ";
            setupbody.AppendFloat(fVal);
            setupbody += "\n\t\t";

            rollout += "\n\t\t";
        }
    }

    param += ")";
}

//-----------------------------------------------------------------------------
/**
    Add 'Bool' type of parameter to custom attribute.
*/
void AddBoolParam(const nObjectAttribute::Command* objCmd,
                   nCmd* cmd, nArg* arg,
                   nString& param, nString& rollout, nString& setupbody)
{
    n_assert(cmd);
    n_assert(arg);
     
    nString strAttrName = cmd->GetProto()->GetName();
    strAttrName.AppendInt(cmd->GetInArgIndex());

    nString strAttrUIName = strAttrName + "UI";
    strAttrUIName.AppendInt(cmd->GetInArgIndex());

    bool bValue;
    bValue = arg->GetB();

    param += "\t\t";
    param += strAttrName + " type:#boolean default:";
    if(bValue)
        param += "true";
    else
        param += "false";

    param += " ui:";
    param += strAttrUIName;

    rollout += "checkbox ";
    rollout += strAttrUIName + " \"" + 
    strAttrName + ": \" ";
    rollout += "checked: ";

    if (bValue)
        rollout += "true ";
    else
        rollout += "false ";

    rollout += " align:#left width:70 offset:[0,6]";
}

//-----------------------------------------------------------------------------
/**
    Add rgb 'Color' type of parameter to custom attribute.
*/
void AddRGBParam(const nObjectAttribute::Command* objCmd,
    nCmd* cmd, /*nArg* arg,*/
    nString& param, nString& rollout, nString& setupbody)
{
    n_assert(cmd);
     
    nString strAttrName = cmd->GetProto()->GetName();
    nString strAttrUIName = strAttrName + "UI";

    // retreive rgb values.
    vector3 val;
    val.x = cmd->In()->GetF();
    val.y = cmd->In()->GetF();
    val.z = cmd->In()->GetF();
    
    nString rgb;

    rgb.AppendFloat(val.x * 255.0f);    rgb += " ";
    rgb.AppendFloat(val.y * 255.0f);    rgb += " ";
    rgb.AppendFloat(val.z * 255.0f);    rgb += " ";

    param += strAttrName + " type:#color default:";
    param += "(color "+ rgb + ") ";
    param += " ui:" + strAttrUIName;
    param += "\n\t\t";

    rollout += "colorpicker ";
    rollout += strAttrUIName + " \"" + strAttrName + 
        ": \" align:#left across:2 offset:[0,6]\n\t\t";
}

//-----------------------------------------------------------------------------
/**
    Add rgba 'Color' type of parameter to custom attribute.
*/
void AddRGBAParam(const nObjectAttribute::Command* objCmd,
    nCmd* cmd, /*nArg* arg,*/
    nString& param, nString& rollout, nString& setupbody)
{
    n_assert(cmd);
     
    nString strAttrName = cmd->GetProto()->GetName();
    nString strAttrUIName = strAttrName + "UI";

    vector3 val;
    val.x = cmd->In()->GetF();
    val.y = cmd->In()->GetF();
    val.z = cmd->In()->GetF();

    float a = cmd->In()->GetF();
    
    nString rgb, alpha;

    rgb.AppendFloat(val.x * 255.0f);    rgb += " ";
    rgb.AppendFloat(val.y * 255.0f);    rgb += " ";
    rgb.AppendFloat(val.z * 255.0f);    rgb += " ";

    alpha.AppendFloat(a);

    param += strAttrName + " type:#color default:";
    param += "(color "+ rgb + ") ";
    param += " ui:" + strAttrUIName;
    param += "\n\t\t";

    param += strAttrName + "Alpha type:#float default:";
    param += alpha;

    param += " ui:";
    param += strAttrUIName + "Alpha";

    rollout += "colorpicker ";
    rollout += strAttrUIName + " \"" + strAttrName + 
        ": \" align:#left across:2 offset:[0,6]\n\t\t";

    rollout += "spinner ";
    rollout += strAttrUIName + "Alpha \"Alpha: \" ";
    rollout += "type:#float range:[0, 1, 1] scale:0.05 align:#left"
        " fieldwidth:70 height:16 offset:[0, 9]";
}

//-----------------------------------------------------------------------------
/**
    Add 'String' type of parameter to custom attribute.
*/
void AddStringParam(const nObjectAttribute::Command* objCmd,
    nCmd* cmd, nArg* arg,
    nString& param, nString& rollout, nString& setupbody)
{
    n_assert(cmd);
    n_assert(arg);

    nString strAttrName = cmd->GetProto()->GetName();
    strAttrName.AppendInt(cmd->GetInArgIndex());

    nString strAttrUIName = strAttrName + "UI";
    strAttrUIName.AppendInt(cmd->GetInArgIndex());

    // this can be null.
    const char* pcValue;
    pcValue = arg->GetS();
    if (!pcValue)
        pcValue = "(null)";

    param += "\t\t";
    param += strAttrName + " type:#string default:\"";
    param += (char*)pcValue;
    param += "\" ";
    param += "ui:" + strAttrUIName;

    rollout += "edittext ";
    rollout += strAttrUIName + " \"" + 
    strAttrName + ": \" ";
    rollout += "text:\"";
    rollout += (char*)pcValue;
    rollout += "\" align:#left width:275 offset:[0,6]";
}

//-----------------------------------------------------------------------------
/**
    Add 'Int' type of parameter to custom attribute.
*/
void AddIntParam(const nObjectAttribute::Command* objCmd,
    nCmd* cmd, nArg* arg,
    nString& param, nString& rollout, nString& setupbody)
{
    n_assert(cmd);
    n_assert(arg);
     
    nString strAttrName = cmd->GetProto()->GetName();
    strAttrName.AppendInt(cmd->GetInArgIndex());

    nString strAttrUIName = strAttrName + "UI";
    strAttrUIName.AppendInt(cmd->GetInArgIndex());

    uint uiMax = 2147483281;
    uint uiMin = 0;

    if (objCmd)
    {
        uiMin = (uint)objCmd->GetMin();
        if (uiMin > 2147483281)
            uiMin = 0;

        uiMax = (uint)objCmd->GetMax();
        if (uiMax <= uiMin || uiMax > 2147483281)
            uiMax = 2147483281;
    }

    unsigned int uiVal;
    uiVal = arg->GetI();

    if (uiVal > 2147483281 || uiVal < uiMin)
        uiVal = uiMin;
    else if (uiVal > uiMax)
        uiVal = uiMax;

    param += "\t\t";
    param += strAttrName + " type:#integer default:";
    param.AppendInt(uiVal);

    param += " ui:";
    param += strAttrUIName;

    rollout += "spinner ";
    rollout += strAttrUIName + " \"" + 
    strAttrName + ": \" ";
               
    rollout += "type:#integer ";
    rollout += "range:[";
    rollout.AppendInt(uiMin);
    rollout += " , ";
    rollout.AppendInt(uiMax);
    rollout += " , ";
    rollout.AppendInt(uiVal);
    rollout += "] ";
    rollout += " align:#left fieldwidth:70 offset:[0,6]";
}
