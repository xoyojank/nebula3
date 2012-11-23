//------------------------------------------------------------------------------
//  particle.fx
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "lighting.fxh"
#include "common.fxh"
#include "defaultsampler.fxh"
#include "particles.fxh"

//------------------------------------------------------------------------------
int CullMode = 1;
int AlphaRef = 128;
//------------------------------------------------------------------------------
LightTechnique(Alpha0, "Alpha|LocalLights0", ParticleVertexShader, ParticleAlphaPixelShader, 0);
LightTechnique(Alpha1, "Alpha|LocalLights1", ParticleVertexShader, ParticleAlphaPixelShader, 1);
LightTechnique(Alpha2, "Alpha|LocalLights2", ParticleVertexShader, ParticleAlphaPixelShader, 2);
LightTechnique(Alpha3, "Alpha|LocalLights3", ParticleVertexShader, ParticleAlphaPixelShader, 3);
LightTechnique(Alpha4, "Alpha|LocalLights4", ParticleVertexShader, ParticleAlphaPixelShader, 4);

LightTechnique(Additive0, "Additive|LocalLights0", ParticleVertexShader, ParticleAdditivePixelShader, 0);
LightTechnique(Additive1, "Additive|LocalLights1", ParticleVertexShader, ParticleAdditivePixelShader, 1);
LightTechnique(Additive2, "Additive|LocalLights2", ParticleVertexShader, ParticleAdditivePixelShader, 2);
LightTechnique(Additive3, "Additive|LocalLights3", ParticleVertexShader, ParticleAdditivePixelShader, 3);
LightTechnique(Additive4, "Additive|LocalLights4", ParticleVertexShader, ParticleAdditivePixelShader, 4);

