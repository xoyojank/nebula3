#pragma once
//------------------------------------------------------------------------------
/**
    @class Particles::EnvelopeCurve
    
    An Attack/Sustain/Release envelope curve class with optional
    sine/cosine modulation. Used for animated particle emitter attributes.
    
    (C) 2008 Radon Labs GmbH
*/
#include "math/scalar.h"

//------------------------------------------------------------------------------
namespace Particles
{
class EnvelopeCurve
{
public:
    /// modulation enumerator
    enum ModFunc
    {
        Sine = 0,
        Cosine,
    };

    /// default constructor
    EnvelopeCurve();
    /// set parameters
    void Setup(float val0, float val1, float val2, float val3, float keyPos0, float keyPos1, float freq, float amp, ModFunc mod);
    /// sample at specific time (0..1)
    float Sample(float t) const;
    /// sample from t=0 to t=1 into array of values
    void PreSample(float* sampleBuffer, SizeT numSamples, SizeT sampleStride) const;
    /// get the max of val0, val1, val2, val3
    float GetMaxValue() const;

private:
    float values[4];
    float keyPos0;
    float keyPos1;
    float frequency;
    float amplitude;
    ModFunc modFunction;
};


} // namespace Particles
//------------------------------------------------------------------------------
