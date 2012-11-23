//-----------------------------------------------------------------------------
//  nmaxcustattrib.cc
//
//  (c)2005 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "stdneb.h"
#include "export2/nmax.h"

#include "export2/nmaxcustattrib.h"
//#include "kernel/ntypes.h"

//-----------------------------------------------------------------------------
/**
    Check the given string is valid.
*/
static
bool IsValidName(TCHAR* str)
{
    int iLen = strlen(str);
    if ( iLen == 0) 
        return false;

    for (int ui = 0; ui < iLen; ui++)
    {
        if (str[ui] >= 'a' && str[ui] <= 'z')
            continue;
        else 
        if (str[ui] >= 'A' && str[ui] <= 'Z')
            continue;
        else 
        if (str[ui] >= '1' && str[ui] <= '9')
            continue;
        else 
        if (str[ui] == '0')
            continue;
        else 
        if (str[ui] == ' ')
            continue;
        else 
        if (str[ui] == '\t')
            continue;
        else 
        if (str[ui] == '_')
            continue;
        else
        if (str[ui] == '-')
            continue;
        else
            return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
/**
*/
nMaxCustAttrib::nMaxCustAttrib()
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxCustAttrib::~nMaxCustAttrib()
{
}

//-----------------------------------------------------------------------------
/**
    Convert 3dsmax custom attributes to xml data.

    @param obj pointer to Object
    @param xmlDoc
*/
bool nMaxCustAttrib::Convert(Animatable* obj, TiXmlDocument& xmlDoc)
{
    if (NULL == obj)
        return false;

    // retrieves custom attribute's container.
    ICustAttribContainer* caContainer = obj->GetCustAttribContainer();
    if (caContainer)
    {
        int numAttribs = caContainer->GetNumCustAttribs();

        CustAttrib* custAttr = 0;

        for (int i=0; i<numAttribs; i++)
        {
            custAttr = (CustAttrib*)caContainer->GetCustAttrib(i);

            if (custAttr)
            {
                // get the number of the param blocks.
                int numParamBlocks = custAttr->NumParamBlocks();

                for (int j=0; j<numParamBlocks; j++)
                {
                    // get the pointer of the i'th param block.
                    IParamBlock2* paramBlock = custAttr->GetParamBlock(j);

                    // get the number of the params.
                    int numParams = paramBlock->NumParams();

                    int paramType;
                    for (int k=0; k<numParams; k++)
                    {
                        paramType = paramBlock->GetParameterType(k);

                        switch(paramType)
                        {
                        case TYPE_STRING:
                            this->StringToXml(paramBlock, k, xmlDoc);
                            break;

                        case TYPE_FLOAT:
                            this->FloatToXml(paramBlock, k, xmlDoc);
                            break;

                        case TYPE_INT:
                            this->IntToXml(paramBlock, k, xmlDoc);
                            break;

                        case TYPE_BOOL:
                            this->BoolToXml(paramBlock, k, xmlDoc);
                            break;

                        case TYPE_RGBA:
                            this->RgbaToXml(paramBlock, k, xmlDoc);
                            break;

                        case TYPE_POINT3:
                            this->Point3ToXml(paramBlock, k, xmlDoc);
                            break;

                        case TYPE_FLOAT_TAB:
                            this->FloatTabToXml(paramBlock, k, xmlDoc);
                            break;

                        default:
                            this->UnknownToXml(paramBlock, k, xmlDoc);
                            break;
                        }
                    }// end of for each parameters.
                }// end of for each paramblocks.
            }
        }// end of for each attributes.
    }
    else
    {
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
/**
    Retrieves 'string' type of the param and link it to the given XML document.

    -28-Aug-06  kims  Changed to assign "<<NULL>>" if the string value has no data.
*/
void nMaxCustAttrib::StringToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc)
{
    // get paramblock description 
    ParamBlockDesc2* pblockDesc = pBlock->GetDesc();
    if (NULL == pblockDesc)
        return;

    ParamID paramID = pblockDesc->IndextoID(index);
    ParamDef& paramDef = pblockDesc->GetParamDef(paramID);

    // get the index'th value.
    BOOL result;
    //TCHAR* strValue = NULL;
	int fValue;
    Interval interval;
    result = pBlock->GetValue(index, 0, fValue, interval);

    if (result)
    {
        // get fixed internal name.
        TCHAR* name = paramDef.int_name; 
        if (!IsValidName(name))
        {
            // if the internal name is not valid, get local name of the param.
            name = pBlock->GetLocalName(index);
        }

        // get the paramblock's name.
        TCHAR* pblockName = pBlock->GetLocalName();
        TiXmlHandle xmlHandle(&xmlDoc);
        TiXmlElement* elmParamBlock = xmlHandle.FirstChild(pblockName).Element();
        if (!elmParamBlock)
        {
            // if there's no element which name is the paramblock's name,
            // we create a new one.
            elmParamBlock = n_new(TiXmlElement(pblockName));
            xmlDoc.LinkEndChild(elmParamBlock);
        }

        // create a new xml element and make it to the child element of the paramblock's element.
        TiXmlElement* elm = n_new(TiXmlElement(name));

        elm->SetAttribute("type", "string");
        elm->SetAttribute("count", 1);

        TiXmlElement* elmVal;

        elmVal = n_new(TiXmlElement(""));

        // HACK: passing null-pointer causes internal error.
        //       So, just assigned any default value not to causes that.
        //if (!strValue)
        //    strValue = "<<NULL>>";

        elmVal->SetAttribute("value", fValue);

        elm->LinkEndChild(elmVal);

        elmParamBlock->LinkEndChild(elm);
    }
}

//-----------------------------------------------------------------------------
/**
    Retrieves 'bool' type of the param and link it to the given XML document.

    @param pBlock
    @param index '0' based index of the param.
    @param xmlDoc
*/
void nMaxCustAttrib::BoolToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc)
{
    ParamBlockDesc2* pblockDesc = pBlock->GetDesc();
    if (NULL == pblockDesc)
        return;

    ParamID paramID = pblockDesc->IndextoID(index);
    ParamDef& paramDef = pblockDesc->GetParamDef(paramID);

    BOOL result;
    int intValue;
    Interval interval;
    result = pBlock->GetValue(index, 0, intValue, interval);

    if (result)
    {
        TCHAR* name = paramDef.int_name;
        if (!IsValidName(name))
        {
            // if the internal name is not valid, get local name of the param.
            name = pBlock->GetLocalName(index);
        }

        // get the paramblock's name.
        TCHAR* pblockName = pBlock->GetLocalName();
        TiXmlHandle xmlHandle(&xmlDoc);
        TiXmlElement* elmParamBlock = xmlHandle.FirstChild(pblockName).Element();
        if (!elmParamBlock)
        {
            // if there's no element which name is the paramblock's name,
            // we create a new one.
            elmParamBlock = n_new(TiXmlElement(pblockName));
            xmlDoc.LinkEndChild(elmParamBlock);
        }

        // create a new xml element and make it to the child element of the paramblock's element.
        TiXmlElement* elm = n_new(TiXmlElement(name));

        elm->SetAttribute("type", "bool");
        elm->SetAttribute("count", 1);

        TiXmlElement* elmVal;

        elmVal = n_new(TiXmlElement(""));
        elmVal->SetAttribute("value", intValue);

        elm->LinkEndChild(elmVal);

        elmParamBlock->LinkEndChild(elm);
    }
}

//-----------------------------------------------------------------------------
/**
    Retrieves 'float' type of the param and link it to the given XML document.
*/
void nMaxCustAttrib::FloatToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc)
{
    ParamBlockDesc2* pblockDesc = pBlock->GetDesc();
    if (NULL == pblockDesc)
        return;

    ParamID paramID = pblockDesc->IndextoID(index);
    ParamDef& paramDef = pblockDesc->GetParamDef(paramID);

    BOOL result;
    int floatValue;
    Interval interval;
    result = pBlock->GetValue(index, 0, floatValue, interval);

    if (result)
    {
        TCHAR* name = paramDef.int_name;
        if (!IsValidName(name))
        {
            // if the internal name is not valid, get local name of the param.
            name = pBlock->GetLocalName(index);
        }

        // get the paramblock's name.
        TCHAR* pblockName = pBlock->GetLocalName();
        TiXmlHandle xmlHandle(&xmlDoc);
        TiXmlElement* elmParamBlock = xmlHandle.FirstChild(pblockName).Element();
        if (!elmParamBlock)
        {
            // if there's no element which name is the paramblock's name,
            // we create a new one.
            elmParamBlock = n_new(TiXmlElement(pblockName));
            xmlDoc.LinkEndChild(elmParamBlock);
        }

        // create a new xml element and make it to the child element of the paramblock's element.
        TiXmlElement* elm = n_new(TiXmlElement(name));

        elm->SetAttribute("type", "float");
        elm->SetAttribute("count", 1);

        TiXmlElement* elmVal;

        elmVal = n_new(TiXmlElement(""));
        elmVal->SetAttribute("value", floatValue);

        elm->LinkEndChild(elmVal);

        elmParamBlock->LinkEndChild(elm);
    }
}

//-----------------------------------------------------------------------------
/**
    Retrieves 'integer' type of the param and link it to the given XML document.
*/
void nMaxCustAttrib::IntToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc)
{
    ParamBlockDesc2* pblockDesc = pBlock->GetDesc();
    if (NULL == pblockDesc)
        return;

    ParamID paramID = pblockDesc->IndextoID(index);
    ParamDef& paramDef = pblockDesc->GetParamDef(paramID);

    BOOL result;
    int intValue;
    Interval interval;
    result = pBlock->GetValue(index, 0, intValue, interval);

    if (result)
    {
        TCHAR* name = paramDef.int_name;
        if (!IsValidName(name))
        {
            // if the internal name is not valid, get local name of the param.
            name = pBlock->GetLocalName(index);
        }

        // get the paramblock's name.
        TCHAR* pblockName = pBlock->GetLocalName();
        TiXmlHandle xmlHandle(&xmlDoc);
        TiXmlElement* elmParamBlock = xmlHandle.FirstChild(pblockName).Element();
        if (!elmParamBlock)
        {
            // if there's no element which name is the paramblock's name,
            // we create a new one.
            elmParamBlock = n_new(TiXmlElement(pblockName));
            xmlDoc.LinkEndChild(elmParamBlock);
        }

        // create a new xml element and make it to the child element of the paramblock's element.
        TiXmlElement* elm = n_new(TiXmlElement(name));

        elm->SetAttribute("type", "integer");
        elm->SetAttribute("count", 1);

        TiXmlElement* elmVal;

        elmVal = n_new(TiXmlElement(""));
        elmVal->SetAttribute("value", intValue);

        elm->LinkEndChild(elmVal);

        elmParamBlock->LinkEndChild(elm);
    }
}

