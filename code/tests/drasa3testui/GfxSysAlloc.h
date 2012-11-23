#pragma once
#include "GSysAlloc.h"

class GfxSysAlloc : public GSysAlloc
{
public:
	virtual void GetInfo(Info* i) const;
	virtual void* Alloc(UPInt size,UPInt align);
	virtual bool Free(void* ptr,UPInt size,UPInt align);
};
