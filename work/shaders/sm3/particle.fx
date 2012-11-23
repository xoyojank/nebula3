//------------------------------------------------------------------------------
//  particle.fx
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "common.fxh"
#include "defaultsampler.fxh"
#include "particles.fxh"

//------------------------------------------------------------------------------
int CullMode = 1;
int AlphaRef = 128;
//------------------------------------------------------------------------------
// Unlit techniques
SimpleTechnique(Additive, "Additive", ParticleAdditiveVertexShader, ParticleAdditivePixelShader);
SimpleTechnique(AlphaUnlit, "AlphaUnlit", ParticleAdditiveVertexShader, ParticleUnlitPixelShader);
//------------------------------------------------------------------------------
// Unlit techniques
SimpleTechnique(NormalDepth, "NormalDepth", ParticleNormalDepthVertexShader, ParticleNormalDepthPixelShader);
SimpleTechnique(Alpha, "ParticleLit", ParticleVertexShader, ParticleAlphaPixelShader);