//-----------------------------------------------------------------------------
/**
    Retrieves 'string' type of the param and link it to the given XML document.
*/
void nMaxCustAttrib::RgbaToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc)
{

}

//-----------------------------------------------------------------------------
/**
    Retrieves 'point3' type of the param and link it to the given XML document.
*/
void nMaxCustAttrib::Point3ToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc)
{
    ParamBlockDesc2* pblockDesc = pBlock->GetDesc();
    if (NULL == pblockDesc)
        return;

    ParamID paramID = pblockDesc->IndextoID(index);
    ParamDef& paramDef = pblockDesc->GetParamDef(paramID);

    BOOL result;
    Point3 point3Value;
    Interval interval;
    result = pBlock->GetValue(index, 0, point3Value, interval);

    if (result)
    {
        TCHAR* name = paramDef.int_name;
        if (!IsValidName(name))
        {
            // if the internal name is not valid, get local name of the param.
            name = pBlock->GetLocalName(index);
        }

        // get the paramblock's name.
        TCHAR* pblockName = pBlock->GetLocalName();
        TiXmlHandle xmlHandle(&xmlDoc);
        TiXmlElement* elmParamBlock = xmlHandle.FirstChild(pblockName).Element();
        if (!elmParamBlock)
        {
            // if there's no element which name is the paramblock's name,
            // we create a new one.
            elmParamBlock = n_new(TiXmlElement(pblockName));
            xmlDoc.LinkEndChild(elmParamBlock);
        }

        // create a new xml element and make it to the child element of the paramblock's element.
        TiXmlElement* elm = n_new(TiXmlElement(name));

        elm->SetAttribute("type", "point3");
        elm->SetAttribute("count", 3);

        TiXmlElement* elmVal;

        int p[3];
        p[0] = (int)point3Value.x; p[1] = (int)point3Value.y; p[2] = (int)point3Value.z; 
        for (int i=0; i<3; i++)
        {
            elmVal = n_new(TiXmlElement(""));
            elmVal->SetAttribute("value", p[i]);

            elm->LinkEndChild(elmVal);
        }

        elmParamBlock->LinkEndChild(elm);
    }
}

