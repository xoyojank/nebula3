//-----------------------------------------------------------------------------
//  nmaxtransformanimator_main.cc
//
//  (c)2005 Kim, Hyoun Woo.
//-----------------------------------------------------------------------------
#include "export2/nmax.h"
#include "export2/nmaxinterface.h"
#include "export2/nmaxoptions.h"
#include "export2/nmaxcontrol.h"
#include "scene/ntransformanimator.h"
#include "export2/nMaxtransformanimator.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

#include "kernel/ntypes.h"

//-----------------------------------------------------------------------------
/**
*/
nMaxTransformAnimator::nMaxTransformAnimator() :
    maxNode(0)
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxTransformAnimator::~nMaxTransformAnimator()
{
}

//-----------------------------------------------------------------------------
/**
*/
bool nMaxTransformAnimator::Export(INode* inode, Control *control)
{
    n_assert(inode);
    n_assert(control);

    this->maxNode = inode;

    Control *posControl, *rotControl, *scaleControl;
    posControl   = control->GetPositionController();
    rotControl   = control->GetRotationController();
    scaleControl = control->GetScaleController();

    nString animatorName;
    animatorName += inode->GetName();
    animatorName += "animator";
    animatorName.ToLower();

    // create nTransformAnimator.
    nTransformAnimator* animator;
    animator = static_cast<nTransformAnimator*>(CreateNebulaObject("ntransformanimator", 
                                                                   animatorName.Get()));
    if (animator)
    {
        int numKeys = 0;

        if (posControl)
        {
            numKeys += ExportPosition(posControl, animator);
            n_maxlog(High, "A position animation of the node: %s Num keys: %d", inode->GetName(), numKeys);
        }

        if (rotControl)
        {
            numKeys += ExportRotation(rotControl, animator);
            n_maxlog(High, "A rotation animation of the node: %s Num keys: %d", inode->GetName(), numKeys);
        }

        if (scaleControl)
        {
            numKeys += ExportScale(scaleControl, animator);
            n_maxlog(High, "A scale animation of the node: %s Num keys: %d", inode->GetName(), numKeys);
        }

        if (numKeys)
        {
            // specifies the created animator to the parent node.
            nSceneNode* parent = static_cast<nSceneNode*>(animator->GetParent());
            parent->AddAnimator(animator->GetName());
        }
        else
        {
            // no keys are exported, so release created transform animator and pop cwd.
            nKernelServer::Instance()->PopCwd();
            animator->Release();

            return false;
        }
    }
    else
    {
        animator->Release();
        n_maxlog(Error, "Error: Failed to create Nebula object for the node '%s'.", inode->GetName());
        return false;
    }

    return true;
}
