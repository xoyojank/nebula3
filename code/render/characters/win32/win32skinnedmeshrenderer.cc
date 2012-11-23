//------------------------------------------------------------------------------
//  Win32skinnedmeshrenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/win32/win32skinnedmeshrenderer.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/displaydevice.h"

namespace Win32
{
__ImplementClass(Win32::Win32SkinnedMeshRenderer, 'WSMR', Base::SkinnedMeshRendererBase);
__ImplementSingleton(Win32::Win32SkinnedMeshRenderer);

using namespace Base;
using namespace Math;
using namespace Util;
using namespace Characters;
using namespace CoreGraphics;
using namespace Win360;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
Win32SkinnedMeshRenderer::Win32SkinnedMeshRenderer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
Win32SkinnedMeshRenderer::~Win32SkinnedMeshRenderer()
{
    __DestructSingleton;    
}

//------------------------------------------------------------------------------
/**
Draw Call of gpu skinning with vertex shader constants
*/
void
Win32SkinnedMeshRenderer::DrawGPUSkinnedMesh(const Ptr<CharacterInstance>& charInst,
                                               const Ptr<Mesh>& mesh,
                                               IndexT primGroupIndex,
                                               const Array<IndexT>& jointPalette,
                                               const Ptr<ShaderVariable>& jointPaletteShdVar)
{
    if (charInst->IsValidForRendering())
    {
        const int maxJointPaletteSize = ConstantBufferMaxJoints;
        matrix44 jointArray[maxJointPaletteSize];

        // make sure the character update has finished
        charInst->WaitUpdateDone();

        // extract joint palette skin matrices from skeleton
        const FixedArray<matrix44>& skinMatrixArray = charInst->Skeleton().GetSkinMatrixArray();
        IndexT i;
        SizeT numJointsInPalette = jointPalette.Size();
        n_assert(numJointsInPalette <= maxJointPaletteSize);
        for (i = 0; i < numJointsInPalette; i++)
        {
            jointArray[i] = skinMatrixArray[jointPalette[i]];
        }

        // commit skin matrices into joint palette shader variable
        jointPaletteShdVar->SetMatrixArray(jointArray, numJointsInPalette);
        ShaderServer::Instance()->GetActiveShaderInstance()->Commit();

        // apply mesh data and draw skinned primitives
        mesh->ApplyPrimitives(primGroupIndex);
        RenderDevice::Instance()->Draw();
    }
}

} // namespace Characters