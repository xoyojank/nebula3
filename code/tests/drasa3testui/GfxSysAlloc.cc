#include "stdneb.h"
#include "GfxSysAlloc.h"
#include "memory/memory.h"

using namespace Memory;

void GfxSysAlloc::GetInfo(Info* i) const
{
	i->MinAlign = 16;
	i->MaxAlign = 16;
	i->Granularity = 4 * 1024 * 1024;
	i->HasRealloc = false;
}

void* GfxSysAlloc::Alloc(UPInt size,UPInt align)
{
	return Memory::Alloc(Memory::ScaleformHeap,size);
}

bool GfxSysAlloc::Free(void* ptr,UPInt size,UPInt align)
{
	Memory::Free(Memory::ScaleformHeap,ptr);
	return true;
}