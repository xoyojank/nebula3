/// \file
/// \brief A RakNet plugin performing networking to communicate with UDPProxyCoordinator. Ultimately used to tell UDPProxyServer to forward UDP packets.
///
/// This file is part of RakNet Copyright 2003 Jenkins Software LLC
///
/// Usage of RakNet is subject to the appropriate license agreement.
/// Creative Commons Licensees are subject to the
/// license found at
/// http://creativecommons.org/licenses/by-nc/2.5/
/// Single application licensees are subject to the license found at
/// http://www.jenkinssoftware.com/SingleApplicationLicense.html
/// Custom license users are subject to the terms therein.

#ifndef __UDP_PROXY_CLIENT_H
#define __UDP_PROXY_CLIENT_H

#include "Export.h"
#include "DS_Multilist.h"
#include "RakNetTypes.h"
#include "PluginInterface2.h"

namespace RakNet
{
class UDPProxyClient;

/// Callback to handle results of calling UDPProxyClient::RequestForwarding()
struct UDPProxyClientResultHandler
{
	/// Called when our forwarding request was completed. We can now connect to \a targetAddress by using \a proxyAddress instead
	/// \param[out] proxyIPAddress IP Address of the proxy server, which will forward messages to targetAddress
	/// \param[out] proxyPort Remote port to use on the proxy server, which will forward messages to targetAddress
	/// \param[out] reverseProxyPort The port the destination will reply back to us on. You may not need this.
	/// \param[out] proxyCoordinator \a proxyCoordinator parameter originally passed to UDPProxyClient::RequestForwarding
	/// \param[out] sourceAddress \a sourceAddress parameter passed to UDPProxyClient::RequestForwarding. If it was UNASSIGNED_SYSTEM_ADDRESS, it is now our external IP address.
	/// \param[out] targetAddress \a targetAddress parameter originally passed to UDPProxyClient::RequestForwarding
	/// \param[out] proxyClient The plugin that is calling this callback
	virtual void OnForwardingSuccess(const char *proxyIPAddress, unsigned short proxyPort, unsigned short reverseProxyPort,
		SystemAddress proxyCoordinator, SystemAddress sourceAddress, SystemAddress targetAddress, RakNet::UDPProxyClient *proxyClientPlugin)=0;

	/// Called when another system has setup forwarding, with our system as the target address.
	/// Plugin automatically sends a datagram to proxyIPAddress before this callback, to open our router if necessary.
	/// \param[out] proxyIPAddress IP Address of the proxy server, which will forward messages to targetAddress
	/// \param[out] proxyPort Remote port to use on the proxy server, which will forward messages to targetAddress
	/// \param[out] reverseProxyPort The port the destination will reply back to us on. You may not need this.
	/// \param[out] proxyCoordinator \a proxyCoordinator parameter originally passed to UDPProxyClient::RequestForwarding
	/// \param[out] sourceAddress \a sourceAddress parameter passed to UDPProxyClient::RequestForwarding. This is originating source IP address of the remote system that will be sending to us.
	/// \param[out] targetAddress \a targetAddress parameter originally passed to UDPProxyClient::RequestForwarding. This is our external IP address.
	/// \param[out] proxyClient The plugin that is calling this callback
	virtual void OnForwardingNotification(const char *proxyIPAddress, unsigned short proxyPort, unsigned short reverseProxyPort,
		SystemAddress proxyCoordinator, SystemAddress sourceAddress, SystemAddress targetAddress, RakNet::UDPProxyClient *proxyClientPlugin)=0;

	/// Called when our forwarding request failed, because no UDPProxyServers are connected to UDPProxyCoordinator
	/// \param[out] proxyCoordinator \a proxyCoordinator parameter originally passed to UDPProxyClient::RequestForwarding
	/// \param[out] sourceAddress \a sourceAddress parameter passed to UDPProxyClient::RequestForwarding. If it was UNASSIGNED_SYSTEM_ADDRESS, it is now our external IP address.
	/// \param[out] targetAddress \a targetAddress parameter originally passed to UDPProxyClient::RequestForwarding
	/// \param[out] proxyClient The plugin that is calling this callback
	virtual void OnNoServersOnline(SystemAddress proxyCoordinator, SystemAddress sourceAddress, SystemAddress targetAddress, RakNet::UDPProxyClient *proxyClientPlugin)=0;

	/// Called when our forwarding request failed, because all UDPProxyServers that are connected to UDPProxyCoordinator are at their capacity
	/// Either add more servers, or increase capacity via UDPForwarder::SetMaxForwardEntries()
	/// \param[out] proxyCoordinator \a proxyCoordinator parameter originally passed to UDPProxyClient::RequestForwarding
	/// \param[out] sourceAddress \a sourceAddress parameter passed to UDPProxyClient::RequestForwarding. If it was UNASSIGNED_SYSTEM_ADDRESS, it is now our external IP address.
	/// \param[out] targetAddress \a targetAddress parameter originally passed to UDPProxyClient::RequestForwarding
	/// \param[out] proxyClient The plugin that is calling this callback
	virtual void OnAllServersBusy(SystemAddress proxyCoordinator, SystemAddress sourceAddress, SystemAddress targetAddress, RakNet::UDPProxyClient *proxyClientPlugin)=0;

