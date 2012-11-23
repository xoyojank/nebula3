#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::AnimBuilderSaver
    
    Save AnimBuilder object into NAX3 file.
    
    (C) 2009 Radon Labs GmbH
*/
#include "io/uri.h"
#include "toolkitutil/meshutil/meshbuilder.h"
#include "toolkitutil/platform.h"
#include "io/stream.h"
#include "system/byteorder.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class MeshBuilderSaver
{
public:
    /// save NAX3 file
    static bool SaveNvx2(const IO::URI& uri, MeshBuilder& meshBuilder, Platform::Code platform);

private:
    /// write header to stream
    static void WriteHeader(const Ptr<IO::Stream>& stream, MeshBuilder& meshBuilder, const System::ByteOrder& byteOrder);
    /// write clip headers to stream
    static void WriteVertices(const Ptr<IO::Stream>& stream, MeshBuilder& meshBuilder, const System::ByteOrder& byteOrder);
    /// write clip anim events to stream
    static void WriteTriangles(const Ptr<IO::Stream>& stream, MeshBuilder& meshBuilder, const System::ByteOrder& byteOrder);
};

} // namespace ToolkitUtil
    