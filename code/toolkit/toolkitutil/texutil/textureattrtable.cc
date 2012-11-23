//------------------------------------------------------------------------------
//  textureattrtable.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "textureattrtable.h"
#include "io/ioserver.h"
#include "io/stream.h"
#include "io/xmlreader.h"

namespace ToolkitUtil
{
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
TextureAttrTable::TextureAttrTable() :
    valid(false)
{
    this->texAttrs.Reserve(10000);
    this->indexMap.Reserve(10000);
}

//------------------------------------------------------------------------------
/**
*/
TextureAttrTable::~TextureAttrTable()
{
    if (this->IsValid())
    {
        this->Discard();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
TextureAttrTable::Setup(const String& path)
{
    n_assert(!this->IsValid());
    n_assert(this->texAttrs.IsEmpty());
    n_assert(this->indexMap.IsEmpty());

    // read the batchattributes.xml file
    Ptr<Stream> stream = IoServer::Instance()->CreateStream(path);
    Ptr<XmlReader> xmlReader = XmlReader::Create();
    xmlReader->SetStream(stream);
    if (xmlReader->Open())
    {
        xmlReader->SetToNode("/Textures");
        if (xmlReader->SetToFirstChild("Texture")) do
        {
            // read current attributes
            String pattern  = xmlReader->GetString("pattern");
            SizeT maxHeight = xmlReader->GetInt("maxHeight");
            SizeT maxWidth  = xmlReader->GetInt("maxWidth");
            TextureAttrs::Filter mipFilter = TextureAttrs::StringToFilter(xmlReader->GetString("mipFilter"));
            bool genMipMaps = xmlReader->GetString("mipMaps") == "Yes";
            TextureAttrs::PixelFormat rgbFormat = TextureAttrs::StringToPixelFormat(xmlReader->GetString("rgb"));
            TextureAttrs::PixelFormat rgbaFormat = TextureAttrs::StringToPixelFormat(xmlReader->GetString("rgba"));
            TextureAttrs::Filter scaleFilter = TextureAttrs::StringToFilter(xmlReader->GetString("scaleFilter"));
            TextureAttrs::Quality quality = TextureAttrs::StringToQuality(xmlReader->GetString("quality"));

            // create a new TextureAttrs object and add it to the dictionary
            TextureAttrs attrs;
            attrs.SetMaxWidth(maxWidth);
            attrs.SetMaxHeight(maxHeight);
            attrs.SetGenMipMaps(genMipMaps);
            attrs.SetRGBPixelFormat(rgbFormat);
            attrs.SetRGBAPixelFormat(rgbaFormat);
            attrs.SetMipMapFilter(mipFilter);
            attrs.SetScaleFilter(scaleFilter);
            attrs.SetQuality(quality);
            if (pattern == "*/*")
            {
                this->defaultAttrs = attrs;
            }
            else
            {
                this->texAttrs.Append(attrs);
                this->indexMap.Add(pattern, this->texAttrs.Size() - 1);
            }
        }
        while (xmlReader->SetToNextChild("Texture"));
        this->valid = true;
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
TextureAttrTable::Discard()
{
    n_assert(this->IsValid());
    this->valid = false;
    this->texAttrs.Clear();
    this->indexMap.Clear();
}

//------------------------------------------------------------------------------
/**
    NOTE: the texName must have the form "category/texture.ext", so it's
    not an actual file name!
*/
bool
TextureAttrTable::HasEntry(const String& texName) const
{
    n_assert(this->IsValid());
    return this->indexMap.Contains(texName);
}

//------------------------------------------------------------------------------
/**
    NOTE: the texName must have the form "category/texture.ext", so it's
    not an actual file name!
    If no matching entry is found, the default texture attribute object 
    will be returned.
*/
const TextureAttrs&
TextureAttrTable::GetEntry(const String& texName) const
{
    n_assert(this->IsValid());
    IndexT i = this->indexMap.FindIndex(texName);
    if (InvalidIndex == i)
    {
        return this->defaultAttrs;
    }
    else
    {
        return this->texAttrs[this->indexMap.ValueAtIndex(i)];
    }
}

//------------------------------------------------------------------------------
/**
*/
const TextureAttrs&
TextureAttrTable::GetDefaultEntry() const
{
    n_assert(this->IsValid());
    return this->defaultAttrs;
}

} // namespace ToolkitUtil
