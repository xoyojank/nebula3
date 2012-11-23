//------------------------------------------------------------------------------
//  properties/timeproperty.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/timeproperty.h"
//#include "basegametiming/gametimesource.h"
#include "game/entity.h"
#include "basegameattr/basegameattributes.h"

namespace BaseGameFeature
{
__ImplementClass(BaseGameFeature::TimeProperty, 'TIMP', Game::Property);

using namespace Game;

//------------------------------------------------------------------------------
/**
*/
TimeProperty::TimeProperty() :
    attachTime(0.0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TimeProperty::~TimeProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
TimeProperty::SetupCallbacks()
{
    this->entity->RegisterPropertyCallback(this, BeginFrame);
}

//------------------------------------------------------------------------------
/**
*/
void
TimeProperty::SetupDefaultAttributes()
{
    Property::SetupDefaultAttributes();
    SetupAttr(Attr::Time);
}

//------------------------------------------------------------------------------
/**
*/
void
TimeProperty::OnActivate()
{
    Property::OnActivate();
    //this->attachTime = GameTimeSource::Instance()->GetTime();
}

//------------------------------------------------------------------------------
/**
*/
void
TimeProperty::OnBeginFrame()
{
    //Time absTime = GameTimeSource::Instance()->GetTime();
    //Time relTime = absTime - this->attachTime;
    //this->GetEntity()->SetFloat(Attr::Time, float(relTime));
}

}; // namespace Properties
