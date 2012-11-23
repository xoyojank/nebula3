//------------------------------------------------------------------------------
//  graphicsserver.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsserver.h"

namespace Graphics
{
__ImplementClass(Graphics::GraphicsServer, 'GSRV', Core::RefCounted);
__ImplementSingleton(Graphics::GraphicsServer);

using namespace Util;
using namespace Math;
using namespace RenderModules;
using namespace InternalGraphics;

//------------------------------------------------------------------------------
/**
*/
GraphicsServer::GraphicsServer() :
    isOpen(false),
    frameCount(0)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GraphicsServer::~GraphicsServer()
{
    n_assert(!this->isOpen);
    n_assert(this->stages.IsEmpty());
    n_assert(this->stageIndexMap.IsEmpty());
    n_assert(this->views.IsEmpty());
    n_assert(this->viewIndexMap.IsEmpty());
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::Open()
{
    n_assert(!this->isOpen);
    this->isOpen = true;
    this->frameCount = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::Close()
{
    n_assert(this->isOpen);
    this->DiscardAllViews();
    this->DiscardAllStages();
    this->renderModules.Clear();
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<Stage>
GraphicsServer::CreateStage(const Util::StringAtom& name, const Util::Array<Ptr<Visibility::VisibilitySystemBase> >& visSystems)
{
    n_assert(this->isOpen);
    n_assert(!this->stageIndexMap.Contains(name));

    Ptr<Stage> stage = Stage::Create();
    stage->SetName(name);
    stage->Setup(visSystems);

    this->stages.Append(stage);
    this->stageIndexMap.Add(name, this->stages.Size() - 1);

    return stage;
}

//------------------------------------------------------------------------------
/**
    FIXME FIXME FIXME: this method is broken when more then 1 stage
    exists (indices in stageIndexMap are broken after first stage is removed).
*/
void
GraphicsServer::DiscardStage(const Ptr<Stage>& stage)
{
    n_assert(this->stageIndexMap.Contains(stage->GetName()));
    this->stages.EraseIndex(this->stageIndexMap[stage->GetName()]);
    this->stageIndexMap.Erase(stage->GetName());
    stage->Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::DiscardAllStages()
{
    IndexT i;
    for (i = 0; i < this->stages.Size(); i++)
    {
        this->stages[i]->Discard();
    }
    this->stages.Clear();
    this->stageIndexMap.Clear();
}

//------------------------------------------------------------------------------
/**
*/
bool
GraphicsServer::HasStage(const StringAtom& stageName) const
{
    return this->stageIndexMap.Contains(stageName);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Stage>&
GraphicsServer::GetStageByName(const StringAtom& stageName) const
{
    return this->stages[this->stageIndexMap[stageName]];
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<Stage> >&
GraphicsServer::GetStages() const
{
    return this->stages;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<View>
GraphicsServer::CreateView(const Core::Rtti& viewClass, const Util::StringAtom& viewName, const Util::StringAtom& stageName, const Resources::ResourceId& frameShaderName, bool isDefaultView)
{
    n_assert(this->isOpen);
    n_assert(!this->viewIndexMap.Contains(viewName));

    Ptr<View> view = View::Create();
    view->SetName(viewName);
    view->SetViewClass(viewClass);
    view->SetStageName(stageName);
    view->SetFrameShaderName(frameShaderName);
    view->SetDefaultView(isDefaultView);
    view->Setup();

    this->views.Append(view);
    this->viewIndexMap.Add(viewName, this->views.Size() - 1);
    if (isDefaultView)
    {
        this->defaultView = view;
    }
    return view;
}

//------------------------------------------------------------------------------
/**
    FIXME FIXME FIXME: this method is broken when more then 1 view
    exists (indices in viewIndexMap are broken after first view is removed).
*/
void
GraphicsServer::DiscardView(const Ptr<View>& view)
{
    n_assert(this->isOpen);
    n_assert(this->viewIndexMap.Contains(view->GetName()));

    this->views.EraseIndex(this->viewIndexMap[view->GetName()]);
    this->viewIndexMap.Erase(view->GetName());
    if (view == this->defaultView)
    {
        this->defaultView = 0;
    }
    view->Discard();
}

//------------------------------------------------------------------------------
/**
*/
void 
GraphicsServer::SetDefaultView(const Ptr<View>& view)
{
    n_assert(this->isOpen);
    n_assert(this->viewIndexMap.Contains(view->GetName()));

    if(this->defaultView != view)
    {
        this->defaultView->SetDefaultView(false);
        this->defaultView = view;
        view->SetDefaultView(true);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::DiscardAllViews()
{
    n_assert(this->isOpen);
    IndexT i;
    for (i = 0; i < this->views.Size(); i++)
    {
        this->views[i]->Discard();
    }
    this->views.Clear();
    this->viewIndexMap.Clear();
    this->defaultView = 0;
}

//------------------------------------------------------------------------------
/**
*/
bool
GraphicsServer::HasView(const StringAtom& viewName) const
{
    return this->viewIndexMap.Contains(viewName);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<View>&
GraphicsServer::GetViewByName(const StringAtom& viewName) const
{
    return this->views[this->viewIndexMap[viewName]];
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<View> >&
GraphicsServer::GetViews() const
{
    return this->views;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<View>&
GraphicsServer::GetDefaultView() const
{
    return this->defaultView;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::OnFrame()
{
    // notify stages
    IndexT i;
    for (i = 0; i < this->stages.Size(); i++)
    {
        this->stages[i]->OnFrame();
    }

    // notify render modules
    for (i = 0; i < this->renderModules.Size(); i++)
    {
        this->renderModules[i]->OnFrame();
    }

    this->frameCount++;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::RegisterRenderModule(const Ptr<RenderModule>& renderModule)
{
    n_assert(InvalidIndex == this->renderModules.FindIndex(renderModule));
    this->renderModules.Append(renderModule);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::UnregisterRenderModule(const Ptr<RenderModule>& renderModule)
{
    IndexT renderModuleIndex = this->renderModules.FindIndex(renderModule);
    n_assert(InvalidIndex != renderModuleIndex);
    this->renderModules.EraseIndex(renderModuleIndex);
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<RenderModule> >&
GraphicsServer::GetRenderModules() const
{
    return this->renderModules;
}

} // namespace Graphics