//-----------------------------------------------------------------------------
/**

*/
void nMaxCustAttrib::FloatTabToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc)
{
    ParamBlockDesc2* pblockDesc = pBlock->GetDesc();
    if (NULL == pblockDesc)
        return;

    ParamID paramID = pblockDesc->IndextoID(index);
    ParamDef& paramDef = pblockDesc->GetParamDef(paramID);

    int count = pBlock->Count(pblockDesc->IndextoID(index));
    if (count <= 0)
        return;

    // retrieves float_tab values.
    Interval interval;
    int* val = n_new(int[count]);

    BOOL result;
    for (int i=0; i<count; i++)
    {
        result &= pBlock->GetValue(index, 0, val[i], interval, i);
    }

    if (result)
    {
        TCHAR* name = paramDef.int_name;
        if (!IsValidName(name))
        {
            // if the internal name is not valid, get local name of the param.
            name = pBlock->GetLocalName(index);
        }

        TCHAR* pblockName = pBlock->GetLocalName();
        TiXmlHandle xmlHandle(&xmlDoc);
        TiXmlElement* elmParamBlock = xmlHandle.FirstChild(pblockName).Element();
        if (!elmParamBlock)
        {
            // if there's no element which name is the paramblock's name,
            // we create a new one.
            elmParamBlock = n_new(TiXmlElement(pblockName));
            xmlDoc.LinkEndChild(elmParamBlock);
        }

        // create a new xml element and make it to the child element of the paramblock's element.
        TiXmlElement* elm = n_new(TiXmlElement(name));

        elm->SetAttribute("type", "floattab");
        elm->SetAttribute("count", count);

        TiXmlElement* elmVal;

        for (int i=0; i<count; i++)
        {
            elmVal = n_new(TiXmlElement(""));
            elmVal->SetAttribute("value", val[i]);

            elm->LinkEndChild(elmVal);
        }

        elmParamBlock->LinkEndChild(elm);
    }
}

