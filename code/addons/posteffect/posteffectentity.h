#pragma once
//------------------------------------------------------------------------------
/**
    @class PostEffect::PostEffectsEntity
    
    Implements a post effect entity which influences global post effect
    parameters when the player enters it.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "posteffect/rt/params/colorparams.h"
#include "posteffect/rt/params/depthoffieldparams.h"
#include "posteffect/rt/params/fogparams.h"
#include "posteffect/rt/params/hdrparams.h"
#include "posteffect/rt/params/lightparams.h"
#include "posteffect/rt/params/skyparams.h"
#include "math/point.h"

//------------------------------------------------------------------------------
namespace PostEffect
{
class PostEffectEntity : public Core::RefCounted
{
    __DeclareClass(PostEffectEntity);
public:
    /// shape type
    enum ShapeType
    {
        Sphere,
        Box,
    };

    /// all post effect parameters bundled in a set
    struct ParamSet
    {
        Ptr<ColorParams> color;
        Ptr<DepthOfFieldParams> dof;
        Ptr<FogParams> fog;
        Ptr<HdrParams> hdr;
        Ptr<LightParams> light;
        Ptr<SkyParams> sky;
    };

    /// constructor
    PostEffectEntity();
    /// destructor
    virtual ~PostEffectEntity();

    /// return true if the provided position is "inside"
    bool IsInside(const Math::point& pos) const;

    /// called when attached to server
    virtual void OnAttach();
    /// called when removed from server
    virtual void OnRemove();
    /// return true if currently attached
    bool IsAttached() const;

    /// set to true if this is the global default entity
    void SetDefaultEntity(bool b);
    /// return true if this is the default entity
    bool IsDefaultEntity() const; 

    /// set to true if this is enabled
    void SetEnabled(bool b);
    /// return true if this is enabled
    bool IsEnabled() const;

    /// set shape type
    void SetShapeType(ShapeType s);
    /// get shape type
    ShapeType GetShapeType() const;

    /// set transform (scale defines size)
    void SetTransform(const Math::matrix44& m);
    /// get transform
    const Math::matrix44& GetTransform() const;

    /// set fade time
    void SetBlendTime(float f);
    /// get fade time
    float GetBlendTime() const;

    /// set the priority
    void SetPriority(int p);
    /// get priority
    int GetPriority() const;

    /// read/write access to post effect parameters
    ParamSet& Params();

    /// convert string to shape type
    static ShapeType StringToShapeType(const Util::String& s);

private:
    int priority;
    bool isAttached;
    bool isEnabled;
    bool defaultEntity;
    ShapeType shapeType;
    Math::matrix44 transform;
    Math::matrix44 invTransform;
    float blendTime;
    ParamSet params;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
PostEffectEntity::SetPriority(int p)
{
    this->priority = p;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
PostEffectEntity::GetPriority() const
{
    return this->priority;
}

//------------------------------------------------------------------------------
/**
*/
inline
PostEffectEntity::ShapeType
PostEffectEntity::StringToShapeType(const Util::String& s)
{
    if (s == "Sphere") return Sphere;
    else if (s == "Box") return Box;
    else
    {
        n_error("PostEffect::Entity::StringToShapeType(): invalid shape type '%s'!", s.AsCharPtr());
        return Box;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
PostEffectEntity::IsAttached() const
{
    return this->isAttached;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
PostEffectEntity::SetDefaultEntity(bool b)
{
    this->defaultEntity = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
PostEffectEntity::IsEnabled() const
{
    return this->isEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
PostEffectEntity::SetEnabled(bool b)
{
    this->isEnabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
PostEffectEntity::IsDefaultEntity() const
{
    return this->defaultEntity;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
PostEffectEntity::SetShapeType(ShapeType s)
{
    this->shapeType = s;
}

//------------------------------------------------------------------------------
/**
*/
inline
PostEffectEntity::ShapeType
PostEffectEntity::GetShapeType() const
{
    return this->shapeType;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
PostEffectEntity::SetTransform(const Math::matrix44& m)
{
    this->transform = m;
    this->invTransform = Math::matrix44::inverse(m);
    this->params.light->SetLightTransform(m);
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::matrix44&
PostEffectEntity::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
PostEffectEntity::SetBlendTime(float f)
{
    this->blendTime = f;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
PostEffectEntity::GetBlendTime() const
{
    return this->blendTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
PostEffectEntity::ParamSet&
PostEffectEntity::Params()
{
    return this->params;
}

} // namespace PostEffects
//------------------------------------------------------------------------------

