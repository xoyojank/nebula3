#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::VertexLayoutServer    
    
    The VertexLayoutServer creates VertexLayout objects shared by their
    vertex component signature. On some platforms it is more efficient
    to share VertexLayout objects across meshes with identical
    vertex structure. Note that there is no way to manually discard
    vertex components. Vertex components stay alive for the life time
    of the application until the Close() method of the VertexLayoutServer
    is called.
    
    (C) 2007 Radon Labs GmbH
*/    
#if (__WIN32__ || __XBOX360__ || __PS3__)
#include "coregraphics/base/vertexlayoutserverbase.h"
namespace CoreGraphics
{
class VertexLayoutServer : public Base::VertexLayoutServerBase
{
    __DeclareClass(VertexLayoutServer);
};
}
#elif __XBOX360__
#include "coregraphics/base/vertexlayoutserverbase.h"
namespace CoreGraphics
{
class VertexLayoutServer : public Base::VertexLayoutServerBase
{
    __DeclareClass(VertexLayoutServer);
};
}
#elif __WII__
#include "coregraphics/wii/wiivertexlayoutserver.h"
namespace CoreGraphics
{
class VertexLayoutServer : public Wii::WiiVertexLayoutServer
{
    __DeclareClass(VertexLayoutServer);
};
}
#else
#error "VertexLayoutServer class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------



    