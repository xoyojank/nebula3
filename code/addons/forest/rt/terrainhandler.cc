//------------------------------------------------------------------------------
//  foresthandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "forest/rt/terrainhandler.h"
#include "io/memorystream.h"
#include "internalgraphics/internalgraphicsserver.h"

namespace Terrain
{
	__ImplementClass(Terrain::TerrainHandler, 'FOEH', Interface::InterfaceHandlerBase);

	using namespace Messaging;

	//------------------------------------------------------------------------------
	/**
	*/
	TerrainHandler::TerrainHandler()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	TerrainHandler::~TerrainHandler()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	bool
	TerrainHandler::HandleMessage(const Ptr<Message>& msg)
	{
		n_assert(msg.isvalid());
		// fallthrough: message was handled
		return true;
	}

} // namespace Debug
