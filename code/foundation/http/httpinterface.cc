//------------------------------------------------------------------------------
//  httpinterface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/httpinterface.h"
#include "http/httpmessagehandler.h"
#include "messaging/runthroughhandlerthread.h"

namespace Http
{
__ImplementClass(Http::HttpInterface, 'HTIF', Interface::InterfaceBase);
__ImplementInterfaceSingleton(Http::HttpInterface);

using namespace Interface;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
HttpInterface::HttpInterface()
{
    __ConstructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
*/
HttpInterface::~HttpInterface()
{
    __DestructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
HttpInterface::Open()
{
    // setup runthrough handler thread
    Ptr<RunThroughHandlerThread> handlerThread = RunThroughHandlerThread::Create();
    handlerThread->SetName("HttpInterface Thread");
    handlerThread->SetCoreId(System::Cpu::MiscThreadCore);
#if __WII_
    handlerThread->SetPriority(Thread::NormalBoost);
#endif    
    handlerThread->AttachHandler(HttpMessageHandler::Create());
    this->SetHandlerThread(handlerThread.cast<HandlerThreadBase>());

    InterfaceBase::Open();
}

} // namespace Http
