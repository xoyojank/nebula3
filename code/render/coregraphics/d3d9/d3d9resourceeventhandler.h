#pragma once
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9ResourceEventHandler
  
    A RenderEventHandler to handle d3d9's lost/reset device event.
    
    (C) 2010 xoyojank
*/
#include "core/singleton.h"
#include "coregraphics/rendereventhandler.h"
#include "coregraphics/d3d9/d3d9resource.h"
#include "util/list.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{

class D3D9ResourceEventHandler : public CoreGraphics::RenderEventHandler
{
	__DeclareClass(D3D9ResourceEventHandler);
	__DeclareSingleton(D3D9ResourceEventHandler);
public:
	/// constructor
	D3D9ResourceEventHandler();
	/// destructor
	virtual ~D3D9ResourceEventHandler();

	/// add d3d9 resource(in default pool)
	D3D9Resource::ResourceHandle AddResource(D3D9Resource* resource);
	/// remove resource, automatically called by D3D9Resource's destructor
	void RemoveResource(const D3D9Resource::ResourceHandle& hResource);

private:
	/// called when an event should be processed
	bool HandleEvent(const CoreGraphics::RenderEvent& event);

private:
	Util::List<D3D9Resource*> d3d9Resources;
};

}// Direct3D9