/// \file
///
/// This file is part of RakNet Copyright 2003 Jenkins Software LLC
///
/// Usage of RakNet is subject to the appropriate license agreement.


#include "FullyConnectedMesh2.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakAssert.h"
#include "GetTime.h"

DEFINE_MULTILIST_PTR_TO_MEMBER_COMPARISONS(FullyConnectedMesh2::RemoteSystemWithTime, RakNetGUID, guid)

FullyConnectedMesh2::FullyConnectedMesh2()
{
	startupTime=0;
	lastTimeCalculatedNewElapsedRuntime=0;
	lastRemoteHostGuid=UNASSIGNED_RAKNET_GUID;
	lastRemoteHostAddress=UNASSIGNED_SYSTEM_ADDRESS;
	lastElapsedRuntime=0;
}
FullyConnectedMesh2::~FullyConnectedMesh2()
{
	Clear();
}
RakNetGUID FullyConnectedMesh2::GetConnectedHost(void) const
{
	if (remoteSystemList.GetSize()==0)
		return UNASSIGNED_RAKNET_GUID;

	return lastRemoteHostGuid;
}
RakNetGUID FullyConnectedMesh2::GetHostSystem(void) const
{
	return lastRemoteHostGuid;
}
bool FullyConnectedMesh2::IsHostSystem(void) const
{
	return GetHostSystem()==rakPeerInterface->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS);
}
bool FullyConnectedMesh2::IsConnectedHost(void) const
{
	return GetConnectedHost()==rakPeerInterface->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS);
}
void FullyConnectedMesh2::Update(void)
{
	// If got lastTimeCalculatedNewElapsedRuntime and one second passed, move all newElapsedRuntime to elapsedRuntime
	RakNetTimeUS curTime=RakNet::GetTimeUS();
	if (lastTimeCalculatedNewElapsedRuntime &&
		curTime > lastTimeCalculatedNewElapsedRuntime &&
		curTime > lastTimeCalculatedNewElapsedRuntime+1000)
	{
		DataStructures::DefaultIndexType lastHostIndex=CalculateHostSystemIndex();
		lastTimeCalculatedNewElapsedRuntime=0;
		
		// Update all runtimes and calculate new host, if any
		lastElapsedRuntime=newElapsedRuntime;
		DataStructures::DefaultIndexType i;
		for (i=0; i < remoteSystemList.GetSize(); i++)
			remoteSystemList[i]->elapsedRuntime=remoteSystemList[i]->newElapsedRuntime;

		DataStructures::DefaultIndexType newHostIndex=CalculateHostSystemIndex();
		if (lastHostIndex!=newHostIndex)
		{
			if (newHostIndex==(DataStructures::DefaultIndexType)-1)
			{
				lastRemoteHostGuid=rakPeerInterface->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS);
				lastRemoteHostAddress=rakPeerInterface->GetInternalID(UNASSIGNED_SYSTEM_ADDRESS);
			}
			else
			{
				lastRemoteHostGuid=remoteSystemList[newHostIndex]->guid;
				lastRemoteHostAddress=remoteSystemList[newHostIndex]->systemAddress;
			}
			PushNewHost();
		}
	}

}
PluginReceiveResult FullyConnectedMesh2::OnReceive(Packet *packet)
{
	switch (packet->data[0])
	{
	case ID_REMOTE_NEW_INCOMING_CONNECTION:
		{
			unsigned int count;
			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(MessageID));
			bsIn.Read(count);
			SystemAddress remoteAddress;
			RakNetGUID remoteGuid;
			char str[64];
			for (unsigned int i=0; i < count; i++)
			{
				bsIn.Read(remoteAddress);
				bsIn.Read(remoteGuid);
				remoteAddress.ToString(false,str);
				rakPeerInterface->Connect(str,remoteAddress.port,0,0);
			}
		}
		break;

		case ID_FCM2_ELAPSED_RUNTIME:
		{
			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(MessageID));
			RakNetTimeUS elapsedRuntime;
			bsIn.Read(elapsedRuntime);

			lastTimeCalculatedNewElapsedRuntime = (RakNetTime)RakNet::GetTimeUS();

			DataStructures::DefaultIndexType i = remoteSystemList.GetIndexOf(packet->guid);
			if (i!=(DataStructures::DefaultIndexType)-1)
			{
				remoteSystemList[i]->newElapsedRuntime=elapsedRuntime;
			}
			else
			{
				RemoteSystemWithTime *rswt = RakNet::OP_NEW<RemoteSystemWithTime>(__FILE__,__LINE__);
				rswt->elapsedRuntime=0;
				rswt->guid=packet->guid;
				rswt->newElapsedRuntime=elapsedRuntime;
				rswt->systemAddress=packet->systemAddress;
				remoteSystemList.Push(rswt,packet->guid,__FILE__,__LINE__);
			}
		}
		break;
	}
	return RR_CONTINUE_PROCESSING;
}
void FullyConnectedMesh2::OnStartup(void)
{
	startupTime=RakNet::GetTimeUS();
}
void FullyConnectedMesh2::OnAttach(void)
{
	// In case Startup() was called first
	if (rakPeerInterface->IsActive())
		startupTime=RakNet::GetTimeUS();
	lastRemoteHostGuid=rakPeerInterface->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS);
	lastRemoteHostAddress=rakPeerInterface->GetInternalID(UNASSIGNED_SYSTEM_ADDRESS);
	lastElapsedRuntime=0;
}
void FullyConnectedMesh2::OnShutdown(void)
{
	Clear();
	startupTime=0;
	lastTimeCalculatedNewElapsedRuntime=0;
}
void FullyConnectedMesh2::OnClosedConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID, PI2_LostConnectionReason lostConnectionReason )
{
	(void) lostConnectionReason;
	(void) systemAddress;

	DataStructures::DefaultIndexType droppedSystemIndex = remoteSystemList.GetIndexOf(rakNetGUID);
	if (droppedSystemIndex==(DataStructures::DefaultIndexType)-1)
		return;

	if (remoteSystemList[droppedSystemIndex]->guid==lastRemoteHostGuid)
	{
		remoteSystemList.RemoveAtIndex(droppedSystemIndex,__FILE__,__LINE__);
		DataStructures::DefaultIndexType newHostIndex=CalculateHostSystemIndex();
		// New host
		if (newHostIndex==(DataStructures::DefaultIndexType)-1)
		{
			lastRemoteHostGuid=rakPeerInterface->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS);
			lastRemoteHostAddress=rakPeerInterface->GetInternalID(UNASSIGNED_SYSTEM_ADDRESS);
		}
		else
		{
			lastRemoteHostGuid=remoteSystemList[newHostIndex]->guid;
			lastRemoteHostAddress=remoteSystemList[newHostIndex]->systemAddress;
		}
		PushNewHost();
	}
}
void FullyConnectedMesh2::CalculateNewElapsedRuntime(void)
{
	RakNetTimeUS curTime=RakNet::GetTimeUS();
	if (curTime>startupTime)
		newElapsedRuntime=curTime-startupTime;
	else
		newElapsedRuntime=0;

	if (lastElapsedRuntime==0)
		lastElapsedRuntime=newElapsedRuntime;

	lastTimeCalculatedNewElapsedRuntime=(RakNetTime)curTime;
}
void FullyConnectedMesh2::OnNewConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID, bool isIncoming)
{
	(void) isIncoming;
	(void) rakNetGUID;
	(void) systemAddress;

	CalculateNewElapsedRuntime();

	RakNet::BitStream bsOut;
	bsOut.Write((MessageID)ID_FCM2_ELAPSED_RUNTIME);
	bsOut.Write(newElapsedRuntime);
	// Intentional broadcast
	DataStructures::DefaultIndexType i;
	for (i=0; i < remoteSystemList.GetSize(); i++)
	{
		rakPeerInterface->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,remoteSystemList[i]->systemAddress,false);
	}
	rakPeerInterface->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,systemAddress,false);

}
void FullyConnectedMesh2::Clear(void)
{
	remoteSystemList.ClearPointers(false,__FILE__,__LINE__);
}
void FullyConnectedMesh2::PushNewHost(void)
{
	Packet *p = rakPeerInterface->AllocatePacket(sizeof(MessageID));
	p->data[0]=ID_FCM2_NEW_HOST;
	p->systemAddress=lastRemoteHostAddress;
	p->systemIndex=(SystemIndex)-1;
	p->guid=lastRemoteHostGuid;
	rakPeerInterface->PushBackPacket(p, true);
}

DataStructures::DefaultIndexType FullyConnectedMesh2::CalculateHostSystemIndex(void) const
{
	if (remoteSystemList.GetSize()==0)
		return (DataStructures::DefaultIndexType)-1;

	// Return system with highest time. If none, return ourselves
	DataStructures::DefaultIndexType i, highestRuntimeIndex=0;
	RakNetTimeUS highestRuntime=remoteSystemList[0]->elapsedRuntime;
	for (i=1; i < remoteSystemList.GetSize(); i++)
	{
		if (remoteSystemList[i]->elapsedRuntime > highestRuntime)
		{
			highestRuntime=remoteSystemList[i]->elapsedRuntime;
			highestRuntimeIndex=i;
		}
	}

	if (lastElapsedRuntime>highestRuntime)
		return (DataStructures::DefaultIndexType)-1;

	return highestRuntimeIndex;
}
