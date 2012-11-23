//------------------------------------------------------------------------------
//  treebillboardgraphicsentity.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "forest/treebillboardgraphicsentity.h"
#include "forest/treebillboardrenderer.h"
#include "internalgraphics/internalgraphicsentitytype.h"

namespace Forest
{
__ImplementClass(Forest::TreeBillboardGraphicsEntity, 'TBGE', InternalGraphics::InternalGraphicsEntity);

using namespace InternalGraphics;

//------------------------------------------------------------------------------
/**
*/
TreeBillboardGraphicsEntity::TreeBillboardGraphicsEntity()
{
    this->SetType(InternalGraphicsEntityType::Model);
}

//------------------------------------------------------------------------------
/**
*/
TreeBillboardGraphicsEntity::~TreeBillboardGraphicsEntity()
{
}

//------------------------------------------------------------------------------
/**
    Need to override the Graphics entity's activation.
*/
void
TreeBillboardGraphicsEntity::OnActivate()
{
//    n_assert(!this->active);

    /*
    // normally happens in parent class
    MGraphics::Server::Instance()->GetEntityTypeLod(this->GetRtti(), this->minVisibleDistance, this->maxVisibleDistance, this->minScreenSize);
    this->ResetActivationTime();
    this->shadowBoxValid = false;
    this->active = true;

    // needs always to be "rendered"
    this->SetLocalBox(MGraphics::Server::Instance()->GetLevel()->GetRootCell()->GetBox());
    */
    InternalGraphicsEntity::OnActivate();
}

//------------------------------------------------------------------------------
/**
*/
void
TreeBillboardGraphicsEntity::OnDeactivate()
{
    InternalGraphicsEntity::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
TreeBillboardGraphicsEntity::Render()
{
       /*
    if (this->GetVisible())
    {
        // we need to communicate the directional light to 
        // the billboard renderer
        nRenderContext* dirLightRenderContext = 0;
        SizeT num = this->GetNumLinks(LightLink);
        IndexT i;
        for (i = 0; i < num; i++)
        {
            MGraphics::Entity* entity = this->GetLinkAt(LightLink, i);
            if (entity->IsInstanceOf(MGraphics::LightEntity::RTTI))
            {
                MGraphics::LightEntity* lightEntity = (MGraphics::LightEntity*) entity;
                if (lightEntity->GetLight().GetType() == nLight::Directional)
                {
                    dirLightRenderContext = &(lightEntity->GetRenderContext());
                    break;
                }
            }
        }
        
        // must also communicate a 0 pointer!
        TreeBillboardRenderer::Instance()->UpdateLightLink(dirLightRenderContext);
    }
    */
}

} // namespace Forest
