#pragma once
//------------------------------------------------------------------------------
/**
    @class Forest::ForestServer
        
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "io/binaryreader.h"
#include "chunklodmesh.h"
#include "math/bbox.h"

//------------------------------------------------------------------------------
namespace Terrain
{
class ChunkLodTree;

class ChunkLodNode : public Core::RefCounted
{
    __DeclareClass(ChunkLodNode);

public:

    /// constructor
    ChunkLodNode();
    /// destructor
    virtual ~ChunkLodNode();  
	/// recursively read chunk parameters from .chu file
	bool LoadParams(const Ptr<IO::BinaryReader>& reader, int recurseCount, const Ptr<ChunkLodTree>& tree);
	/// convert neighbour labels to pointers
	void LookupNeighbours(const Ptr<ChunkLodTree>& tree);
	/// set pointer to parent chunk
	void SetParent(const Ptr<ChunkLodNode>& p);
	/// get pointer to parent chunk
	const Ptr<ChunkLodNode>& GetParent() const;
	/// set lod value
	void SetLod(ushort l);
	/// get lod value
	ushort GetLod() const;
	/// return true if node has children
	bool HasChildren() const;

	/// request to load the data for this node at the given priority
	void RequestLoadData(const Ptr<ChunkLodTree>& tree, float priority);
	/// compute the bounding box of this node
	void ComputeBoundingBox(const Ptr<ChunkLodTree>& tree, Math::bbox& box) const;
	/// get the status of the split flag
	bool GetSplit() const;
	/// clears the split values throughout the quad tree
	void ClearSplits();
	/// update geometry based on viewpoint
	void Update(const Ptr<ChunkLodTree>& tree, const Math::vector& viewPoint);	
	/// split this chunk (makes it valid for rendering)
	void DoSplit(const Ptr<ChunkLodTree>& tree, const Math::vector& viewPoint);
	/// return true if this chunk can be split
	bool CanSplit(const Ptr<ChunkLodTree>& tree);
	/// schedule this node's data for loading at the given priority
	void WarmUpData(const Ptr<ChunkLodTree>& tree, float priority);
	/// returns true when the data for this chunk is available
	bool IsValid();
	/// request unloading the chunk node and all child nodes
	void RequestUnloadSubtree(const Ptr<ChunkLodTree>& tree);
	/// request unloading the data of this node
	void RequestUnloadData(const Ptr<ChunkLodTree>& tree);
	/// render the chunk lod node
	void Render(const Ptr<ChunkLodTree>& tree,const Math::matrix44& modelViewProj,Math::ClipStatus::Type clipStatus);

private:
	Ptr<ChunkLodNode> parent;          // pointer to parent chunk
	Ptr<ChunkLodNode> children[4];     // pointer to children
	union
	{
		int label;
		ChunkLodNode* node;
	} neighbour[4];                 // labels / pointers to neighbor nodes

	ushort x;                       // chunk "address" (its position in the quadtree)
	ushort z;                    
	uchar level;

	short minY;                     // vertical bounds, for constructing bounding box
	short maxY;

	bool split;                     // true if this node should be rendered by descendents
	ushort lod;                     // LOD of this chunk.  high byte never changes; low byte is the morph parameter.

	int vertexDataPosition;
	Ptr<ChunkLodMesh> chunkLodMesh;
};

//------------------------------------------------------------------------------
/**
*/
inline
bool
ChunkLodNode::IsValid()
{
	return this->chunkLodMesh.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
ChunkLodNode::GetSplit() const
{
	return this->split;
}

//------------------------------------------------------------------------------
/**
    This clears the "split" node members throughout the quad tree.
    If this node is not split, then the recursion does not continue to the 
    child nodes, since their m_split values should be false.

    Do this before calling Update().
*/
inline
void
ChunkLodNode::ClearSplits()
{   
    if (this->split)
    {
        this->split = false;
        if (this->HasChildren())
        {
            int i;
            for (i = 0; i < 4; i++)
            {
                this->children[i]->ClearSplits();
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ChunkLodNode::SetParent(const Ptr<ChunkLodNode>& p)
{
	this->parent = p;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Ptr<ChunkLodNode>&
ChunkLodNode::GetParent() const
{
	return this->parent;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ChunkLodNode::SetLod(ushort l)
{
	this->lod = l;
}

//------------------------------------------------------------------------------
/**
*/
inline
ushort
ChunkLodNode::GetLod() const
{
	return this->lod;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
ChunkLodNode::HasChildren() const
{
	return (this->children[0] != 0);
}

} 