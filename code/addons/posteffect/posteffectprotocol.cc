//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "posteffectprotocol.h"

namespace PostEffect
{
    __ImplementClass(PostEffect::SetupPostEffectSystem, 'sepe', Messaging::Message);
    __ImplementMsgId(SetupPostEffectSystem);
    __ImplementClass(PostEffect::ResetPostEffectSystem, 'rpse', Messaging::Message);
    __ImplementMsgId(ResetPostEffectSystem);
    __ImplementClass(PostEffect::PreloadTexture, 'pret', Messaging::Message);
    __ImplementMsgId(PreloadTexture);
    __ImplementClass(PostEffect::UnloadTexture, 'unlt', Messaging::Message);
    __ImplementMsgId(UnloadTexture);
    __ImplementClass(PostEffect::BlendBase, 'faba', Messaging::Message);
    __ImplementMsgId(BlendBase);
    __ImplementClass(PostEffect::FadeOut, 'faou', BlendBase);
    __ImplementMsgId(FadeOut);
    __ImplementClass(PostEffect::FadeIn, 'fain', BlendBase);
    __ImplementMsgId(FadeIn);
    __ImplementClass(PostEffect::BlendColor, 'setc', BlendBase);
    __ImplementMsgId(BlendColor);
    __ImplementClass(PostEffect::BlendDepthOfField, 'setd', BlendBase);
    __ImplementMsgId(BlendDepthOfField);
    __ImplementClass(PostEffect::BlendFog, 'setf', BlendBase);
    __ImplementMsgId(BlendFog);
    __ImplementClass(PostEffect::BlendHdr, 'seth', BlendBase);
    __ImplementMsgId(BlendHdr);
    __ImplementClass(PostEffect::BlendLight, 'setl', BlendBase);
    __ImplementMsgId(BlendLight);
    __ImplementClass(PostEffect::BlendSky, 'sets', BlendBase);
    __ImplementMsgId(BlendSky);
} // PostEffect

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
