//------------------------------------------------------------------------------
//  properties/graphicsproperty.h
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/graphicsproperty.h"
#include "appgame/appconfig.h"
#if __USE_PHYSICS__
//#include "graphicsutil/physicsgfxutil.h"
//#include "properties/physicsproperty.h"
#endif
#include "graphicsfeatureunit.h"
#include "graphicsutil/segmentedgfxutil.h"
#include "graphics/graphicsserver.h"
#include "graphics/stage.h"
#include "game/entity.h"
#include "basegameattr/basegameattributes.h"
#include "basegameprotocol.h"
#include "debugrender/debugrender.h"

namespace GraphicsFeature
{
__ImplementClass(GraphicsFeature::GraphicsProperty, 'GFXP', Game::Property);

using namespace Graphics;
using namespace Game;
using namespace Math;
using namespace Util;
using namespace BaseGameFeature;

//------------------------------------------------------------------------------
/**
*/
GraphicsProperty::GraphicsProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GraphicsProperty::~GraphicsProperty()
{
    n_assert(this->graphicsEntities.Size() == 0);
}

//------------------------------------------------------------------------------
/** 
*/
void
GraphicsProperty::SetupDefaultAttributes()
{
    Property::SetupDefaultAttributes();
    SetupAttr(Attr::Graphics);
}

//------------------------------------------------------------------------------
/** 
    Attach the property to a game entity. This will create and setup
    the required graphics entities.
*/
void
GraphicsProperty::OnActivate()
{
    Game::Property::OnActivate();
    this->SetupGraphicsEntities();
}

//------------------------------------------------------------------------------
/**    
    Remove the property from its game entity. This will release the
    graphics entities owned by the property.
*/
void
GraphicsProperty::OnDeactivate()
{
    const Ptr<Graphics::Stage>& stage = GraphicsFeatureUnit::Instance()->GetDefaultStage();

    // release graphics entities
    IndexT i;
    for (i = 0; i < this->graphicsEntities.Size(); i++)
    {
        stage->RemoveEntity(this->graphicsEntities[i].cast<GraphicsEntity>());
        this->graphicsEntities[i] = 0;
    }
    this->graphicsEntities.Clear();

    // up to parent class
    Property::OnDeactivate();
}

//------------------------------------------------------------------------------
/**    
*/
void
GraphicsProperty::SetupCallbacks()
{    
    this->entity->RegisterPropertyCallback(this, RenderDebug);
}

//------------------------------------------------------------------------------
/**    
*/
void
GraphicsProperty::OnRenderDebug()
{    
    String category(this->entity->GetCategory());
    String guidTxt(this->entity->GetGuid(Attr::Guid).AsString());
    category.Append(": ");
    category.Append(guidTxt);
    Math::point pos = this->GetEntity()->GetMatrix44(Attr::Transform).get_position();
    //_debug_text3D(category, pos, float4(0.5,1.0,0.5,1));
}

//------------------------------------------------------------------------------
/**    
    Setup the graphics entities. You may override this method in a subclass
    if different setup is needed.
*/
void
GraphicsProperty::SetupGraphicsEntities()
{
    // get some entity attributes
    const Util::String& resName =  this->GetGraphicsResource();
    const Ptr<Stage>& stage = GraphicsFeatureUnit::Instance()->GetDefaultStage();

#if __USE_PHYSICS__
    //// check if we have a physics property attached
    //PhysicsFeature::PhysicsProperty* physProperty = (PhysicsFeature::PhysicsProperty*) GetEntity()->FindProperty(PhysicsFeature::PhysicsProperty::RTTI).get();
    //if (physProperty && physProperty->IsEnabled())
    //{
    //    // setup graphics entities for visualizing physics
    //    Physics::PhysicsEntity* physEntity = physProperty->GetPhysicsEntity();
    //    if (physEntity)
    //    {
    //        this->graphicsEntities = Util::PhysicsGfxUtil::CreateGraphics(physEntity);
    //        n_assert(this->graphicsEntities.Size() > 0);
    //        Util::PhysicsGfxUtil::SetupGraphics(resName, physEntity, this->graphicsEntities);
    //        return;
    //    }
    //}
#endif

    // fallthrough: setup physics-less graphics entity
    const matrix44& worldMatrix = GetEntity()->GetMatrix44(Attr::Transform);
    SegmentedGfxUtil segGfxUtil;
    this->graphicsEntities = segGfxUtil.CreateAndSetupGraphicsEntities(resName, worldMatrix, stage);
}

//------------------------------------------------------------------------------
/**    
*/
void
GraphicsProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(BaseGameFeature::UpdateTransform::Id);
    this->RegisterMessage(GraphicsFeature::SetVisibleMsg::Id);
    this->RegisterMessage(GraphicsFeature::GetGraphicsEntities::Id);
    this->RegisterMessage(GraphicsFeature::SetOverwriteColor::Id);
    this->RegisterMessage(GraphicsFeature::SetShaderVariable::Id);

