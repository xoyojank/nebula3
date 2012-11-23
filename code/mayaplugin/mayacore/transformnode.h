#pragma once
//------------------------------------------------------------------------------
/**
@class Models::TransformNode

Defines a transformation in a transform hierarchy.

(C) 2007 Radon Labs GmbH
*/
#include "mayacore/modelnode.h"
#include "math/point.h"
#include "math/vector.h"
#include "math/quaternion.h"

//------------------------------------------------------------------------------
namespace Models
{
	class TransformNode : public ModelNode
	{
		__DeclareClass(TransformNode);
	public:
		/// constructor
		TransformNode();
		/// destructor
		virtual ~TransformNode();

		// write data to stream
		virtual bool WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer);
		/// extract a single node and recurse
		virtual MStatus ExtractNode(const MDagPath& nodePath);

		/// set position
		void SetPosition(const Math::point& p);
		/// get position
		const Math::point& GetPosition() const;
		/// set rotate quaternion
		void SetRotation(const Math::quaternion& r);
		/// get rotate quaternion
		const Math::quaternion& GetRotation() const;
		/// set scale
		void SetScale(const Math::vector& s);
		/// get scale
		const Math::vector& GetScale() const;
		/// set rotate pivot
		void SetRotatePivot(const Math::point& p);
		/// get rotate pivot
		const Math::point& GetRotatePivot() const;
		/// set scale pivot
		void SetScalePivot(const Math::point& p);
		/// get scale pivot
		const Math::point& GetScalePivot() const;
		/// view space
		bool IsInViewSpace() const;
		/// set view space
		void SetInViewSpace(bool b);
		/// get MinDistance	
		float GetMinDistance() const;
		/// set MinDistance
		void SetMinDistance(float val);
		/// get MaxDistance	
		float GetMaxDistance() const;
		/// set MaxDistance
		void SetMaxDistance(float val);  
		/// are lod distances used
		bool LodDistancesUsed() const;
		/// get LockedToViewer	
		bool GetLockedToViewer() const;
		/// set LockedToViewer
		void SetLockedToViewer(bool val);

	protected:
		Math::point position;
		Math::quaternion rotate;
		Math::vector scale;
		Math::point rotatePivot;
		Math::point scalePivot;
		bool isInViewSpace;
		float minDistance;
		float maxDistance;
		bool useLodDistances;
		bool lockedToViewer;
	};

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		TransformNode::SetPosition(const Math::point& p)
	{
		this->position = p;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Math::point&
		TransformNode::GetPosition() const
	{
		return this->position;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		TransformNode::SetRotation(const Math::quaternion& r)
	{
		this->rotate = r;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Math::quaternion&
		TransformNode::GetRotation() const
	{
		return this->rotate;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		TransformNode::SetScale(const Math::vector& s)
	{
		this->scale = s;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Math::vector&
		TransformNode::GetScale() const
	{
		return this->scale;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		TransformNode::SetRotatePivot(const Math::point& p)
	{
		this->rotatePivot = p;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Math::point&
		TransformNode::GetRotatePivot() const
	{
		return this->rotatePivot;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		TransformNode::SetScalePivot(const Math::point& p)
	{
		this->scalePivot = p;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Math::point&
		TransformNode::GetScalePivot() const
	{
		return this->scalePivot;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline bool 
		TransformNode::IsInViewSpace() const
	{
		return this->isInViewSpace;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void 
		TransformNode::SetInViewSpace(bool b)
	{
		this->isInViewSpace = b;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline float 
		TransformNode::GetMinDistance() const
	{
		return minDistance;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void 
		TransformNode::SetMinDistance(float val)
	{
		this->minDistance  = val;
		this->useLodDistances = true;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline float 
		TransformNode::GetMaxDistance() const
	{
		return this->maxDistance;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void 
		TransformNode::SetMaxDistance(float val)
	{
		this->maxDistance = val;
		this->useLodDistances = true;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline bool
		TransformNode::LodDistancesUsed() const
	{
		return this->useLodDistances;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline bool 
		TransformNode::GetLockedToViewer() const
	{
		return this->lockedToViewer;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void 
		TransformNode::SetLockedToViewer(bool val)
	{
		this->lockedToViewer = val;
	}
} // namespace Models
//------------------------------------------------------------------------------