//-----------------------------------------------------------------------------
/**
    
*/
void nMaxCustAttrib::UnknownToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc)
{

}

//-----------------------------------------------------------------------------
/**
    Retrieves string from the given parameter

    @param pBlock parameter block
    @param index i'th parameter of the parameter block
    @param value retrieved value

    @return return true if we success to retrieve the value.
*/
bool nMaxCustAttrib::GetString(IParamBlock2* pBlock, int index, TCHAR * &value)
{
    return true;
}

//-----------------------------------------------------------------------------
/**
    Retrieves bool value from the given parameter

    @param pBlock parameter block
    @param index i'th parameter of the parameter block
    @param value retrieved value

    @return return true if we success to retrieve the value.
*/
bool nMaxCustAttrib::GetBool(IParamBlock2* pBlock, int index, bool &value)
{
    return true;
}

//-----------------------------------------------------------------------------
/**
    Retrieves int value from the given parameter

    @param pBlock parameter block
    @param index i'th parameter of the parameter block
    @param value retrieved value

    @return return true if we success to retrieve the value.
*/
bool nMaxCustAttrib::GetInt(IParamBlock2* pBlock, int index, int &value)
{
    return true;
}

//-----------------------------------------------------------------------------
/**
    Retrieves 'float' value from the given parameter

    @param pBlock parameter block
    @param index i'th parameter of the parameter block
    @param value retrieved value

    @return return true if we success to retrieve the value.
*/
bool nMaxCustAttrib::GetFloat(IParamBlock2* pBlock, int index, float &value)
{
    return true;
}

//-----------------------------------------------------------------------------
/**
    Retrieves RGBA color value from the given parameter

    @param pBlock parameter block
    @param index i'th parameter of the parameter block
    @param value retrieved value

    @return return true if we success to retrieve the value.
*/
bool nMaxCustAttrib::GetRGBA(IParamBlock2* pBlock, int index, Color &clr)
{
    return true;
}

//-----------------------------------------------------------------------------
/**
    Retrieves point3 value from the given parameter

    @param pBlock parameter block
    @param index i'th parameter of the parameter block
    @param value retrieved value

    @return return true if we success to retrieve the value.
*/
bool nMaxCustAttrib::GetPoint3(IParamBlock2* pBlock, int index, Point3 &value)
{
    return true;
}

//-----------------------------------------------------------------------------
/**
    Retrieves float tab value (array of floats) from the given parameter

    @param pBlock parameter block
    @param index i'th parameter of the parameter block
    @param value retrieved value
    @param count array size

    @return return true if we success to retrieve the value.
*/
bool nMaxCustAttrib::GetFloatTab(IParamBlock2* pBlock, int index, float* &value, int &count)
{
    return true;
}
