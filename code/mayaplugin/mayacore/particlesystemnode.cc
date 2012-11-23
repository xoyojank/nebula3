//------------------------------------------------------------------------------
//  particlesystemnode.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayacore/particlesystemnode.h"

namespace Particles
{
__ImplementClass(Particles::ParticleSystemNode, 'PSND', Models::StateNode);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
ParticleSystemNode::ParticleSystemNode() :
    primGroupIndex(InvalidIndex)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ParticleSystemNode::~ParticleSystemNode()
{
}

//------------------------------------------------------------------------------
/**
*/
bool
ParticleSystemNode::WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer)
{    
	this->emitterAttrs.WriteDataTag(writer);
	return StateNode::WriteDataTag(writer);
}

} // namespace Particles
