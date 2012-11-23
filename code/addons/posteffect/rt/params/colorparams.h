#pragma once
//------------------------------------------------------------------------------
/**
    @class PostEffect::ColorParams
    
    Holds all the relevant post effects COLOR parameters
    
    (C) 2006 Radon Labs GmbH
*/
#include "math/float4.h"
#include "parambase.h"

//------------------------------------------------------------------------------
namespace PostEffect
{
class ColorParams : public ParamBase
{
    __DeclareClass(ColorParams);
public:
    /// constructor
    ColorParams();
    /// constructor
    virtual ~ColorParams();

    /// copy operator
    virtual void operator=(const Ptr<ParamBase>& rhs);

    /// blend towards a target paramset
    virtual void BlendTo(const Ptr<ParamBase>& p, float l);

    /// comapre
    virtual bool Equals(const Ptr<ParamBase>& v2, float threshold) const;

    /// set color saturation
    void SetColorSaturation(float f);
    /// get color saturation
    float GetColorSaturation() const;
    /// set color balance
    void SetColorBalance(const Math::float4& v);
    /// get color balance
    const Math::float4& GetColorBalance() const;

private:
    float colorSaturation;
    Math::float4 colorBalance;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
ColorParams::SetColorSaturation(float f)
{
    this->colorSaturation = f;
}

//------------------------------------------------------------------------------
/**
*/
inline
float 
ColorParams::GetColorSaturation() const
{
    return this->colorSaturation;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
ColorParams::SetColorBalance(const Math::float4& v)
{
    this->colorBalance = v;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float4& 
ColorParams::GetColorBalance() const
{
    return this->colorBalance;
}
}
//------------------------------------------------------------------------------