	/// Called when our forwarding request is already in progress on the \a proxyCoordinator.
	/// This can be ignored, but indicates an unneeded second request
	/// \param[out] proxyCoordinator \a proxyCoordinator parameter originally passed to UDPProxyClient::RequestForwarding
	/// \param[out] sourceAddress \a sourceAddress parameter passed to UDPProxyClient::RequestForwarding. If it was UNASSIGNED_SYSTEM_ADDRESS, it is now our external IP address.
	/// \param[out] targetAddress \a targetAddress parameter originally passed to UDPProxyClient::RequestForwarding
	/// \param[out] proxyClient The plugin that is calling this callback
	virtual void OnForwardingInProgress(SystemAddress proxyCoordinator, SystemAddress sourceAddress, SystemAddress targetAddress, RakNet::UDPProxyClient *proxyClientPlugin)=0;
};


/// When NAT Punchthrough fails, it is possible to use a non-NAT system to forward messages from us to the recipient, and vice-versa
/// The class to forward messages is UDPForwarder, and it is triggered over the network via the UDPProxyServer plugin.
/// The UDPProxyClient connects to UDPProxyCoordinator to get a list of servers running UDPProxyServer, and the coordinator will relay our forwarding request
/// \brief Communicates with UDPProxyCoordinator, in order to find a UDPProxyServer to forward our datagrams.
/// \ingroup NAT_PUNCHTHROUGH_GROUP
class RAK_DLL_EXPORT UDPProxyClient : public PluginInterface2
{
public:
	UDPProxyClient();
	~UDPProxyClient();

	/// Receives the results of calling RequestForwarding()
	/// Set before calling RequestForwarding or you won't know what happened
	/// \param[in] resultHandler 
	void SetResultHandler(UDPProxyClientResultHandler *rh);

	/// Sends a request to proxyCoordinator to find a server and have that server setup UDPForwarder::StartForwarding() on our address to \a targetAddress
	/// The forwarded datagrams can be from any UDP source, not just RakNet
	/// \pre Must be connected to \a proxyCoordinator
	/// \pre Systems running UDPProxyServer must be connected to \a proxyCoordinator and logged in via UDPProxyCoordinator::LoginServer() or UDPProxyServer::LoginToCoordinator()
	/// \note May still fail, if all proxy servers have no open connections.
	/// \note RakNet's protocol will ensure a message is sent at least every 5 seconds, so if routing RakNet messages, it is a reasonable value for timeoutOnNoDataMS, plus an extra few seconds for latency.
	/// \param[in] proxyCoordinator System we are connected to that is running the UDPProxyCoordinator plugin
	/// \param[in] sourceAddress External IP address of the system we want to forward messages from. This does not have to be our own system. To specify our own system, you can pass UNASSIGNED_SYSTEM_ADDRESS which the coordinator will treat as our external IP address.
	/// \param[in] targetAddress External IP address of the system we want to forward messages to. This system does NOT have to be connected or otherwise do anything.
	/// \param[in] timeoutOnNoData If no data is sent by the forwarded systems, how long before removing the forward entry from UDPForwarder? UDP_FORWARDER_MAXIMUM_TIMEOUT is the maximum value. Recommended high enough to not drop valid connections, but low enough to not waste forwarding slots on the proxy server.
	/// \param[in] serverSelectionBitstream If you want to send data to UDPProxyCoordinator::GetBestServer(), write it here
	/// \return true if the request was sent, false if we are not connected to proxyCoordinator
	bool RequestForwarding(SystemAddress proxyCoordinator, SystemAddress sourceAddress, SystemAddress targetAddress, RakNetTimeMS timeoutOnNoDataMS, RakNet::BitStream *serverSelectionBitstream=0);

	/// \internal
	virtual void Update(void);
	virtual PluginReceiveResult OnReceive(Packet *packet);
	virtual void OnShutdown(void);
	
	struct ServerWithPing
	{
		unsigned short ping;
		SystemAddress serverAddress;
	};
	struct SenderAndTargetAddress
	{
		SystemAddress senderClientAddress;
		SystemAddress targetClientAddress;
	};
	struct PingServerGroup
	{
		SenderAndTargetAddress sata;
		RakNetTime startPingTime;
		SystemAddress coordinatorAddressForPings;
		DataStructures::Multilist<ML_UNORDERED_LIST, ServerWithPing> serversToPing;
		bool AreAllServersPinged(void) const;
		void SendPingedServersToCoordinator(RakPeerInterface *rakPeerInterface);
	};
	DataStructures::Multilist<ML_UNORDERED_LIST, PingServerGroup*> pingServerGroups;
protected:

	void OnPingServers(Packet *packet);
	void Clear(void);
	UDPProxyClientResultHandler *resultHandler;

};

} // End namespace

#endif
