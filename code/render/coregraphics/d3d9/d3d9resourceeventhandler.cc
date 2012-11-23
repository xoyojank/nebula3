//------------------------------------------------------------------------------
//  d3d9resourceeventhandler.cc
//  (C) 2010 xoyojank
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/d3d9/d3d9resourceeventhandler.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
using namespace CoreGraphics;
using namespace Util;

__ImplementClass(Direct3D9::D3D9ResourceEventHandler, 'D9RE', CoreGraphics::RenderEventHandler);
__ImplementSingleton(Direct3D9::D3D9ResourceEventHandler);

//------------------------------------------------------------------------------
D3D9ResourceEventHandler::D3D9ResourceEventHandler()
{
	__ConstructSingleton;
}

//------------------------------------------------------------------------------
D3D9ResourceEventHandler::~D3D9ResourceEventHandler()
{
	n_assert(this->d3d9Resources.IsEmpty());
	__DestructSingleton;
}


//------------------------------------------------------------------------------
D3D9Resource::ResourceHandle
D3D9ResourceEventHandler::AddResource(D3D9Resource* resource)
{
	n_assert(NULL != resource);

	D3D9Resource::ResourceHandle it = this->d3d9Resources.AddBack(resource);
	return it;
}

//------------------------------------------------------------------------------
void
D3D9ResourceEventHandler::RemoveResource(const D3D9Resource::ResourceHandle& hResource)
{
	n_assert(D3D9Resource::InvalidHandle != hResource);

	this->d3d9Resources.Remove(hResource);
}

//------------------------------------------------------------------------------
bool
D3D9ResourceEventHandler::HandleEvent(const RenderEvent& event)
{
	if (RenderEvent::DeviceLost == event.GetEventCode())
	{
		for (D3D9Resource::ResourceHandle it = this->d3d9Resources.Begin();
			it != this->d3d9Resources.End(); ++it)
		{
			D3D9Resource* d3d9Resource = *it;
			d3d9Resource->OnLostDevice();
		}
		return true;
	}
	else if (RenderEvent::DeviceRestored == event.GetEventCode())
	{
		for (D3D9Resource::ResourceHandle it = this->d3d9Resources.Begin();
			it != this->d3d9Resources.End(); ++it)
		{
			D3D9Resource* d3d9Resource = *it;
			d3d9Resource->OnResetDevice();
		}
		return true;
	}

	return false;
}

}// Direct3D9