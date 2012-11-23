//------------------------------------------------------------------------------
//  emitterattrs.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayacore/emitterattrs.h"

namespace Particles
{

//------------------------------------------------------------------------------
/**
*/
EmitterAttrs::EmitterAttrs()
{
    Memory::Clear(this->floatValues, sizeof(this->floatValues));
    Memory::Clear(this->intValues, sizeof(this->intValues));
    Memory::Clear(this->boolValues, sizeof(this->boolValues));
}

//------------------------------------------------------------------------------
/**
*/
bool 
EmitterAttrs::WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer)
{
	for (IndexT index = 0 ; index < NumEnvelopeAttrs; index++)
	{
		writer->BeginTag(ToString((EnvelopeAttr)index), ToFourCC((EnvelopeAttr)index));
		this->envelopeValues[index].WriteDataTag(writer);
		writer->EndTag();
	}	

	for (IndexT index = 0; index < NumIntAttrs; index++ )
	{
		writer->BeginTag(ToString((IntAttr)index), ToFourCC((IntAttr)index));
		writer->WriteInt(this->intValues[index]);
		writer->EndTag();
	}

	for (IndexT index = 0; index < NumBoolAttrs;index++)
	{
		writer->BeginTag(ToString((BoolAttr)index), ToFourCC((BoolAttr)index));
		writer->WriteBool(this->boolValues[index]);
		writer->EndTag();
	}

	for (IndexT index = 0; index < NumFloatAttrs;index++)
	{
		writer->BeginTag(ToString((FloatAttr)index), ToFourCC((FloatAttr)index));
		writer->WriteFloat(this->floatValues[index]);
		writer->EndTag();
	}
	return true;
}

//------------------------------------------------------------------------------
/**
*/
Util::String 
EmitterAttrs::ToString(EnvelopeAttr key)
{
	return "";
}

//------------------------------------------------------------------------------
/**
*/
Util::String 
EmitterAttrs::ToString(IntAttr key)
{
	return "";
}

//------------------------------------------------------------------------------
/**
*/
Util::String 
EmitterAttrs::ToString(BoolAttr key)
{
	return "";
}

//------------------------------------------------------------------------------
/**
*/
Util::String 
EmitterAttrs::ToString(FloatAttr key)
{
	return "";
}

//------------------------------------------------------------------------------
/**
*/
Util::FourCC 
EmitterAttrs::ToFourCC(EnvelopeAttr key)
{
	Util::FourCC cc;
	return cc;
}

//------------------------------------------------------------------------------
/**
*/
Util::FourCC 
EmitterAttrs::ToFourCC(IntAttr key)
{
	Util::FourCC cc;
	return cc;
}

//------------------------------------------------------------------------------
/**
*/
Util::FourCC 
EmitterAttrs::ToFourCC(BoolAttr key)
{
	Util::FourCC cc;
	return cc;
}

//------------------------------------------------------------------------------
/**
*/
Util::FourCC 
EmitterAttrs::ToFourCC(FloatAttr key)
{
	Util::FourCC cc;
	return cc;
}

} // namespace Particles