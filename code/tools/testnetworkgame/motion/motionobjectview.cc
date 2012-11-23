//------------------------------------------------------------------------------
//  network/multiplayerfeature/MotionObjectView.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "testnetworkgame/motion/motionobjectview.h"
#include "multiplayerfeature/distributionmanager.h"
#include "util/bitfield.h"
#include "testnetworkgame/motion/distributemotionproperty.h"

namespace Tools
{
__ImplementClass(MotionObjectView, 'HOMV', MultiplayerFeature::ObjectView);

using namespace Attr;
using namespace IO;
using namespace Util;
using namespace Messaging;
using namespace MultiplayerFeature;

//------------------------------------------------------------------------------
/**
*/
MotionObjectView::MotionObjectView()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
MotionObjectView::~MotionObjectView()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Overwrite for interpolation, extrapolation etc.
*/
void 
MotionObjectView::ComputeCurrentValue(const Ptr<MultiplayerFeature::NetworkAttribute>&  networkAttr)
{       
    //if (networkAttr->GetLinkedAttributeId() == Attr::MovementPos)
    {
        Queue<NetworkAttribute::HistoryEntry>& history = networkAttr->GetHistory();
        NetworkAttribute::HistoryEntry lastEntry = history.Dequeue();
        Variant lastValue = lastEntry.value;
        Timing::Tick timeStamp = lastEntry.timeStamp;

        // TODO: compute correct value from timestamp
        // now apply the data
        networkAttr->ApplyValue(lastValue);
    }
    
}
} // namespace MultiplayerFeature
