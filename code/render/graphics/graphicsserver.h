#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::GraphicsServer
    
    Client-side proxy of the InternalGraphicsServer. Used to create and update 
    Stages and Views.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "graphics/stage.h"
#include "graphics/view.h"
#include "rendermodules/rendermodule.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class GraphicsServer : public Core::RefCounted
{
    __DeclareClass(GraphicsServer);
    __DeclareSingleton(GraphicsServer);
public:
    /// constructor
    GraphicsServer();
    /// destructor
    virtual ~GraphicsServer();

    /// open the graphics server 
    void Open();
    /// close the graphics server
    void Close();
    /// return true if graphics server is open
    bool IsOpen() const;
    /// perform client-side per-frame updates
    void OnFrame();

    /// create a stage
    Ptr<Stage> CreateStage(const Util::StringAtom& name, const Util::Array<Ptr<Visibility::VisibilitySystemBase> >& visSystems);
    /// discard a stage object
    void DiscardStage(const Ptr<Stage>& stage);
    /// discard all stage objects
    void DiscardAllStages();
    /// return true if a stage exists by name
    bool HasStage(const Util::StringAtom& name) const;
    /// lookup a stage by name
    const Ptr<Stage>& GetStageByName(const Util::StringAtom& name) const;
    /// get all stages
    const Util::Array<Ptr<Stage> >& GetStages() const;

    /// create a view
    Ptr<View> CreateView(const Core::Rtti& viewClass, const Util::StringAtom& viewName, const Util::StringAtom& stageName, const Resources::ResourceId& frameShaderName, bool isDefaultView=false);
    /// discard a view 
    void DiscardView(const Ptr<View>& view);
    /// discard all views
    void DiscardAllViews();
    /// return true if a view exists by name
    bool HasView(const Util::StringAtom& name) const;
    /// lookup a view by name
    const Ptr<View>& GetViewByName(const Util::StringAtom& name) const;
    /// get all view
    const Util::Array<Ptr<View> >& GetViews() const;
    /// set the default view
    void SetDefaultView(const Ptr<View>& view);
    /// get the default view
    const Ptr<View>& GetDefaultView() const;
    /// get frame count
    uint GetFrameCount() const;

    /// register a render module
    void RegisterRenderModule(const Ptr<RenderModules::RenderModule>& renderModule);
    /// unregister a render module
    void UnregisterRenderModule(const Ptr<RenderModules::RenderModule>& renderModule);
    /// get array of all currently registered render modules
    const Util::Array<Ptr<RenderModules::RenderModule> >& GetRenderModules() const;

private:
    Util::Array<Ptr<Stage> > stages;                         
    Util::Dictionary<Util::StringAtom, IndexT> stageIndexMap; 
    Util::Array<Ptr<View> > views;
    Util::Dictionary<Util::StringAtom, IndexT> viewIndexMap;
    Ptr<View> defaultView;
    Util::Array<Ptr<RenderModules::RenderModule> > renderModules;
    bool isOpen;
    uint frameCount;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
GraphicsServer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline uint 
GraphicsServer::GetFrameCount() const
{
    return this->frameCount;
}

} // namespace Graphics
//------------------------------------------------------------------------------
