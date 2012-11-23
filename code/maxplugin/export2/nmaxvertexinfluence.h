//---------------------------------------------------------------------------
//  nmaxvertexinfluence.h
//
//  (c)2004 Kim, Hyoun Woo
//---------------------------------------------------------------------------
#ifndef N_MAXVERTEXINFLUENCE_H
#define N_MAXVERTEXINFLUENCE_H
//---------------------------------------------------------------------------
/**
    @class nMaxVertexInfluence
    @ingroup NebulaMaxExport2Contrib

    @brief

    This code was taken from Dmitry Peter's TND2Exporter.
*/
//#include "mathlib/vector.h"
//#include "util/narray.h"
#include "util/array.h"
#include "math/vector.h"

//---------------------------------------------------------------------------
class nMaxVertexInfluence
{
public:
    struct BoneInfluence
    {
        int bone;
        float weight;
    };

    nMaxVertexInfluence();
    virtual ~nMaxVertexInfluence();

    int GetNumInfuences();
	Math::vector GetWeightsVector();
    Math::vector GetIndicesVector();
    void AddBoneInfluence(int iBone, float fWeight);
    void AdjustBonesInfluence(float fWeightTreshold);

private:
	Util::Array<BoneInfluence> boneArray;
};
//---------------------------------------------------------------------------
#endif
