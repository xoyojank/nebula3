//------------------------------------------------------------------------------
//  RakNetNetworkPageHandler.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/debug/raknet/raknetnetworkpagehandler.h"
#include "http/html/htmlpagewriter.h"
#include "internalmultiplayer/internalmultiplayerserver.h"  
#include "http/svg/svglinechartwriter.h"
#include "debug/debugserver.h"
#include "internalmultiplayer/internalsession.h"
#include "internalmultiplayer/internalplayer.h"

namespace RakNet
{
__ImplementClass(RakNet::RakNetNetworkPageHandler, 'RNPH', Debug::DebugPageHandler);

using namespace Http;
using namespace Util;
using namespace IO;
using namespace InternalMultiplayer;
using namespace Debug;

//------------------------------------------------------------------------------
/**
*/
RakNetNetworkPageHandler::RakNetNetworkPageHandler()
{
    this->SetName("Network");
    this->SetDesc("show network debug information");
    this->SetRootLocation("network");
}

//------------------------------------------------------------------------------
/**
*/
void
RakNetNetworkPageHandler::HandleRequest(const Ptr<Http::HttpRequest>& request)
{
    n_assert(HttpMethod::Get == request->GetMethod());

    Dictionary<String,String> query = request->GetURI().ParseQuery();
    if (query.Contains("counterChart"))
    {
        this->HandleCounterChartRequest(query["counterChart"], request);
        return;
    }

    // configure a HTML page writer
    Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
    htmlWriter->SetStream(request->GetResponseContentStream());
    htmlWriter->SetTitle("Nebula3 Network Info");
    if (htmlWriter->Open())
    {
        String str;
        htmlWriter->Element(HtmlElement::Heading1, "Network");
        htmlWriter->AddAttr("href", "/index.html");
        htmlWriter->Element(HtmlElement::Anchor, "Home");        

        // list all statistics variables
        InternalMultiplayer::InternalMultiplayerServer::Instance()->WriteStatistics(htmlWriter);

        htmlWriter->Close();
        request->SetStatus(HttpStatus::OK);        
    }
    else
    {
        request->SetStatus(HttpStatus::InternalServerError);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
RakNetNetworkPageHandler::HandleCounterChartRequest(const Util::String& counterName, const Ptr<Http::HttpRequest>& request)
{
#if NEBULA3_ENABLE_PROFILING
    const Ptr<InternalSession>& session = InternalMultiplayerServer::Instance()->GetSession();
    const Util::Array< Ptr<InternalPlayer> > player = session->GetPlayers(true);
        
    Util::Array<Util::String> tokens = counterName.Tokenize("_");
    n_assert(tokens.Size() == 2);
    IndexT playerIndex = (IndexT)tokens[0].AsInt();
    if (player.Size() > playerIndex)
    {
        // setup a SVG line chart writer
        Util::Dictionary<Util::StringAtom, InternalMultiplayer::InternalPlayer::StatisticValue>& counterSet = player[playerIndex]->GetDebugCounterSet();            
        if (counterSet.Contains(tokens[1]))
        {
            const InternalMultiplayer::UniquePlayerId& uniqueId = player[playerIndex]->GetUnqiueId();
            const RakNetGUID& guid = uniqueId.GetRaknetGuid();
            const SystemAddress& systemAddress = InternalMultiplayerServer::Instance()->GetRakPeerInterface()->GetSystemAddressFromGuid(guid); 
            Util::String addressAsString(systemAddress.ToString(false));
          
            Ptr<SvgLineChartWriter> writer = SvgLineChartWriter::Create();
            writer->SetStream(request->GetResponseContentStream());
            writer->SetCanvasDimensions(1024, 256);
            if (writer->Open())
            {
                writer->Text(0, 250, counterSet[tokens[1]].GetDescription()); 
                // get min/max/avg values, convert int to float array
                // FIXME: SvgLineChartWriter should also accept integer tracks!
                Array<int> intArray = counterSet[tokens[1]].ValuesAsArray();
                Array<Timing::Tick> timeArray = counterSet[tokens[1]].TimeStampsAsArray();
                int minVal, maxVal, avgVal;
                this->ComputeMinMaxAvgCounts(intArray, minVal, maxVal, avgVal);

                // for maxVal == minVal set valid maxVal
                if (minVal == maxVal) maxVal = minVal + 1;

                Array<float> floatArray(intArray.Size(), 0);
                IndexT i;
                for (i = 0; i < intArray.Size(); i++)
                {
                    floatArray.Append(float(intArray[i]));
                }

                // setup the svg chart writer and draw the chart
                writer->SetupXAxis("frames", "frames", -int(floatArray.Size()), 0);
                writer->SetupYAxis("samples", "samples", 0.0f, float(maxVal));
                writer->AddTrack("samples", "red", floatArray, timeArray);
                writer->Draw();
                writer->Close();
            }
            request->SetStatus(HttpStatus::OK);
        }
        else
        {
            request->SetStatus(HttpStatus::NotFound);
        }
    }
#endif
}

} // namespace Debug
