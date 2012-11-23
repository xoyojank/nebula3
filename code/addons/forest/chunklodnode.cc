#include "stdneb.h"
#include "math/bbox.h"
#include "forest/chunklodnode.h"
#include "forest/chunklodtree.h"

namespace Terrain
{
__ImplementClass(ChunkLodNode,'CLND',Core::RefCounted);


using namespace IO;
using namespace Math;
//------------------------------------------------------------------------------
/**
*/
ChunkLodNode::ChunkLodNode()
{
}

//------------------------------------------------------------------------------
/**
*/
ChunkLodNode::~ChunkLodNode()
{
}

//------------------------------------------------------------------------------
/**
    Converts the neighbour chunk labels to pointers, recurse into child
    chunks.
*/
void
ChunkLodNode::LookupNeighbours(const Ptr<ChunkLodTree>& tree)
{
    n_assert(tree.isvalid());
    int i;
    for (i = 0; i < 4; i++)
    {
        if (this->neighbour[i].label == -1)
        {
            this->neighbour[i].node = 0;
        }
        else
        {
            this->neighbour[i].node = tree->GetChunkByLabel(this->neighbour[i].label);
        }
    }

    if (this->HasChildren())
    {
        for (i = 0; i < 4; i++)
        {
            this->children[i]->LookupNeighbours(tree);
        }
    }
}

//------------------------------------------------------------------------------
/**
    Recursively reads the chunk structure (not the actual mesh data)
    from the chu file and constructs the tree of chunk nodes.
*/
bool
ChunkLodNode::LoadParams(const Ptr<BinaryReader>& reader, int recurseCount, const Ptr<ChunkLodTree>& tree)
{
    n_assert(reader.isvalid());
    n_assert(tree.isvalid());

    // read the chunk's label
    int chunkLabel = reader->ReadInt();
    tree->SetChunkByLabel(chunkLabel, this);

    // read neighbour labels
    int i;
    for (i = 0; i < 4; i++)
    {
        this->neighbour[i].label =  reader->ReadInt();
    }

    // read chunk address
    this->level		= reader->ReadChar();
    this->x			= reader->ReadUShort();
    this->z			= reader->ReadUShort();
    this->minY		= reader->ReadUShort();
    this->maxY		= reader->ReadUShort();

    // read vertex data file offset
    this->vertexDataPosition = reader->ReadInt();

    // recurse into child chunks
    if (recurseCount > 0)
    {
        for (i = 0; i < 4; i++)
        {
            this->children[i] = tree->chunks[tree->chunksAllocated++];
            this->children[i]->SetParent(this);
            this->children[i]->SetLod(this->lod + 0x100);
            this->children[i]->LoadParams(reader, recurseCount - 1, tree);
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
    Request to load the data for this node.
*/
void
ChunkLodNode::RequestLoadData(const Ptr<ChunkLodTree>& tree, float /*priority*/)
{
    // load request already pending?
    if (this->chunkLodMesh.isvalid())
    {
        return;
    }

	this->chunkLodMesh = ChunkLodMesh::Create();
    n_assert(this->chunkLodMesh);
    
    // fill chunk mesh parameters
    static bbox box;
    //this->ComputeBoundingBox(tree, box);
    this->chunkLodMesh->SetStream(tree->GetStream());
    this->chunkLodMesh->SetFileLocation(this->vertexDataPosition);

	Math::vector scale = box.extents();
	float(1<<14);
	scale.set_x(scale.x()/float(1<<14));
	scale.set_y(tree->verticalScale);
	scale.set_z(scale.z()/float(1<<14));
    Math::vector offset = box.center();
	offset.set_y(0.0f);

    this->chunkLodMesh->SetVertexScale(scale);
    this->chunkLodMesh->SetVertexOffset(offset);
    this->chunkLodMesh->Setup();
    tree->numMeshesAllocated++;
}

//------------------------------------------------------------------------------
/**
    Compute the bounding box for this node.
*/
inline
void
ChunkLodNode::ComputeBoundingBox(const Ptr<ChunkLodTree>& tree, Math::bbox& box) const
{
    float levelFactor = float(1 << (tree->treeDepth - 1 - this->level));
    static vector center;
    static vector extent;
    const vector& origin = tree->GetTerrainOrigin();

    center.set_x( ((this->x + 0.5f) * levelFactor * tree->baseChunkDimension) - origin.x() );
    center.set_y( ((this->maxY + this->minY) * 0.5f * tree->verticalScale) - origin.y() );
    center.set_z( ((this->z + 0.5f) * levelFactor * tree->baseChunkDimension) - origin.z());

    const float extraBoxSize = 1e-3f;   // this is to make chunks overlap by about a millimeter, to avoid cracks.
    extent.set_x( levelFactor * tree->baseChunkDimension * 0.5f + extraBoxSize);
    extent.set_y( (this->maxY + this->minY) * 0.5f * tree->verticalScale );
    extent.set_z( extent.x() );

    box.set(center, extent);
}

//------------------------------------------------------------------------------
/**
    Computes 'lod' and split values for this chunk and its subtree,
    based on the given camera parameters and the parameters stored in
    'base'.  Traverses the tree and forces neighbor chunks to a valid
    LOD, and updates its contained edges to prevent cracks.

    Invariant: if a node has m_split == true, then all its ancestors
    have m_split == true.
    
    Invariant: if a node has m_data != NULL, then all its ancestors
    have m_data != NULL.

    !!!  For correct results, the tree must have been clear()ed before
    calling update() !!!
*/
void
ChunkLodNode::Update(const Ptr<ChunkLodTree>& tree, const Math::vector& viewPoint)
{
    static bbox box;
    this->ComputeBoundingBox(tree, box);

    ushort desiredLod = tree->ComputeLod(box, viewPoint);

    if (this->HasChildren() && (desiredLod > (this->lod | 0xff)) && this->CanSplit(tree))
    {
        this->DoSplit(tree, viewPoint);
        
        // recurse to children
        int i;
        for (i = 0; i < 4; i++)
        {
            this->children[i]->Update(tree, viewPoint);
        }
    }
    else
    {
		// We're good... this chunk can represent its region within the max error tolerance.
        if ((this->lod & 0xff00) == 0)
        {
			// Root chunk -- make sure we have valid morph value.
            this->lod = n_iclamp(desiredLod, this->lod & 0xff00, this->lod | 0xff);
            n_assert((this->lod >> 8) == this->level);
        }

		// Request residency for our children, and request our
		// grandchildren and further descendents be unloaded.
        if (this->HasChildren())
        {
            float priority = 0.0f;
            if (desiredLod > (this->lod & 0xff00))
            {
                priority = (this->lod & 0xff) / 255.0f;
            }
            if (priority < 0.5f)
            {
                int i;
                for (i = 0; i < 4; i++)
                {
                    this->children[i]->RequestUnloadSubtree(tree);
                }
            }
            else
            {
                int i;
                for (i = 0; i < 4; i++)
                {
                   this->children[i]->WarmUpData(tree, priority);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
    Enable this chunk.  Use the given viewpoint to decide what morph
    level to use.
*/
void
ChunkLodNode::DoSplit(const Ptr<ChunkLodTree>& tree, const Math::vector& viewPoint)
{
    if (false == this->split)
    {
        n_assert(this->CanSplit(tree));
        n_assert(this->IsValid());
        
        this->split = true;

        if (this->HasChildren())
        {
			// Make sure children have a valid lod value.
            int i;
            for (i = 0; i < 4; i++)
            {
                ChunkLodNode* child = this->children[i];
                static bbox box;
                child->ComputeBoundingBox(tree, box);
                ushort desiredLod = tree->ComputeLod(box, viewPoint);
                child->lod = n_iclamp(desiredLod, child->lod & 0xff00, child->lod | 0xff);
            }
        }

        // make sure ancestors are split...
        ChunkLodNode* p;
        for (p = this->parent; p && (false == p->split); p = p->parent)
        {
            p->DoSplit(tree, viewPoint);
        }
    }
}

//------------------------------------------------------------------------------
/**
    Return true if this chunk can be split.  Also, requests the
    necessary data for the chunk children and its dependents.

    A chunk won't be able to be split if it doesn't have vertex data,
    or if any of the dependent chunks don't have vertex data.
*/
bool
ChunkLodNode::CanSplit(const Ptr<ChunkLodTree>& tree)
{
    if (this->split)
    {
		// Already split.  Also our data & dependents' data is already
		// freshened, so no need to request it again.
        return true;
    }

    if (false == this->HasChildren())
    {
		// Can't ever split.  No data to request.
		return false;
	}

    bool canSplit = true;

	// Check the data of the children.
    int i;
    for (i = 0; i < 4; i++)
    {
        Ptr<ChunkLodNode> child = this->children[i];
        if (child.isvalid())
        {
            child->RequestLoadData(tree, 1.0f);
            canSplit = false;
        }
    }

    // make sure ancestors have data
    ChunkLodNode* p;
    for (p = this->parent; p && (false == p->split); p = p->parent)
    {
        if (false == p->CanSplit(tree))
        {
            canSplit = false;
        }
    }

	// Make sure neighbors have data at a close-enough level in the tree.
    for (i = 0; i < 4; i++)
    {
        ChunkLodNode* n = this->neighbour[i].node;
		const int maxAllowedNeighbourDifference = 2;    // allow up to two levels of difference between chunk neighbors.
        int count;
        for (count = 0; n && (count < maxAllowedNeighbourDifference); count++)
        {
            n = n->parent;
        }
        if (n && (false == n->CanSplit(tree)))
        {
            canSplit = false;
        }
    }
    return canSplit;
}

//------------------------------------------------------------------------------
/**
    Schedule this node's data for loading at the given priority.  Also,
    schedule our child/descendent nodes for unloading.
*/
void
ChunkLodNode::WarmUpData(const Ptr<ChunkLodTree>& tree, float priority)
{
    if (!this->IsValid())
    {
        // request our data
        this->RequestLoadData(tree, priority);
    }

	// Request unload.  Skip a generation if our priority is
	// fairly high.
    if (this->HasChildren())
    {
        if (priority < 0.5f)
        {
			// Dump our child nodes' data.
            int i;
            for (i = 0; i < 4; i++)
            {
                this->children[i]->RequestUnloadSubtree(tree);
            }
        }
        else
        {
			// Fairly high priority; leave our children
			// loaded, but dump our grandchildren's data.
            for (int i = 0; i < 4; i++)
            {
                ChunkLodNode* child = this->children[i];
                if (child->HasChildren())
                {
                    for (int j = 0; j < 4; j++)
                    {
                        child->children[j]->RequestUnloadSubtree(tree);
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
    If we have any data, request that it be unloaded.  Make the same
    request of our descendants.
*/
void
ChunkLodNode::RequestUnloadSubtree(const Ptr<ChunkLodTree>& tree)
{
    if (this->IsValid())
    {
		// Put descendents in the queue first, so they get
		// unloaded first.
        if (this->HasChildren())
        {
            int i;
            for (i = 0; i < 4; i++)
            {
                this->children[i]->RequestUnloadSubtree(tree);
            }
        }
        this->RequestUnloadData(tree);
    }
}

//------------------------------------------------------------------------------
/**
    Request to unload the data for this node.
*/
void
ChunkLodNode::RequestUnloadData(const Ptr<ChunkLodTree>& tree)
{
    n_assert(this->chunkLodMesh.isvalid());
	this->chunkLodMesh->Discard();
    this->chunkLodMesh = 0;
    tree->numMeshesAllocated--;
}

//------------------------------------------------------------------------------
/**
*/
void 
ChunkLodNode::Render(const Ptr<ChunkLodTree>& tree,const Math::matrix44& modelViewProj,Math::ClipStatus::Type clipStatus)
{
	n_assert(this->IsValid());
	n_assert(clipStatus != ClipStatus::Outside);

	// view frustum culling (we don't need to check for culling if one
	// of our parents was already fully inside)
	if (ClipStatus::Clipped == clipStatus)
	{
		// cull against view frustum
		static bbox box;
		this->ComputeBoundingBox(tree, box);
		clipStatus = box.clipstatus(modelViewProj);
		if (ClipStatus::Outside == clipStatus)
		{
			// fully outside, return and break recursion
			return;
		}
	}


	int triCount = 0;
	if (this->GetSplit())
	{
		n_assert(this->HasChildren());

		// a split node, recurse to children
		int i;
		for (i = 0; i < 4; i++)
		{
			this->children[i]->Render(tree, modelViewProj,clipStatus);
		}
	}
	else
	{
		// not split, display our own data
		this->chunkLodMesh->Draw();
	}
}

}