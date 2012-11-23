//------------------------------------------------------------------------------
//  characterserver.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/characterserver.h"

namespace Characters
{
__ImplementClass(Characters::CharacterServer, 'CHRS', Core::RefCounted);
__ImplementSingleton(Characters::CharacterServer);

using namespace Util;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
CharacterServer::CharacterServer() :
    curFrameIndex(InvalidIndex),
    isValid(false),
    inFrame(false),
    inGather(false),
    inDraw(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
CharacterServer::~CharacterServer()
{
    n_assert(!this->isValid);
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterServer::Setup()
{
    n_assert(!this->isValid);
    this->isValid = true;
    this->skinnedMeshRenderer = SkinnedMeshRenderer::Create();
    this->skinnedMeshRenderer->Setup();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterServer::Discard()
{
    n_assert(this->isValid);
    n_assert(this->visCharInstArray.IsEmpty());
    this->skinnedMeshRenderer->Discard();
    this->skinnedMeshRenderer = 0;
    this->isValid = false;    
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterServer::BeginFrame(IndexT frameIndex)
{
    n_assert(this->isValid);
    n_assert(!this->inFrame);
    n_assert(this->curFrameIndex != frameIndex);
    n_assert(this->visCharInstArray.IsEmpty());

    this->inFrame = true;
    this->curFrameIndex = frameIndex;
    this->skinnedMeshRenderer->OnBeginFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterServer::EndFrame()
{
    n_assert(this->inFrame);
    this->inFrame = false;
    this->visCharInstArray.Clear();
    this->skinnedMeshRenderer->OnEndFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterServer::BeginGather()
{
    n_assert(this->inFrame);
    n_assert(!this->inGather);
    n_assert(this->visCharInstArray.IsEmpty());

    this->inGather = true;
    this->skinnedMeshRenderer->BeginGatherSkins();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterServer::GatherVisibleCharacter(const Ptr<CharacterInstance>& charInst, Timing::Time time)
{
    n_assert(this->inGather);
        
    // RegisterVisibleCharacter may be called several times per frame with the same
    // character instance, make sure we add the character instance only once!
    Timing::Tick ticks = Timing::SecondsToTicks(time);
    if (charInst->PrepareUpdate(ticks, this->curFrameIndex))
    {
        this->visCharInstArray.Append(charInst);
    }
}

//------------------------------------------------------------------------------
/**
*/
SkinnedMeshRenderer::DrawHandle
CharacterServer::GatherSkinMesh(const Ptr<CharacterInstance>& charInst, const Ptr<CoreGraphics::Mesh>& srcMesh)
{
    n_assert(this->inGather);
    n_assert(this->skinnedMeshRenderer->GetSkinningTechnique() == SkinningTechnique::SoftwareSkinning);
    return this->skinnedMeshRenderer->RegisterSoftwareSkinnedMesh(charInst, srcMesh);
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterServer::EndGather()
{
    n_assert(this->inGather);
    this->inGather = false;
    this->skinnedMeshRenderer->EndGatherSkins();
}

//------------------------------------------------------------------------------
/**
    Start updating character skeletons. This is an asynchronous operation!        
*/
void
CharacterServer::StartUpdateCharacterSkeletons()
{
    n_assert(!this->inGather)
    IndexT i;
    SizeT num = this->visCharInstArray.Size();
    for (i = 0; i < num; i++)
    {
        this->visCharInstArray[i]->StartUpdate();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterServer::UpdateCharacterSkins()
{
    n_assert(!this->inGather);
    if (this->skinnedMeshRenderer->GetSkinningTechnique() == SkinningTechnique::SoftwareSkinning)
    {
        this->skinnedMeshRenderer->UpdateSoftwareSkinnedMeshes();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterServer::BeginDraw()
{
    n_assert(this->isValid);
    n_assert(!this->inDraw);
    this->inDraw = true;
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterServer::DrawSoftwareSkinnedMesh(SkinnedMeshRenderer::DrawHandle h, IndexT primGroupIndex)
{
    n_assert(this->inDraw);
    n_assert(this->skinnedMeshRenderer->GetSkinningTechnique() == SkinningTechnique::SoftwareSkinning);
    this->skinnedMeshRenderer->DrawSoftwareSkinnedMesh(h, primGroupIndex);
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterServer::DrawGPUSkinnedMesh(const Ptr<CharacterInstance>& charInst, const Ptr<Mesh>& mesh, IndexT primGroupIndex, const Array<IndexT>& jointPalette, const Ptr<ShaderVariable>& jointPaletteShdVar)
{
    n_assert(this->inDraw);
    n_assert(this->skinnedMeshRenderer->GetSkinningTechnique() == SkinningTechnique::GPUSkinning);
    this->skinnedMeshRenderer->DrawGPUSkinnedMesh(charInst, mesh, primGroupIndex, jointPalette, jointPaletteShdVar);
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterServer::DrawGPUTextureSkinnedMesh(const Ptr<CharacterInstance>& charInst, const Ptr<Mesh>& mesh, IndexT primGroupIndex, const Ptr<ShaderVariable>& charInstShaderVar)
{
    n_assert(this->inDraw);
    n_assert(this->skinnedMeshRenderer->GetSkinningTechnique() == SkinningTechnique::GPUTextureSkinning);
    this->skinnedMeshRenderer->DrawGPUTextureSkinnedMesh(charInst, mesh, primGroupIndex, charInstShaderVar);
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterServer::EndDraw()
{
    n_assert(this->inDraw);
    this->inDraw = false;
}

} // namespace Characters