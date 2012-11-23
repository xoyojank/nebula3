//-----------------------------------------------------------------------------
//  nmaxcustattrib.h
//
//  (c)2005 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXCUSTATTRIB_H
#define N_MAXCUSTATTRIB_H
//-----------------------------------------------------------------------------
/**
    @class nMaxCustAttrib
    @ingroup NebulaMaxExport2Contrib

    @brief The class for handling 3dsmax custom attribute.

    @code
    TiXmlDocument xmlDoc;
    ...
    nMaxCustAttrib custAttrib;
    custAttrib.Convert(obj, xmlDoc);
    ...
    xmlDoc.SaveFile("c:\\nebula2\\custattr.xml");
    @endcode

    The fallowing sample max scripts shows how parameters block converted to
    xml data.

    @verbatim
    nMeshOptionsCA = attributes "nMeshOptions"
    (
        -- we have two paramblocks

        -- first paramblock
        parameters meshTypes rollout:rOptions
        (
            -- first params in the paramblock
            meshTypes type:#integer default:1 align:#left ui:rdoMeshTypes
        )
        -- second paramblock
        parameters vertexOptions rollout:rOptions
        (
            -- we have four params in the paramblock.

            normal  type:#boolean default:true animatable:false ui:chkNormal
            color   type:#boolean default:true animatable:false ui:chkColor
            uvs     type:#boolean default:true animatable:false ui:chkUvs
            tangent type:#boolean default:false animatable:false ui:chkTangent
        )

        rollout rOptions "Nebula2 Mesh Options"
        (
            ...
        )
    )
    @endverbatim

    Each parameters block is converted to separated xml element and the name of
    parameters block is converted to the first elements name of xml data.
    The parameters of parameters block is converted to the child elements in the
    xml data.

    @verbatim
    <MeshTypes>
        <meshtype type="integer" count="1">
            < value="1" />
        </meshtype>
        <attachednode type="bool" count="1">
            < value="0" />
        </attachednode>
    </MeshTypes>
    <VertexOptions>
        <normal type="bool" count="1">
            < value="1" />
        </normal>
        <color type="bool" count="1">
            < value="1" />
        </color>
        <uvs type="bool" count="1">
            < value="1" />
        </uvs>
        <tangent type="bool" count="1">
            < value="0" />
        </tangent>
    </VertexOptions>
    @endverbatim

*/
#include "tinyxml/tinyxml.h"

//-----------------------------------------------------------------------------
class nMaxCustAttrib
{
public:
    nMaxCustAttrib();
    virtual ~nMaxCustAttrib();

    bool Convert(Animatable* obj, TiXmlDocument& xmlDoc);

    bool GetString(IParamBlock2* pBlock, int index, TCHAR * &value);
    bool GetBool(IParamBlock2* pBlock, int index, bool &value);
    bool GetInt(IParamBlock2* pBlock, int index, int &value);
    bool GetFloat(IParamBlock2* pBlock, int index, float &value);
    bool GetRGBA(IParamBlock2* pBlock, int index, Color &clr);
    bool GetPoint3(IParamBlock2* pBlock, int index, Point3 &value);
    bool GetFloatTab(IParamBlock2* pBlock, int index, float* &value, int &count);

protected:
    void StringToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc);
    void BoolToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc);
    void FloatToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc);
    void IntToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc);
    void RgbaToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc);
    void Point3ToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc);
    void FloatTabToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc);
    void UnknownToXml(IParamBlock2* pBlock, int index, TiXmlDocument& xmlDoc);

};
//-----------------------------------------------------------------------------
#endif
