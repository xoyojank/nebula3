//------------------------------------------------------------------------------
//  suilayout.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "sui/rt/suilayout.h"

namespace SUI
{
#if (__WIN32__||__XBOX360__)
__ImplementClass(SUI::SUILayout, 'SUIL', Base::SUILayoutBase);
#elif __WII__
__ImplementClass(SUI::SUILayout, 'SUIL', Wii::WiiSUILayout);
#else
#error "SUI::SUILayout not implemented on this platform"
#endif
}