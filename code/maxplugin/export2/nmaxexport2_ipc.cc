//-----------------------------------------------------------------------------
//  nmaxexport2_ipc.cc
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "export2/nmax.h"
#include "base/nmaxlistener.h"
#include "kernel/nipcclient.h"

static nIpcClient* ipcClient = 0;

//-----------------------------------------------------------------------------
/**
    Helper function to send a message via ipc to handle shader parameters.

    The following maxscript changes diffuse color of the material to [1.0 0.0 0.0 1.0].
    @verbatim
    nchangeshaderparameter "Standard" "common" "MatDiffuse" "1.0 0.0 0.0 1.0"
    @endverbatim

    @param toolkitShaderName e.g) 'Standard'
    @param shaderHandling e.g) 'common' or 'particle2'
    @param paramID e.g) 'MatDiffuse'
    @param value In the case of the value is color of a material, e.g)  '1.0, 1.0, 1.0, 1.0'
*/
bool nChangeShaderParameter(const char* toolkitShaderName, const char* shaderHandling, 
                            const char* paramID, const char* value)
{
    nString msg;

    msg += "/sys/servers/toolkit.changeshaderparameter";
    msg += " ";
    msg += "\"";
    msg += toolkitShaderName;
    msg += "\"";
    msg += " ";

    msg += "\"";
    msg += shaderHandling;//"common";
    msg += "\"";
    msg += " ";

    msg += "\"";
    msg += paramID;
    msg += "\"";
    msg += " ";

    msg += "\"";
    msg += value;
    msg += "\"";

#ifdef _DEBUG
    n_listener("%s\n", msg.Get());
#endif

    return nSendMsgIpc(msg.Get());
}

//-----------------------------------------------------------------------------
/**
    Do hotloading through IPC.
*/
bool nDoHotLoading(const char* objPath)
{
    nString msg;

    msg += "/sys/servers/toolkit.dohotloading";
    msg += " ";
    msg += "\"";
    msg += objPath;
    msg += "\"";

#ifdef _DEBUG
    n_listener("%s\n", msg.Get());
#endif

    return nSendMsgIpc(msg.Get());
}

//-----------------------------------------------------------------------------
/**
    Connnet IPC with the given host and port.

    The following maxscript try to connect IPC to the nviewer in local.
    @verbatim
    nconnectipc "localhost" "nviewer"
    @endverbatim

    @param host host to connect.
    @param port port to connect.

    @return true if the IPC is connected otherwise false.
*/
bool nConnectIpc(const char* host, const char* port)
{
    if (!host || !port)
    {
        n_listener("Nebula2 IPC connection failed!\n");
        return false;
    }

    nIpcAddress ipcAddress(host, port);
    
    ipcClient = n_new(nIpcClient);
    ipcClient->SetBlocking(true);
    if (!ipcClient->Connect(ipcAddress))
    {
        n_listener("Nebula2 IPC connection failed!\n");
        return false;
    }

    n_listener("Nebula2 IPC connected\n");

    return true;
}

//-----------------------------------------------------------------------------
/**
    Disconnect IPC.
*/
void nDisConnectIpc()
{
    if (ipcClient)
    {
        if (ipcClient->IsConnected())
        {
            ipcClient->Disconnect();

            n_listener("Nebula2 IPC disconnected.\n");
        }

        n_delete(ipcClient);
        ipcClient = 0;
    }
}

//-----------------------------------------------------------------------------
/**
    Send the given message via connected IPC.
*/
bool nSendMsgIpc(const char* msg)
{
    static nIpcBuffer ipcBuf(4096);

    ipcBuf.SetString(msg);

    if (0 == ipcClient)
    {
        n_listener("Invalid IPC client.\n");
        return false;
    }

    return ipcClient->Send(ipcBuf);
}

//-----------------------------------------------------------------------------
/**
    Check the validation of IPC connection.
*/
bool nIsConnectedIpc()
{
    if (ipcClient && ipcClient->IsConnected())
        return true;

    return false;
}
