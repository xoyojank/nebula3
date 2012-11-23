#pragma once
//------------------------------------------------------------------------------
/**
    @class Lighting::PSSMUtil

    Helper class which compute LightProj matrices for Parallel-Split-
    Shadowmap rendering.

    (C) 2007 Radon Labs GmbH
*/
#include "math/matrix44.h"
#include "math/vector.h"
#include "math/bbox.h"
#include "util/fixedarray.h"
#include "internalgraphics/internalcameraentity.h"

// fixme: maybe put it in a more central file
#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38F
#endif
//------------------------------------------------------------------------------
namespace Lighting
{
class PSSMUtil
{
public:
    /// constructor
    PSSMUtil();
    /// set camera entity which defines the view and projection transform
    void SetCameraEntity(const Ptr<InternalGraphics::InternalCameraEntity>& camera);
    /// get camera entity
    const Ptr<InternalGraphics::InternalCameraEntity>& GetCameraEntity() const;
    /// set light direction
    void SetLightDir(const Math::vector& dir);
    /// get light direction
    const Math::vector& GetLightDir() const;

    /// compute PSSM split volumes
    void Compute();

    /// get view matrix for a view frustum split (valid after Compute)
    const Math::matrix44& GetSplitLightTransform(IndexT splitIndex) const;
    /// get projection transform for a view frustum split (valid after Compute)
    const Math::matrix44& GetSplitProjTransform(IndexT splitIndex) const;
    /// get light projection transform for given frustum split (valid after Compute())
    const Math::matrix44& GetSplitLightProjTransform(IndexT splitIndex) const;

    /// get raw pointer to split distances
    const float* GetSplitDistances() const;
    /// get raw pointer to LightProjTransforms
    const Math::matrix44* GetSplitLightProjTransforms() const;

    /// number of view volume splits
    static const SizeT NumSplits = 4;

private:
    /// calculate split distances in projection space
    void CalculateSplitDistances();
    /// calculate corner points of a frustum split
    void CalculateFrustumPoints(IndexT splitIndex);
    /// calculate "best" light projection matrices for current frustum corner points
    void CalculateTransforms(IndexT splitIndex);

    static const SizeT NumCorners = 8;
    float maxShadowDistance;
    Ptr<InternalGraphics::InternalCameraEntity> cameraEntity;
    Math::vector lightDir;

    float splitDistances[NumSplits + 1];
    Math::float4 frustumCenter;
    Math::float4 frustumCorners[NumCorners];
    Math::matrix44 splitLightTransforms[NumSplits];
    Math::matrix44 splitProjTransforms[NumSplits];
    Math::matrix44 splitLightProjTransforms[NumSplits];
};

//------------------------------------------------------------------------------
/**
*/
inline void
PSSMUtil::SetCameraEntity(const Ptr<InternalGraphics::InternalCameraEntity>& camera)
{
    this->cameraEntity = camera;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<InternalGraphics::InternalCameraEntity>&
PSSMUtil::GetCameraEntity() const
{
    return this->cameraEntity;
}

//------------------------------------------------------------------------------
/**
*/
inline void
PSSMUtil::SetLightDir(const Math::vector& v)
{
    this->lightDir = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
PSSMUtil::GetLightDir() const
{
    return this->lightDir;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
PSSMUtil::GetSplitLightProjTransform(IndexT i) const
{
    n_assert(i < NumSplits);
    return this->splitLightProjTransforms[i];
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
PSSMUtil::GetSplitLightTransform(IndexT i) const
{
    n_assert(i < NumSplits);
    return this->splitLightTransforms[i];
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
PSSMUtil::GetSplitProjTransform(IndexT i) const
{
    n_assert(i < NumSplits);
    return this->splitProjTransforms[i];
}

//------------------------------------------------------------------------------
/**
*/
inline const float*
PSSMUtil::GetSplitDistances() const
{
    return this->splitDistances;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44*
PSSMUtil::GetSplitLightProjTransforms() const
{
    return this->splitLightProjTransforms;
}

} // namespace Lighting
//------------------------------------------------------------------------------