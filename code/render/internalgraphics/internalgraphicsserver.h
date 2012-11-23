#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::InternalGraphicsServer
    
    The graphics server maintains a the "graphics world" consisting of 
    one or more "stages" and one or more "views" which are attached to
    the stages.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "util/stringatom.h"
#include "util/array.h"
#include "math/line.h"
#include "math/float2.h"
#include "timing/time.h"
#include "coregraphics/shadervariable.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "rendermodules/rt/rtpluginregistry.h"
#include "debug/debugtimer.h"
#include "visibility/visibilitysystems/visibilitysystembase.h"

// forward declarations
namespace Lighting
{
    class InternalGlobalLightEntity;
}

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class InternalStage;
class InternalView;
class StageBuilder;
    
class InternalGraphicsServer : public Core::RefCounted
{
    __DeclareClass(InternalGraphicsServer);
    __DeclareSingleton(InternalGraphicsServer);
public:
    /// constructor
    InternalGraphicsServer();
    /// destructor
    virtual ~InternalGraphicsServer();
    
    /// open the graphics server
    void Open();
    /// close the graphics server
    void Close();
    /// return true if graphics server is open
    bool IsOpen() const;
    
    /// create a stage object
    Ptr<InternalStage> CreateStage(const Util::StringAtom& name, const Util::Array<Ptr<Visibility::VisibilitySystemBase> >& visSystems);
    /// discard a stage object
    void DiscardStage(const Ptr<InternalStage>& stage);
    /// discard all stage objects
    void DiscardAllStages();
    /// return true if a stage exists by name
    bool HasStage(const Util::StringAtom& name) const;
    /// lookup a stage by name
    const Ptr<InternalStage>& GetStageByName(const Util::StringAtom& name) const;
    /// get all stages
    const Util::Array<Ptr<InternalStage> >& GetStages() const;

    /// create a view object
    Ptr<InternalView> CreateView(const Core::Rtti& viewClass, const Util::StringAtom& name, bool isDefaultView=false);
    /// discard a view object
    void DiscardView(const Ptr<InternalView>& view);
    /// discard all view objects
    void DiscardAllViews();
    /// return true if a view exists by name
    bool HasView(const Util::StringAtom& name) const;
    /// lookup a view by name
    const Ptr<InternalView>& GetViewByName(const Util::StringAtom& name) const;
    /// get all views
    const Util::Array<Ptr<InternalView> >& GetViews() const;
    /// set the default view
    void SetDefaultView(const Ptr<InternalView>& defView);
    /// get the default view
    const Ptr<InternalView>& GetDefaultView() const;

    /// return true if an entity exists by its unique id
    bool HasEntity(InternalGraphicsEntity::Id id) const;
    /// lookup an entity by its unique id
    const Ptr<InternalGraphicsEntity>& GetEntityById(InternalGraphicsEntity::Id id) const;
    /// get all entities
    const Util::Array<Ptr<InternalGraphicsEntity> >& GetEntities() const;
    
    /// call per-frame, this renders the default view
    void OnFrame(Timing::Time curTime, Timing::Time globalTimeFactor);
    /// set render debug flag
    void SetRenderDebug(bool b);
    /// get render debug flag
    bool GetRenderDebug() const;

    /// gets the current global light entity (may return INVALID ptr)
    Ptr<Lighting::InternalGlobalLightEntity> GetCurrentGlobalLightEntity() const;

private:
    friend class InternalGraphicsEntity;

    /// register a graphics entity
    void RegisterEntity(const Ptr<InternalGraphicsEntity>& entity);
    /// unregister a graphics entity
    void UnregisterEntity(const Ptr<InternalGraphicsEntity>& entity);

    Ptr<RenderModules::RTPluginRegistry> rtPluginRegistry;
    Util::Array<Ptr<InternalGraphicsEntity> > entities;
    Util::Dictionary<InternalGraphicsEntity::Id, IndexT> entityIndexMap;
    Util::Array<Ptr<InternalStage> > stages;                         
    Util::Dictionary<Util::StringAtom, IndexT> stageIndexMap; 
    Util::Array<Ptr<InternalView> > views;
    Util::Dictionary<Util::StringAtom, IndexT> viewIndexMap;
    Ptr<InternalView> defaultView;
    Ptr<CoreGraphics::ShaderVariable> timeShaderVar;
    bool isOpen;
    bool renderDebug;
    _declare_timer(InternalGfxServerEndFrameTimer);
    _declare_timer(InternalGfxServerRenderView);
    _declare_timer(InternalGfxServerUpdateVisibilityLinks);
    _declare_timer(InternalGfxServerUpdateLightLinks);
};

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalGraphicsServer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalGraphicsServer::SetRenderDebug(bool b)
{
    this->renderDebug = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
InternalGraphicsServer::GetRenderDebug() const
{
    return this->renderDebug;
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------