    Property::SetupAcceptedMessages();
}

//------------------------------------------------------------------------------
/**    
*/
void
GraphicsProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg);
    if (msg->CheckId(BaseGameFeature::UpdateTransform::Id))
    {
		this->UpdateTransform((msg.cast<BaseGameFeature::UpdateTransform>())->GetMatrix(), false);
    }
    else if (msg->CheckId(GraphicsFeature::SetVisibleMsg::Id))
    {
        this->SetVisible((msg.cast<SetVisibleMsg>())->GetVisible());
    }
    else if (msg->CheckId(GraphicsFeature::GetGraphicsEntities::Id))
    {
        (msg.cast<GetGraphicsEntities>())->SetEntities(this->graphicsEntities);
    }
    else if (msg->CheckId(GraphicsFeature::SetOverwriteColor::Id))
    {
        this->OnSetOverwriteColor(msg.cast<SetOverwriteColor>());
    }
    else if (msg->CheckId(GraphicsFeature::SetShaderVariable::Id))
    {
        this->OnSetShaderVariable(msg.cast<SetShaderVariable>());
    }
    else
    {
        Property::HandleMessage(msg);
    }
}
    
//------------------------------------------------------------------------------
/**    
*/
void
GraphicsProperty::OnSetOverwriteColor(const Ptr<SetOverwriteColor>& msg)
{
    n_assert(this->graphicsEntities.Size() > 0);    
    IndexT i;
    for (i = 0; i < this->graphicsEntities.Size(); i++)
    {
        this->graphicsEntities[i]->UpdateModelNodeInstanceShaderVariable(msg->GetNodeName(), "MaterialColor", Util::Variant(msg->GetColor()));
    }
}

//------------------------------------------------------------------------------
/**    
*/
void
GraphicsProperty::OnSetShaderVariable(const Ptr<SetShaderVariable>& msg)
{
    n_assert(this->graphicsEntities.Size() > 0);    
    IndexT i;
    for (i = 0; i < this->graphicsEntities.Size(); i++)
    {
        this->graphicsEntities[i]->UpdateModelNodeInstanceShaderVariable(msg->GetNodeName(), msg->GetShaderVarName(), msg->GetValue());
    }
}

//------------------------------------------------------------------------------
/**    
    Called to update the graphics entity's transform.
*/
void
GraphicsProperty::UpdateTransform(const matrix44& m, bool setDirectly)
{
    // TODO: DO NOT SEARCH FOR A ATTACHED PHYSICS PROPERTY!
#if __USE_PHYSICS__    
   /* // if physics property exists, gather transforms from physics property,
    // otherwise use the transform in the UpdateTransform message
    PhysicsProperty* physProperty = (PhysicsProperty*) GetEntity()->FindProperty(PhysicsProperty::RTTI).get();
    if (!setDirectly && (0 != physProperty))
    {
        // gather transform from physics entity
        if (physProperty->IsEnabled())
        {
            Physics::PhysicsEntity* physEntity = physProperty->GetPhysicsEntity();
            if (physEntity)
            {
                Util::PhysicsGfxUtil::UpdateGraphicsTransforms(physEntity, this->graphicsEntities);
            }
        }
        
    }
    else*/
#endif
    {
        // set transformation directly
        IndexT i;
        for (i = 0; i < this->graphicsEntities.Size(); i++)
        {
            this->graphicsEntities[i]->SetTransform(m);
        }
    }
}

//------------------------------------------------------------------------------
/**    
    Shows or hides all attached graphics entities. 
*/
void 
GraphicsProperty::SetVisible(bool visible)
{
    if (visible)
    {
        // show
        IndexT i;
        for (i = 0; i < this->graphicsEntities.Size(); i++)
        {
            this->graphicsEntities[i]->SetVisible(true);
        }
    }
    else
    {
        // hide
        IndexT i;
        for (i = 0; i < this->graphicsEntities.Size(); i++)
        {
            this->graphicsEntities[i]->SetVisible(false);
        }
    }
}

}; // namespace GraphicsFeature
