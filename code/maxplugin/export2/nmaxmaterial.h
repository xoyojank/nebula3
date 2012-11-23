//-----------------------------------------------------------------------------
//  nmaxmaterial.h
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXMATERIAL_H
#define N_MAXMATERIAL_H

//-----------------------------------------------------------------------------
/**
    @class nMaxMaterial
    @ingroup NebulaMaxExport2Contrib

    @brief Class for exporting 3DS Max materials.

    The followings are supported:
      -# Standard material.
      -# Muliti-sub material.
      -# Nebula2 custom mateiral which is shader censtric and more Nebula
         friendly.
    
*/
//#include "mathlib/vector.h"
//#include "gfx2/nshaderstate.h"

//class nString;
//class nShapeNode;
#include <math/vector.h>
#include <util/string.h>

//-----------------------------------------------------------------------------
//  Class_ID for Nebula2 custom material.
//  (This should be match the class_ID of the scripted plugin script.
//
#define N2MTL_CLASS_ID0 0x6b5ae640
#define N2MTL_CLASS_ID1 0x319f53b8

#define NEBULAMTL_CLASSID Class_ID(N2MTL_CLASS_ID0, N2MTL_CLASS_ID1)

//-----------------------------------------------------------------------------
class nMaxMaterial
{
public:
    enum Type 
    {
        Standard,
        MultiSub,
        Shell,
        NebulaCustom,
        Unknown,
    };

    /// Constructor.
    nMaxMaterial();
    /// Destructor.
    virtual ~nMaxMaterial();

    void Export(Mtl* mtl, nShapeNode* shapeNode, int matID);

    static Type GetType(Mtl* material);
    static Texmap* GetSubTexmap(Mtl* mtl, int subMapIdx);
    static bool HasMultiTexture(StdMat2* mat);

    static bool IsClassID(Mtl* mtl, ulong classID);

    int GetNumMaps(StdMat2 *mtl);

protected:
    void CreateDefaultMaterial(nShapeNode* shapeNode);

#if (MAX_RELEASE >= 6000)
    void GetDirectXMaterial(Mtl* mtl, IDxMaterial* dxMat, nShapeNode* shapeNode);
#endif
    void GetStandardMaterial(Mtl* mtl, nShapeNode* shapeNode);
    
    void GetNebulaMaterial(Mtl* mtl, nShapeNode* shapeNode);

    // @name Functions for nparticleshapenode2.
    // @{
    ///
    void GetParticle2Material(Mtl* mtl, nShapeNode* shapeNode);
    // @}

    // @name Functions for vector4 type material animation
    // @{
    ///
    void ExportShaderAnimations(Mtl* mtl, nShapeNode* shapeNode);
    ///
    void CreateShaderAnimator(nShapeNode* shapeNode,
                              nShaderState::Param vectorParameter, 
                              Control* ctrlColor, Control* ctrlAlpha);
    // @}

    ///
    bool SetAlphaParam(StdMat2* stdMat, nShapeNode* shapeNode);
    ///
    void SetStandardNebulaShader(nShapeNode* shapeNode, bool hasOpacityMap = false);

    ///
    nShaderState::Param GetShaderParam(const char* param);
    ///
    void ClampColor(vector4 &val);
    ///
    bool IsPredefinedCA(TCHAR* caName);

    enum {NUM_PREDEFINED_CA  = 3};
    /// the followings are 3dmax predefined custom attribute.
    static const char* predefinedCA[NUM_PREDEFINED_CA];

};
//-----------------------------------------------------------------------------
/**
    Check the given class ID is matched to the gien material's class ID.

    @param mtl pointer to the Mtl which to be checked.
    @param classID  class ID to check.
*/
inline
bool nMaxMaterial::IsClassID(Mtl* mtl, ulong classID)
{
    if (mtl && mtl->ClassID() == Class_ID(classID, 0))
        return true;

    return false;
}

//-----------------------------------------------------------------------------
/**
Nebula uses color value for [0, 1] but 3dsmax uses that for [0, 255]. 
So it is needed to clamp
*/
inline
void nMaxMaterial::ClampColor(vector4 &val)
{
    if (val.x > 1.0f)
        val.x = 1.0f;
    if (val.y > 1.0f)
        val.y = 1.0f;
    if (val.z > 1.0f)
        val.z = 1.0f;
    if (val.w > 1.0f)
        val.w = 1.0f;
}

//-----------------------------------------------------------------------------
/**
    Check the given name of the custom attribute is predefined standard 
    custom attribute of 3dsmax or not.
    
    @param caName custom attribute name.
*/
inline
bool nMaxMaterial::IsPredefinedCA(TCHAR* caName)
{
    for (uint i=0; i<NUM_PREDEFINED_CA; i++)
    {
        if (n_stricmp (caName, predefinedCA[i]) == 0)
            return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
/**
    Retrieve default texture filename from shader.xml.

    @param shader shader alias which is alread assigned to the given node. i.g. "static", "skinned" etc.
    @param param string of shader parameter such as 'nShaderState::DiffMap0' and so on. 
    @param outvalue texture filename retrieved from database which will be used for default texture.

    @return true, if the default value is found ohterwise false.
*/
bool GetDefaultValueFromDataBase(const nString &shader, const nString &param, nString &outvalue);

//-----------------------------------------------------------------------------
#endif
