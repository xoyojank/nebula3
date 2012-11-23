#include "PacketOutputWindowLogger.h"
#if defined(_WIN32) && !defined(_XBOX)
#include "win32/WindowsIncludes.h"
#include "RakString.h"
#endif

PacketOutputWindowLogger::PacketOutputWindowLogger()
{
}
PacketOutputWindowLogger::~PacketOutputWindowLogger()
{
}
void PacketOutputWindowLogger::WriteLog(const char *str)
{
#if defined(_WIN32) && !defined(_XBOX)
	RakNet::RakString str2 = str;
	str2+="\n";
	OutputDebugStr(str2.C_String());
#else
    str;        // just added to prevent warning of unreferenced variable
#endif
}
