#pragma once
//------------------------------------------------------------------------------
/** 
    @class Win32::Win32SkinnedMeshRenderer
  
    D3D9/Xbox360 implemention of SkinnedMeshRenderer.
    
    (C) 2008 Radon Labs GmbH
*/    
#include "characters/base/skinnedmeshrendererbase.h"
#include "util/stack.h"
#include "coregraphics/texture.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32SkinnedMeshRenderer : public Base::SkinnedMeshRendererBase
{
    __DeclareClass(Win32SkinnedMeshRenderer);
    __DeclareSingleton(Win32SkinnedMeshRenderer);
public:
    /// constructor
    Win32SkinnedMeshRenderer();
    /// destructor
    virtual ~Win32SkinnedMeshRenderer();

    /// get the skinning technique used by the renderer
    Characters::SkinningTechnique::Code GetSkinningTechnique() const;

private:
    friend class Characters::CharacterServer;

    /// draw a skinned mesh
    void DrawGPUSkinnedMesh(const Ptr<Characters::CharacterInstance>& charInst, const Ptr<CoreGraphics::Mesh>& mesh, IndexT primGroupIndex, const Util::Array<IndexT>& jointPalette, const Ptr<CoreGraphics::ShaderVariable>& jointPaletteShdVar);

    static const SizeT ConstantBufferMaxJoints = 72;
};

//------------------------------------------------------------------------------
/**
*/
inline Characters::SkinningTechnique::Code
Win32SkinnedMeshRenderer::GetSkinningTechnique() const
{
    return Characters::SkinningTechnique::GPUSkinning;
}

} // namespace Win32
//------------------------------------------------------------------------------
