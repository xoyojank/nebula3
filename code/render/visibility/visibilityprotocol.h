#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/array.h"
#include "math/matrix44.h"
#include "graphics/graphicsentity.h"

//------------------------------------------------------------------------------
namespace Visibility
{
//------------------------------------------------------------------------------
class CreateVisibilityCluster : public Messaging::Message
{
    __DeclareClass(CreateVisibilityCluster);
    __DeclareMsgId;
public:
    CreateVisibilityCluster() 
    { };
public:
    void SetStageName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->stagename = val;
    };
    const Util::String& GetStageName() const
    {
        return this->stagename;
    };
private:
    Util::String stagename;
public:
    void SetBoundingBoxes(const Util::Array<Math::matrix44>& val)
    {
        n_assert(!this->handled);
        this->boundingboxes = val;
    };
    const Util::Array<Math::matrix44>& GetBoundingBoxes() const
    {
        return this->boundingboxes;
    };
private:
    Util::Array<Math::matrix44> boundingboxes;
public:
    void SetEntities(const Util::Array<Ptr<Graphics::GraphicsEntity> >& val)
    {
        n_assert(!this->handled);
        this->entities = val;
    };
    const Util::Array<Ptr<Graphics::GraphicsEntity> >& GetEntities() const
    {
        return this->entities;
    };
private:
    Util::Array<Ptr<Graphics::GraphicsEntity> > entities;
};
//------------------------------------------------------------------------------
class CreateVisibilityBoxes : public Messaging::Message
{
    __DeclareClass(CreateVisibilityBoxes);
    __DeclareMsgId;
public:
    CreateVisibilityBoxes() 
    { };
public:
    void SetStageName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->stagename = val;
    };
    const Util::String& GetStageName() const
    {
        return this->stagename;
    };
private:
    Util::String stagename;
public:
    void SetBoundingBoxes(const Util::Array<Math::matrix44>& val)
    {
        n_assert(!this->handled);
        this->boundingboxes = val;
    };
    const Util::Array<Math::matrix44>& GetBoundingBoxes() const
    {
        return this->boundingboxes;
    };
private:
    Util::Array<Math::matrix44> boundingboxes;
};
} // namespace Visibility
//------------------------------------------------------------------------------
