#pragma once
//------------------------------------------------------------------------------
/**
    @class Lighting::LightPrePassServer
    
    A LightServer which implements pre-light-pass rendering.
    Check here for details:
    http://diaryofagraphicsprogrammer.blogspot.com/2008/03/light-pre-pass-renderer.html    
    
    (C) 2009 Radon Labs GmbH
*/
#include "lighting/base/lightserverbase.h"
#include "lighting/internalpointlightentity.h"
#include "lighting/internalspotlightentity.h"
#include "coregraphics/shaderinstance.h"
#include "resources/managedmesh.h"
#include "resources/managedtexture.h"
#include "renderutil/drawfullscreenquad.h"

//------------------------------------------------------------------------------
namespace Lighting
{
class LightPrePassServer : public LightServerBase
{
    __DeclareClass(LightPrePassServer);
public:
    /// constructor
    LightPrePassServer();
    /// destructor
    virtual ~LightPrePassServer();

    /// open the light server
    void Open();
    /// close the light server
    void Close();
    /// pre-light-pass doesn't require light/model linking
    bool NeedsLightModelLinking() const;

    /// attach a visible light source
    void AttachVisibleLight(const Ptr<InternalAbstractLightEntity>& lightEntity);
    /// end lighting frame
    void EndFrame();

    /// render light pass
    void RenderLights();

protected:
    /// render the global light
    void RenderGlobalLight();
    /// render all point lights
    void RenderPointLights();
    /// render all spot lights
    void RenderSpotLights();
    /// assign render buffers to shaders (one time init)
    void AssignRenderBufferTextures();

    enum ShadowFlag
    {
        CastShadows = 0,
        NoShadows,

        NumShadowFlags
    };

    Util::Array<Ptr<InternalPointLightEntity> > pointLights[NumShadowFlags];
    Util::Array<Ptr<InternalSpotLightEntity> > spotLights[NumShadowFlags];        
    Ptr<Resources::ManagedTexture> lightProjMap; 

    uint pointLightFeatureBits[NumShadowFlags];
    uint spotLightFeatureBits[NumShadowFlags];
    uint globalLightFeatureBits;
    RenderUtil::DrawFullScreenQuad fullScreenQuadRenderer;          // fs quad renderer
    Ptr<CoreGraphics::ShaderInstance> lightShaderInst;  // light source shader
    Ptr<CoreGraphics::ShaderVariable> lightPosRange;
    Ptr<CoreGraphics::ShaderVariable> lightColor;
    Ptr<CoreGraphics::ShaderVariable> globalLightDir;
    Ptr<CoreGraphics::ShaderVariable> globalLightColor;
    Ptr<CoreGraphics::ShaderVariable> globalBackLightColor;
    Ptr<CoreGraphics::ShaderVariable> globalAmbientLightColor;
    Ptr<CoreGraphics::ShaderVariable> globalBackLightOffset;
    Ptr<CoreGraphics::ShaderVariable> normalBufferVar;
    Ptr<CoreGraphics::ShaderVariable> dsfObjectDepthBufferVar;
    Ptr<CoreGraphics::ShaderVariable> lightBufferVar;     
    Ptr<CoreGraphics::ShaderVariable> lightProjTransform;   
    Ptr<CoreGraphics::ShaderVariable> shadowProjTransform;
    Ptr<CoreGraphics::ShaderVariable> lightProjMapVar;
    Ptr<CoreGraphics::ShaderVariable> shadowProjMapVar;      
    Ptr<CoreGraphics::ShaderVariable> shadowOffsetScaleVar;
    Ptr<CoreGraphics::ShaderVariable> shadowFadeVar;
    Ptr<CoreGraphics::ShaderVariable> shadowConstants;
    Ptr<Resources::ManagedMesh> pointLightMesh;         // unit sphere for rendering
    Ptr<Resources::ManagedMesh> spotLightMesh;          // unit sphere for rendering
    bool renderBuffersAssigned;          
};

} // namespace Lighting
//------------------------------------------------------------------------------
