/// \file
/// \brief Connection graph plugin, version 2. Tells new systems about existing and new connections
///
/// This file is part of RakNet Copyright 2003 Jenkins Software LLC
///
/// Usage of RakNet is subject to the appropriate license agreement.


#ifndef __CONNECTION_GRAPH_2_H
#define __CONNECTION_GRAPH_2_H

class RakPeerInterface;
#include "RakMemoryOverride.h"
#include "RakNetTypes.h"
#include "PluginInterface2.h"
#include "DS_List.h"
#include "DS_WeightedGraph.h"
#include "GetTime.h"
#include "Export.h"

/// \ingroup CONNECTION_GRAPH_GROUP
/// \brief A one hop connection graph.
/// Sends ID_CONNECTION_GRAPH_DISCONNECTION_NOTIFICATION, ID_CONNECTION_GRAPH_CONNECTION_LOST, ID_CONNECTION_GRAPH_NEW_CONNECTION
/// All identifiers are followed by SystemAddress, then RakNetGUID
class RAK_DLL_EXPORT ConnectionGraph2 : public PluginInterface2
{
public:
	
protected:
	/// \internal
	virtual void OnClosedConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID, PI2_LostConnectionReason lostConnectionReason );
	/// \internal
	virtual void OnNewConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID, bool isIncoming);
	
};

#endif
