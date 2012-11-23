#pragma once
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9Resource
  
	An abstract class for d3d9 resource only, sub-classes need to check the
	isReset variable to decide whether to run the method or not.

	@see   D3D9ResourceEventHandler
    
    (C) 2010 xoyojank
*/
#include "util/list.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{

class D3D9Resource
{
public:
	typedef Util::List<D3D9Resource*>::Iterator ResourceHandle;
	static const ResourceHandle InvalidHandle;
protected:
	/// constructor
	D3D9Resource();
	/// destructor
	virtual ~D3D9Resource();

	/// add this resource to handler, in order to manage device's lost/reset event
	void AddToResourceEventHandler();
	/// remove this from resource handler
	void RemoveFromResourceEventHandler();

	/// called when d3d9 device is lost
	virtual void OnLostDevice();
	/// called when d3d9 devcie is reset
	virtual void OnResetDevice();

protected:
	friend class D3D9ResourceEventHandler;

	bool isLosted;
	ResourceHandle hResource;
};

}// Direct3D9