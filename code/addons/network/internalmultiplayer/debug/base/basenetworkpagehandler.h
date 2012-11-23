#pragma once
//------------------------------------------------------------------------------
/**
    @class Debug::NetworkPageHandler
    
    Provide information about the network transfer rates.


    (C) 2007 Radon Labs GmbH
*/
#include "debug/debugpagehandler.h"

//------------------------------------------------------------------------------
namespace Debug
{
class NetworkPageHandler : public Debug::DebugPageHandler
{
    __DeclareClass(NetworkPageHandler);
public:
    /// constructor
    NetworkPageHandler();
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual void HandleRequest(const Ptr<Http::HttpRequest>& request);

private: 
    /// handle HTTP request to render a counter char
    void HandleCounterChartRequest(const Util::String& counterName, const Ptr<Http::HttpRequest>& request);

    
};

} // namespace Debug
//------------------------------------------------------------------------------  