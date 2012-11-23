#pragma once
//------------------------------------------------------------------------------
/**
    @class Forest::TreeClusterGraphicsEntity

    A subclass of graphics entity for linking a tree cluster into the
    graphics subsystem.

    (C) 2008 Radon Labs GmbH
*/
#include "internalgraphics/internalmodelentity.h"
#include "forest/treecluster.h"

//------------------------------------------------------------------------------
namespace Forest
{
class TreeClusterGraphicsEntity : public InternalGraphics::InternalGraphicsEntity
{
    __DeclareClass(TreeClusterGraphicsEntity);
public:
    /// constructor
    TreeClusterGraphicsEntity();
    /// destructor
    virtual ~TreeClusterGraphicsEntity();
    /// set the tree cluster associated with this entity
    void SetTreeCluster(const Ptr<TreeCluster>& treeCluster);
    /// get the tree cluster associated with this entity
    const Ptr<TreeCluster>& GetTreeCluster() const;
    /// clear the tree cluster pointer
    void ClearTreeCluster();

    /// called when attached to level
    virtual void OnActivate();
    /// called when removed from level
    virtual void OnDeactivate();

protected:
    /// called to setup the client-portion of the shared data object
    virtual void OnSetupSharedData();
    /// called to discard the client-portion of the shared data object
    virtual void OnDiscardSharedData();
    /// called per frame to reset the shared data object
    virtual void OnResetSharedData();

    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// called when the entity has been found visible, may be called several times per frame!
    //virtual void OnNotifyCullingVisible(const Ptr<InternalGraphicsEntity>& observer, IndexT frameIndex);
    /// called from Render method in internalview
    virtual void OnResolveVisibility();
    /// called right before rendering
    virtual void OnRenderBefore(IndexT frameIndex);
    /// called to render a debug visualization of the entity
    virtual void OnRenderDebug();

private:
    Ptr<TreeCluster> treeCluster;
};

}
//------------------------------------------------------------------------------
