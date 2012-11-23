//------------------------------------------------------------------------------
//  mesh.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/mesh.h"
#if (__WIN32__ || __XBOX360__)
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::Mesh, 'MESH', Base::MeshBase);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::Mesh, 'MESH', Wii::WiiMesh);
}
#elif __PS3__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::Mesh, 'MESH', Base::MeshBase);
}
#else
#error "Mesh class not implemented on this platform!"
#endif
