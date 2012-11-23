#pragma once
#ifndef HTTP_HTTPINTERFACE_H
#define HTTP_HTTPINTERFACE_H
//------------------------------------------------------------------------------
/**
    @class Http::HttpInterface
    
    The HttpInterface launches the HttpServer thread and is the communication
    interface with the HttpServer thread.
    
    (C) 2008 Radon Labs GmbH
*/
#include "interface/interfacebase.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace Http
{
class HttpInterface : public Interface::InterfaceBase
{
    __DeclareClass(HttpInterface);
    __DeclareInterfaceSingleton(HttpInterface);
public:
    /// constructor
    HttpInterface();
    /// destructor
    virtual ~HttpInterface();
    /// open the interface object
    virtual void Open();
};
        
} // namespace HttpInterface
//------------------------------------------------------------------------------
#endif
    
    