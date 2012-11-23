//------------------------------------------------------------------------------
//  lightflickerutil.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphicsutil/lightflickerutil.h"
#include "math/noise.h"
#include "basegametiming/gametimesource.h"

namespace GraphicsFeature
{
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
LightFlickerUtil::LightFlickerUtil() :
    enabled(true),
    frequency(5.0f),
    intensityAmplitude(0.5f),
    positionAmplitude(0.25f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This changes the transformation offset matrix and the intensity
    modulation of the light entity.
*/
void
LightFlickerUtil::Update()
{
    if (this->enabled)
    {
        n_assert(this->lightEntity.isvalid());
        float t = float(BaseGameFeature::GameTimeSource::Instance()->GetTime()) * this->frequency;
        
        // TODO:
        // get perlin noise 
        //float n = noise::gen(t, 0.0f, 0.0f);
        //float intensityModulate = 1.0f + (this->intensityAmplitude * n);
        //this->lightEntity->SetIntensityModulation(intensityModulate);
        
        matrix44 m = matrix44::identity();
        vector v(this->positionAmplitude * noise::gen(t, 1.1f, 0.0f), 
                  this->positionAmplitude * noise::gen(t, 2.2f, 0.0f), 
                  this->positionAmplitude * noise::gen(t, 3.3f, 0.0f));
        m.set_position(v);
        //this->lightEntity->SetTransformOffset(m);
    }
}

} // namespace GraphicsFeature
