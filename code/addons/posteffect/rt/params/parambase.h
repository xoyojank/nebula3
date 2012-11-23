#pragma once
//------------------------------------------------------------------------------
/**
    @class PostEffect::ParamBase
    
    This is the parent class, for all posteffect parameter settings.
    
    (C) 2006 Radon Labs GmbH
*/
#include "timing/time.h"
#include "core/refcounted.h"

//------------------------------------------------------------------------------
namespace PostEffect
{
class ParamBase : public Core::RefCounted
{
    __DeclareClass(ParamBase);
public:
    /// constructor
    ParamBase();
    /// deconstructor
    virtual ~ParamBase();

    /// copy operator
    virtual void operator=(const Ptr<ParamBase>& rhs);

    /// blend towards a target paramset
    virtual void BlendTo(const Ptr<ParamBase>& p, float l);

    /// comapre
    virtual bool Equals(const Ptr<ParamBase>& v2, float threshold) const;
};

}
//------------------------------------------------------------------------------
