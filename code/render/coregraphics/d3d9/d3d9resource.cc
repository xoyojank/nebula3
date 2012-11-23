//------------------------------------------------------------------------------
//  d3d9resource.cc
//  (C) 2010 xoyojank
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/d3d9/d3d9resource.h"
#include "coregraphics/d3d9/d3d9resourceeventhandler.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{

const D3D9Resource::ResourceHandle D3D9Resource::InvalidHandle;

//------------------------------------------------------------------------------
D3D9Resource::D3D9Resource() :
	isLosted(false)
{
	// empty
}

//------------------------------------------------------------------------------
/**
	because the resource is destroyed by reference count, so remove this from handler.
*/
D3D9Resource::~D3D9Resource()
{
	if (this->hResource != InvalidHandle)
	{
		this->RemoveFromResourceEventHandler();
	}
}

//------------------------------------------------------------------------------
void
D3D9Resource::AddToResourceEventHandler()
{
	// make sure it is not in the list
	if (this->hResource == InvalidHandle)
	{
		D3D9ResourceEventHandler* d3d9ResEventHandler = D3D9ResourceEventHandler::Instance();
		this->hResource = d3d9ResEventHandler->AddResource(this);
	}
}

//------------------------------------------------------------------------------
void
D3D9Resource::RemoveFromResourceEventHandler()
{
	D3D9ResourceEventHandler* d3d9ResEventHandler = D3D9ResourceEventHandler::Instance();
	d3d9ResEventHandler->RemoveResource(this->hResource);
	this->hResource = InvalidHandle;
}

//------------------------------------------------------------------------------
void
D3D9Resource::OnLostDevice()
{
	n_assert(!this->isLosted);

	this->isLosted = true;
}

//------------------------------------------------------------------------------
void
D3D9Resource::OnResetDevice()
{
	n_assert(this->isLosted);

	this->isLosted = false;
}

}// Direct3D9