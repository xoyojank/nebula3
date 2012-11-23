#pragma once
//------------------------------------------------------------------------------
/**
    @class Particles::EmitterAttrs
    
    A container for particle emitter attributes.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#include "mayacore/envelopecurve.h"

//------------------------------------------------------------------------------
namespace Particles
{
class EmitterAttrs
{
public:
    /// scalar attributes
    enum FloatAttr
    {
        EmissionDuration = 0,
        ActivityDistance,
        StartRotationMin,
        StartRotationMax,
        Gravity,
        ParticleStretch,
        VelocityRandomize,
        RotationRandomize,
        SizeRandomize,
        PrecalcTime,
        StartDelay,
        TextureTile,

        NumFloatAttrs,
    };
    
    /// boolean attributes
    enum BoolAttr
    {
        Looping = 0,
        RandomizeRotation,
        StretchToStart,
        RenderOldestFirst,
        ViewAngleFade,
        Billboard,

        NumBoolAttrs,
    };

    /// integer attributes
    enum IntAttr
    {
        StretchDetail = 0,

        NumIntAttrs,
    };
    
    /// scalar envelope attributes
    enum EnvelopeAttr
    {
        Red = 0,
        Green,
        Blue,
        Alpha,
        EmissionFrequency,
        LifeTime,
        StartVelocity,
        RotationVelocity,
        Size,
        SpreadMin,
        SpreadMax,
        AirResistance,
        VelocityFactor,
        Mass,
        TimeManipulator,

        Alignment0,             // pad value to align num curves to 16
        
        NumEnvelopeAttrs,
    };

    /// constructor
    EmitterAttrs();
    /// set float attribute
    void SetFloat(FloatAttr key, float value);
    /// get float attribute
    float GetFloat(FloatAttr key) const;
    /// set bool attribute
    void SetBool(BoolAttr key, bool value);
    /// get bool attribute
    bool GetBool(BoolAttr key) const;
    /// set int attribute
    void SetInt(IntAttr key, int value);
    /// get int attribute
    int GetInt(IntAttr key) const;
    /// set envelope attribute
    void SetEnvelope(EnvelopeAttr key, const EnvelopeCurve& value);
    /// get envelope attribute
    const EnvelopeCurve& GetEnvelope(EnvelopeAttr key) const;
	/// key to string
	static Util::String ToString(EnvelopeAttr key);
	/// key to string
	static Util::String ToString(IntAttr key);
	/// key to string
	static Util::String ToString(BoolAttr key);
	/// key to string
	static Util::String ToString(FloatAttr key);

	/// key to fourCC
	static Util::FourCC ToFourCC(EnvelopeAttr key);
	/// key to string
	static Util::FourCC ToFourCC(IntAttr key);
	/// key to string
	static Util::FourCC ToFourCC(BoolAttr key);
	/// key to string
	static Util::FourCC ToFourCC(FloatAttr key);
	
	// write data to stream
	virtual bool WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer);

private:
    float floatValues[NumFloatAttrs];
    int intValues[NumIntAttrs];
    bool boolValues[NumBoolAttrs];
    EnvelopeCurve envelopeValues[NumEnvelopeAttrs];
};

//------------------------------------------------------------------------------
/**
*/
inline void
EmitterAttrs::SetFloat(FloatAttr key, float value)
{
    this->floatValues[key] = value;
}

//------------------------------------------------------------------------------
/**
*/
inline float
EmitterAttrs::GetFloat(FloatAttr key) const
{
    return this->floatValues[key];
}

//------------------------------------------------------------------------------
/**
*/
inline void
EmitterAttrs::SetBool(BoolAttr key, bool value)
{
    this->boolValues[key] = value;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
EmitterAttrs::GetBool(BoolAttr key) const
{
    return this->boolValues[key];
}

//------------------------------------------------------------------------------
/**
*/
inline void
EmitterAttrs::SetInt(IntAttr key, int value)
{
    this->intValues[key] = value;
}

//------------------------------------------------------------------------------
/**
*/
inline int
EmitterAttrs::GetInt(IntAttr key) const
{
    return this->intValues[key];
}

//------------------------------------------------------------------------------
/**
*/
inline void
EmitterAttrs::SetEnvelope(EnvelopeAttr key, const EnvelopeCurve& value)
{
    this->envelopeValues[key] = value;
}

//------------------------------------------------------------------------------
/**
*/
inline const EnvelopeCurve&
EmitterAttrs::GetEnvelope(EnvelopeAttr key) const
{
    return this->envelopeValues[key];
}

} // namespace Particles
//------------------------------------------------------------------------------
    