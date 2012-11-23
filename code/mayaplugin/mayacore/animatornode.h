#pragma once
//------------------------------------------------------------------------------
/**
    @class AnimatorNode

    Legacy N2 crap!

    (C) 2008 RadonLabs GmbH
*/
#include "mayacore/modelnode.h"
#include "mayacore/animkeyarray.h"
#include "mayacore/animlooptype.h"
#include "util/variant.h"

//------------------------------------------------------------------------------
namespace Models
{
class AnimatorNode : public Models::ModelNode
{
    __DeclareClass(AnimatorNode);
public:
    enum NodeType
    {
        IntAnimator = 0,
        FloatAnimator,
        Float4Animator,
        TransformAnimator,
        TransformCurveAnimator,
        UvAnimator,

        InvalidNodeType
    };

    /// constructor
    AnimatorNode();
    /// destructor
    virtual ~AnimatorNode();

	// write data to stream
	virtual bool WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer);

private:

    struct AnimSection
    {
        int animationGroup;
        Util::String animationName;
        Util::Array<Util::String> animatedNodesPath;
        NodeType animationNodeType;

        AnimKeyArray<AnimKey<Math::vector> > posArray;
        AnimKeyArray<AnimKey<Math::vector> > eulerArray;
        AnimKeyArray<AnimKey<Math::vector> > scaleArray;
        Util::Array<int> layer;


        AnimKeyArray<AnimKey<Math::float4> > float4KeyArray;
        AnimKeyArray<AnimKey<float> > floatKeyArray;
        AnimKeyArray<AnimKey<int> > intKeyArray;

        AnimLoopType::Type loopType;
		Util::String paramName;

		bool WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer);
    };

    Util::Array<AnimSection> animSection;
};
__RegisterClass(AnimatorNode);

} // namespace Models
//------------------------------------------------------------------------------
