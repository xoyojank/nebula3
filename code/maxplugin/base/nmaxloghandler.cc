//------------------------------------------------------------------------------
//  nmaxloghandler.cc
//
//  (C)2004 Johannes Kellner 
//------------------------------------------------------------------------------
#include "base/nmaxloghandler.h"
#include "kernel/ndefaultloghandler.h"

//------------------------------------------------------------------------------
/**
*/
nMaxLogHandler::nMaxLogHandler() :
	log(0), 
    numMsgDismissed(0)
{
	//empty
}

//------------------------------------------------------------------------------
/**
*/
nMaxLogHandler::~nMaxLogHandler()
{
	//empty
}

//------------------------------------------------------------------------------
/**
*/
void
nMaxLogHandler::Print(const char* msg, va_list argList)
{
	this->doLog(LOG_PRINT, msg, argList);
}

//------------------------------------------------------------------------------
/**
*/
void
nMaxLogHandler::Message(const char* msg, va_list argList)
{
	this->doLog(LOG_MESSAGE, msg, argList);
}

//------------------------------------------------------------------------------
/**
*/
void
nMaxLogHandler::Error(const char* msg, va_list argList)
{
	this->doLog(LOG_ERROR, msg, argList);
}

//------------------------------------------------------------------------------
/*
*/
void 
nMaxLogHandler::OutputDebug(const char* msg, va_list argList)
{
    char buf[LOG_SIZE];
    int len = vsprintf(buf, msg, argList); 

    OutputDebugString(buf);
}

//------------------------------------------------------------------------------
/**
    Output a log message.
*/
void
nMaxLogHandler::doLog(logType type, const char* msg, va_list argList)
{
	if (this->log)
	{
		char* buf = n_new_array(char, LOG_SIZE);
		int len = vsprintf(buf, msg, argList); 
				
		switch (type)
		{
			case LOG_PRINT:
				this->log->LogEntry(SYSLOG_INFO, NO_DIALOG, NULL, buf);
				break;
			case LOG_MESSAGE:
				this->log->LogEntry(SYSLOG_WARN, DISPLAY_DIALOG, "Nebula2 - Message", buf);
				break;
			case LOG_ERROR:
				this->log->LogEntry(SYSLOG_ERROR, DISPLAY_DIALOG, "Nebula2 - Error", buf);
				break;
		}
		n_delete_array(buf);
	}
	else
    {
		this->numMsgDismissed++;
	    //TODO: add a fifo for the messeages before we have a ptr to the max syslog 
    }
}

//------------------------------------------------------------------------------
/**
    Specifies Max log system to the log handler.
*/
void
nMaxLogHandler::SetLogSys(LogSys* logSys)
{
	if (logSys)
		this->log = logSys;
	n_printf("Nebula2: maxplugin loaded. %i messages lost because of missing buffer.\n", this->numMsgDismissed );
}
