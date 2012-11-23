//------------------------------------------------------------------------------
//  animationremoterequesthandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/coreserver.h"
#include "tools/testremoteviewer/animationremoterequesthandler.h"
#include "timing/calendartime.h"
#include "remote/remotecontrol.h"
#include "io/xmlReader.h"
// for animation tests
#include "tools/testremoteviewer/testremoteviewerapplication.h"

namespace Remote
{
__ImplementClass(Remote::AnimationRemoteRequestHandler, 'ARRH', Remote::RemoteRequestHandler);

using namespace Core;
using namespace Util;
using namespace Timing;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
AnimationRemoteRequestHandler::AnimationRemoteRequestHandler() 
{
    this->controllerName = "animation";
    this->name = "animation request handler";
    this->desc = "Handler for animation requests.";
}
//------------------------------------------------------------------------------
/**
*/
void
AnimationRemoteRequestHandler::HandleRequest(const Ptr<RemoteRequest>& request)
{
    Ptr<XmlReader> xmlReader = request->GetXmlReader();

    if (xmlReader->Open())
    {
        Util::String rootNodeName = xmlReader->GetCurrentNodeName();
    
        // test if valid remote control xml chunk
        if ("RemoteControl" == rootNodeName)
        {
            // child nodes 
            if (xmlReader->SetToFirstChild())
            {
                do
                {
                    Util::String childName = xmlReader->GetCurrentNodeName();
                    if ("Command" == childName)
                    {
                        // get attribttes
                        Util::String attribute = xmlReader->GetString("name");
                        if ("show" == attribute)
                        {
                            Tools::TestRemoteViewerApplication *app = (Tools::TestRemoteViewerApplication*)
                                Tools::TestRemoteViewerApplication::Instance();
                            Ptr<Graphics::ModelEntity> model = app->GetModel();
                            attribute = xmlReader->GetString("value");
                            if ("false" == attribute)
                            {
                                model->SetVisible(false);
                            }
                            else
                            {
                                model->SetVisible(true);
                            }
                        }
                    }
                    else if ("Request" == childName)
                    {
                    }
                    else if ("Reply" == childName)
                    {
                    }
                }
                while (xmlReader->SetToNextChild());
            }
        }
    }
}

} // namespace Remote