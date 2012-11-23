//------------------------------------------------------------------------------
//  animbuildersaver.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "toolkitutil/meshutil/meshbuildersaver.h"
#include "io/ioserver.h"

namespace ToolkitUtil
{
using namespace Util;
using namespace IO;
using namespace System;
using namespace Math;

// file format structs
#pragma pack(push, 1)
struct Nvx2Header
{
	uint magic;
	uint numGroups;
	uint numVertices;
	uint vertexWidth;
	uint numIndices;
	uint numEdges;
	uint vertexComponentMask;
};
struct Nvx2Group
{
	uint firstVertex;
	uint numVertices;
	uint firstTriangle;
	uint numTriangles;
	uint firstEdge;
	uint numEdges;
};
#pragma pack(pop)

//------------------------------------------------------------------------------
/**
*/
bool
MeshBuilderSaver::SaveNvx2(const URI& uri, MeshBuilder& meshBuilder, Platform::Code platform)
{
    // make sure the target directory exists
    IoServer::Instance()->CreateDirectory(uri.LocalPath().ExtractDirName());

    Ptr<Stream> stream = IoServer::Instance()->CreateStream(uri);
    stream->SetAccessMode(Stream::WriteAccess);
    if (stream->Open())
    {
        ByteOrder byteOrder(ByteOrder::Host, Platform::GetPlatformByteOrder(platform));
        MeshBuilderSaver::WriteHeader(stream, meshBuilder, byteOrder);
        MeshBuilderSaver::WriteVertices(stream, meshBuilder, byteOrder);
        MeshBuilderSaver::WriteTriangles(stream, meshBuilder, byteOrder);

        stream->Close();
        stream = 0;
        return true;
    }
    else
    {
        // failed to open write stream
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
MeshBuilderSaver::WriteHeader(const Ptr<Stream>& stream, MeshBuilder& meshBuilder, const ByteOrder& byteOrder)
{
	// sort triangles by group id and create a group map
	meshBuilder.SortTriangles();
	Array<MeshBuilderGroup> groupMap;
	meshBuilder.BuildGroupMap(groupMap);

    // setup header
    Nvx2Header nvx2Header;
	nvx2Header.magic		   = byteOrder.Convert<uint>('NVX2');
	nvx2Header.numGroups	   = byteOrder.Convert<uint>(groupMap.Size());
	nvx2Header.numVertices	   = byteOrder.Convert<uint>(meshBuilder.GetNumVertices());
	nvx2Header.vertexWidth     = byteOrder.Convert<uint>(meshBuilder.VertexAt(0).GetWidth());
	nvx2Header.numIndices      = byteOrder.Convert<uint>(meshBuilder.GetNumTriangles());
	nvx2Header.vertexComponentMask= byteOrder.Convert<uint>(meshBuilder.VertexAt(0).GetComponentMask());

    // write header
    stream->Write(&nvx2Header, sizeof(nvx2Header));

	// write groups
	int curGroupIndex;
	for (curGroupIndex = 0; curGroupIndex < groupMap.Size(); curGroupIndex++)
	{
		const MeshBuilderGroup& curGroup = groupMap[curGroupIndex];
		IndexT MinVertexIndex,MaxVertexIndex;		
		meshBuilder.FindGroupVertexRange(curGroup.GetGroupId(),MinVertexIndex,MaxVertexIndex);
		Nvx2Group nvx2Group;
		nvx2Group.firstVertex	= MinVertexIndex;
		nvx2Group.numVertices	= MaxVertexIndex - MinVertexIndex + 1;
		nvx2Group.firstTriangle = curGroup.GetFirstTriangleIndex();
		nvx2Group.numTriangles  = curGroup.GetNumTriangles();
		stream->Write(&nvx2Group, sizeof(Nvx2Group));
	}
}

//------------------------------------------------------------------------------
/**
*/
void
MeshBuilderSaver::WriteVertices(const Ptr<Stream>& stream, MeshBuilder& meshBuilder, const ByteOrder& byteOrder)
{
	// write mesh block
	int curVertexIndex;
	SizeT vertexWidth = meshBuilder.VertexAt(0).GetWidth();
	SizeT numVertices = meshBuilder.GetNumVertices();
	float* floatBuffer = n_new_array(float, numVertices * vertexWidth);
	float* floatPtr = floatBuffer;
	for (curVertexIndex = 0; curVertexIndex < numVertices; curVertexIndex++)
	{
		const MeshBuilderVertex& curVertex = meshBuilder.VertexAt(curVertexIndex);
		if (curVertex.HasComponents(MeshBuilderVertex::CoordBit))
		{
			const float4& v = curVertex.GetComponent(MeshBuilderVertex::CoordIndex);
			*floatPtr++ = v.x();
			*floatPtr++ = v.y();
			*floatPtr++ = v.z();
		}
		if (curVertex.HasComponents(MeshBuilderVertex::NormalBit))
		{
			const float4& v = curVertex.GetComponent(MeshBuilderVertex::NormalIndex);
			*floatPtr++ = v.x();
			*floatPtr++ = v.y();
			*floatPtr++ = v.z();
		}
		if (curVertex.HasComponents(MeshBuilderVertex::Uv0Bit))
		{
			const float4& v = curVertex.GetComponent(MeshBuilderVertex::Uv0Index);
			*floatPtr++ = v.x();
			*floatPtr++ = v.y();
		}	
		if (curVertex.HasComponents(MeshBuilderVertex::Uv1Bit))
		{
			const float4& v = curVertex.GetComponent(MeshBuilderVertex::Uv1Index);
			*floatPtr++ = v.x();
			*floatPtr++ = v.y();
		}
		if (curVertex.HasComponents(MeshBuilderVertex::Uv2Bit))
		{
			const float4& v = curVertex.GetComponent(MeshBuilderVertex::Uv2Index);
			*floatPtr++ = v.x();
			*floatPtr++ = v.y();
		}
		if (curVertex.HasComponents(MeshBuilderVertex::Uv3Bit))
		{
			const float4& v = curVertex.GetComponent(MeshBuilderVertex::Uv3Index);
			*floatPtr++ = v.x();
			*floatPtr++ = v.y();
		}
		if (curVertex.HasComponents(MeshBuilderVertex::TangentBit))
		{
			const float4& v = curVertex.GetComponent(MeshBuilderVertex::TangentIndex);
			*floatPtr++ = v.x();
			*floatPtr++ = v.y();
			*floatPtr++ = v.z();
		}
		if (curVertex.HasComponents(MeshBuilderVertex::BinormalBit))
		{
			const float4& v = curVertex.GetComponent(MeshBuilderVertex::BinormalIndex);
			*floatPtr++ = v.x();
			*floatPtr++ = v.y();
			*floatPtr++ = v.z();
		}
		if (curVertex.HasComponents(MeshBuilderVertex::ColorBit))
		{
			const float4& v = curVertex.GetComponent(MeshBuilderVertex::ColorIndex);
			*floatPtr++ = v.x();
			*floatPtr++ = v.y();
			*floatPtr++ = v.z();
			*floatPtr++ = v.w();
		}
		if (curVertex.HasComponents(MeshBuilderVertex::WeightsBit))
		{
			const float4& v = curVertex.GetComponent(MeshBuilderVertex::WeightsIndex);
			*floatPtr++ = v.x();
			*floatPtr++ = v.y();
			*floatPtr++ = v.z();
			*floatPtr++ = v.w();
		}
		if (curVertex.HasComponents(MeshBuilderVertex::JIndicesBit))
		{
			const float4& v = curVertex.GetComponent(MeshBuilderVertex::JIndicesIndex);
			*floatPtr++ = v.x();
			*floatPtr++ = v.y();
			*floatPtr++ = v.z();
			*floatPtr++ = v.w();
		}
	}
	stream->Write(floatBuffer, numVertices * vertexWidth * sizeof(float));
	n_delete_array(floatBuffer);
	floatBuffer = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
MeshBuilderSaver::WriteTriangles(const Ptr<Stream>& stream, MeshBuilder& meshBuilder, const ByteOrder& byteOrder)
{
	// write index block
	SizeT numTriangles = meshBuilder.GetNumTriangles();
	ushort* ushortBuffer = n_new_array(ushort, numTriangles * 3);
	ushort* ushortPtr = ushortBuffer;
	int curTriangleIndex;
	for (curTriangleIndex = 0; curTriangleIndex < numTriangles; curTriangleIndex++)
	{
		const MeshBuilderTriangle& curTriangle = meshBuilder.TriangleAt(curTriangleIndex);
		int i0, i1, i2;
		curTriangle.GetVertexIndices(i0, i1, i2);
		*ushortPtr++ = (ushort) i0;
		*ushortPtr++ = (ushort) i1;
		*ushortPtr++ = (ushort) i2;
	}
	stream->Write(ushortBuffer, numTriangles * 3 * sizeof(ushort));
	n_delete_array(ushortBuffer);
	ushortBuffer = 0;
}

} // namespace ToolkitUtil
