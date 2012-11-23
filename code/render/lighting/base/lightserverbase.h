#pragma once
//------------------------------------------------------------------------------
/**
    @class Lighting::LightServerBase
    
    Base class for the light server. The light server collects all lights
    contributing to the scene. Subclasses of LightServerBase implement 
    specific lighting techniques.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "internalgraphics/internalcameraentity.h"
#include "internalgraphics/internalmodelentity.h"
#include "lighting/internalgloballightentity.h"

//------------------------------------------------------------------------------
namespace Lighting
{
class LightServerBase : public Core::RefCounted
{
    __DeclareClass(LightServerBase);
    __DeclareSingleton(LightServerBase);
public:
    /// constructor
    LightServerBase();
    /// destructor
    virtual ~LightServerBase();
    
    /// open the light server
    void Open();
    /// close the light server
    void Close();
    /// return true if light server is open
    bool IsOpen() const;
    /// indicate whether the light server requires light/model linking
    bool NeedsLightModelLinking() const;

    /// begin lighting frame
    void BeginFrame(const Ptr<InternalGraphics::InternalCameraEntity>& cameraEntity);
    /// begin attaching visible light sources
    void BeginAttachVisibleLights();
    /// attach a visible light source
    void AttachVisibleLight(const Ptr<InternalAbstractLightEntity>& lightEntity);
    /// end attaching visible light sources
    void EndAttachVisibleLights();
    /// apply lighting parameters for a visible model entity 
    void ApplyModelEntityLights(const Ptr<InternalGraphics::InternalModelEntity>& modelEntity);
    /// render light pass
    void RenderLights();
    /// end lighting frame
    void EndFrame();

protected:
    bool isOpen;
    bool inBeginFrame;
    bool inBeginAttach;
    Ptr<InternalGraphics::InternalCameraEntity> cameraEntity;
    Ptr<InternalGlobalLightEntity> globalLightEntity;
    Util::Array<Ptr<InternalAbstractLightEntity> > visibleLightEntities;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
LightServerBase::IsOpen() const
{
    return this->isOpen;
}

} // namespace Lighting
//------------------------------------------------------------------------------

    