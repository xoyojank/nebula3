#pragma once
//------------------------------------------------------------------------------
/**
    @class SUI::SUILayout

    Wrapper class for platform specific SUI layout classes.

    (C) 2009 Radon Labs GmbH
*/
#if (__WIN32__||__XBOX360__)
#include "sui/base/suilayoutbase.h"
namespace SUI
{
class SUILayout : public Base::SUILayoutBase
{
    __DeclareClass(SUILayout);
};
}
#elif __WII__
#include "sui/wii/wiisuilayout.h"
namespace SUI
{
class SUILayout : public Wii::WiiSUILayout
{
    __DeclareClass(SUILayout);
};
}
#else
#error "SUI::SUILayout not implemented on this platform"
#endif