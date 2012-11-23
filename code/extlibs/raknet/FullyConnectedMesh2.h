/// \file
/// \brief Fully connected mesh plugin, revision 2.  This will connect RakPeer to all connecting peers, and all peers the connecting peer knows about.
///
/// This file is part of RakNet Copyright 2003 Jenkins Software LLC
///
/// Usage of RakNet is subject to the appropriate license agreement.


#ifndef __FULLY_CONNECTED_MESH_2_H
#define __FULLY_CONNECTED_MESH_2_H

class RakPeerInterface;
#include "PluginInterface2.h"
#include "RakMemoryOverride.h"
#include "DS_Multilist.h"

/// \defgroup FULLY_CONNECTED_MESH_GROUP FullyConnectedMesh2
/// \ingroup PLUGINS_GROUP

/// Fully connected mesh plugin, revision 2
/// This will connect RakPeer to all connecting peers, and all peers the connecting peer knows about.
/// It will also calculate which system has been running longest, to find out who should be host, if you need one system to act as a host
/// \pre You must also install the ConnectionGraph2 plugin
/// \ingroup FULLY_CONNECTED_MESH_GROUP
class RAK_DLL_EXPORT FullyConnectedMesh2 : public PluginInterface2
{
public:
	FullyConnectedMesh2();
	virtual ~FullyConnectedMesh2();

	/// \return System address of whichever system is host. Will return UNASSIGNED_RAKNET_GUID if we are not connected to anyone, or if we are connected and are calculating the host
	RakNetGUID GetConnectedHost(void) const;

	/// \return System address of whichever system is host. Always returns something, even though it may be our own system.
	RakNetGUID GetHostSystem(void) const;

	/// \return If our system is host
	bool IsHostSystem(void) const;

	/// \return If our system is host
	bool IsConnectedHost(void) const;


	/// \internal
	virtual void Update(void);
	/// \internal
	virtual PluginReceiveResult OnReceive(Packet *packet);
	/// \internal
	virtual void OnStartup(void);
	/// \internal
	virtual void OnAttach(void);
	/// \internal
	virtual void OnShutdown(void);
	/// \internal
	virtual void OnClosedConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID, PI2_LostConnectionReason lostConnectionReason );
/// \internal
	virtual void OnNewConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID, bool isIncoming);


	struct RemoteSystemWithTime
	{
		RemoteSystemWithTime() {elapsedRuntime=0;}
		RakNetGUID guid;
		SystemAddress systemAddress;
		RakNetTimeUS elapsedRuntime, newElapsedRuntime;
		bool HasElapsedRuntime(void) const {return elapsedRuntime!=0;}
	};
protected:
	void Clear(void);
	void PushNewHost(void);
	DataStructures::DefaultIndexType CalculateHostSystemIndex(void) const;
	void CalculateNewElapsedRuntime(void);
	RakNetTime lastTimeCalculatedNewElapsedRuntime;
	RakNetTimeUS startupTime;
	RakNetTimeUS lastElapsedRuntime;
	RakNetTimeUS newElapsedRuntime;
	RakNetGUID lastRemoteHostGuid;
	SystemAddress lastRemoteHostAddress;
	DataStructures::Multilist<ML_ORDERED_LIST, RemoteSystemWithTime*, RakNetGUID> remoteSystemList;
};

#endif
