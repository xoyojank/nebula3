#pragma once
//------------------------------------------------------------------------------
/**
    @class Models::StateNode
  
    A model node which holds shader state information and applies shared shader
    state for its set of StateNodeInstances.
    
    (C) 2007 Radon Labs GmbH
*/
#include "mayacore/transformnode.h"
#include "util/variant.h"

//------------------------------------------------------------------------------
namespace Models
{
class StateNode : public TransformNode
{
    __DeclareClass(StateNode);
public:
    /// constructor
    StateNode();
    /// destructor
    virtual ~StateNode();

	// write data to stream
	virtual bool WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer);
	/// extract a single node and recurse
	virtual MStatus ExtractNode(const MDagPath& nodePath);

    /// add optional shader parameter, must be called before LoadResources
    void AddShaderParam(const Util::String& paramName, const Util::Variant& paramValue);
	/// set shader resource id
	void SetShader(const Util::String name);
private:
	/// shader param
    Util::Array<Util::KeyValuePair<Util::StringAtom, Util::Variant> > shaderParams;
	/// shader name
	Util::String shaderName;
};

//------------------------------------------------------------------------------
/**
*/
inline void
StateNode::SetShader(const Util::String name)
{
	this->shaderName = name;
}
} // namespace Models
//------------------------------------------------------------------------------
