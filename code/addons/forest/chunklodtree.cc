#include "stdneb.h"
#include "math/scalar.h"
#include "io/binaryreader.h"
#include "forest/chunklodtree.h"
#include "io/ioserver.h"
#include "forest/chunklodnode.h"
#include "system/byteorder.h"

namespace Terrain 
{
__ImplementClass(ChunkLodTree,'CLTR',Core::RefCounted);
using namespace IO;
using namespace Util;
using namespace Math;
using namespace InternalGraphics;

//------------------------------------------------------------------------------
/**
*/
ChunkLodTree::ChunkLodTree()
{
}

//------------------------------------------------------------------------------
/**
*/
ChunkLodTree::~ChunkLodTree()
{

}

//------------------------------------------------------------------------------
/**
*/
void ChunkLodTree::Setup()
{
	Ptr<BinaryReader> reader = BinaryReader::Create();
	Ptr<Stream> stream = IO::IoServer::Instance()->CreateStream(chunkFilename);
	stream->SetURI(chunkFilename);	
	reader->SetStream(stream);
	this->isValid = true;
	//reader->SetStreamByteOrder(System::ByteOrder::LittleEndian);
	if( reader->Open())
	{
		this->chuStream = stream;
		FourCC magic = reader->ReadUInt();
		int version = reader->ReadShort();
		if (magic != FourCC('UHC'))
		{
			n_error("ChunkLodTree: '%s' is not a chu file!", stream->GetURI().AsString().AsCharPtr());
		}
		if (version != 9)
		{
			n_error("ChunkLodTree: '%s' has wrong version!", stream->GetURI().AsString().AsCharPtr());
		}

		this->treeDepth				= reader->ReadShort();
		this->errorLodMax			= reader->ReadFloat();
		this->verticalScale			= reader->ReadFloat() * this->terrainScale;
		this->baseChunkDimension	= reader->ReadFloat() * this->terrainScale;
		this->chunkCount			= reader->ReadInt();
		//this->chunkTable.Reserve(this->chunkCount);
		

		// load the chunk tree (not the actual data)		
		for (IndexT index=0; index < this->chunkCount;index++ )
		{
			Ptr<ChunkLodNode> treeNode = ChunkLodNode::Create();//n_new_array(ChunkLodNode,this->chunkCount);
			this->chunks.Append(treeNode);
			this->chunkTable.Append(0);
		}

		this->chunkTable.Fill(0,this->chunkCount,0);		
		this->chunksAllocated = 1;
		this->chunks[0]->SetParent(0);
		this->chunks[0]->SetLod(0);
		this->chunks[0]->LoadParams(reader, this->treeDepth - 1, this);
		n_assert(this->chunksAllocated == this->chunkCount);
		this->chunks[0]->LookupNeighbours(this);
		
		reader->Close();
		
	}
}

//------------------------------------------------------------------------------
/**
*/
void ChunkLodTree::Discard()
{
	this->isValid = false;
	for (IndexT i=0;i < this->chunkCount;i++ )
	{
		//this->chunks[i]->
		this->chunks[i] = 0;
	}///end for
}

//------------------------------------------------------------------------------
/**
    Updates the quad tree, must be called when the view matrix changes 
    before rendering.
*/
void
ChunkLodTree::Update(const Ptr<InternalCameraEntity>& camera)
{
	const Math::matrix44& view = camera->GetViewTransform();
	Math::vector viewPoint = view.get_position();

    if (this->chunks[0].isvalid())
    {
        // get root node data
        this->chunks[0]->RequestLoadData(this, 1.0f);
    }

    // clear the split status in the tree
    if (this->chunks[0]->GetSplit())
    {
        this->chunks[0]->ClearSplits();
    }

    // update geometry
    //this->chunks[0]->Update(this, viewPoint);

    // update texture (AFTER updating geometry!)
    //this->chunks[0].UpdateTexture(this, viewPoint);
}

//------------------------------------------------------------------------------
/**
    Given an AABB and the viewpoint, this function computes a desired
    LOD level, based on the distance from the viewpoint to the nearest
    point on the box.  So, desired LOD is purely a function of
    distance and the chunk tree parameters.
*/
ushort
ChunkLodTree::ComputeLod(const Math::bbox& box, const Math::vector& viewPoint) const
{
    static vector disp;
    static point center;
    static vector extent;
    
    center = box.center();
    extent = box.extents();

    disp = viewPoint - center;
	disp.set_x( n_max(0.0f, fabsf(disp.x()) - extent.x()) );
	disp.set_y( n_max(0.0f, fabsf(disp.y()) - extent.y()));
	disp.set_z( n_max(0.0f, fabsf(disp.z()) - extent.z()));

    float d = disp.length();
    ushort lod = n_iclamp(((this->treeDepth << 8) - 1) - int(n_log2(n_max(1.0f, d / this->distanceLodMax)) * 256), 0, 0xffff);

    return lod;
}

//------------------------------------------------------------------------------
/**
*/
void 
ChunkLodTree::Render(const Ptr<InternalCameraEntity>& camera)
{
	Math::matrix44 worldviewProj = camera->GetViewProjTransform();
	if( this->chunks[0].isvalid())
	{
		this->chunks[0]->Render(this,worldviewProj,ClipStatus::Inside);
	}
}
}