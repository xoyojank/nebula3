#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::View
    
    A client-side proxy of a InternalGraphics::InternalView in the Graphic subsystem.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "util/stringatom.h"
#include "resources/resourceid.h"
#include "threading/objectref.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class CameraEntity;

class View : public Core::RefCounted
{
    __DeclareClass(View);
public:
    /// constructor
    View();
    /// destructor
    virtual ~View();

    /// return true if the View is valid
    bool IsValid() const;    
    /// get name of view
    const Util::StringAtom& GetName() const;
    /// get the class-type of the server-side view object
    const Core::Rtti& GetViewClass() const;
    /// get the name of the stage this view is attached to
    const Util::StringAtom& GetStageName() const;
    /// get the name of the frame-shader this view will use for rendering
    const Resources::ResourceId& GetFrameShaderName() const;
    /// check whether this is the default view
    bool IsDefaultView() const;
    /// set the camera entity this view should "look through"
    void SetCameraEntity(const Ptr<CameraEntity>& cameraEntity);
    /// get the view's current camera entity (may return invalid ptr if no camera is set)
    const Ptr<CameraEntity>& GetCameraEntity() const;

private:
    friend class GraphicsServer;

    /// set name of view
    void SetName(const Util::StringAtom& name);
    /// set the class-type of the server-side view object
    void SetViewClass(const Core::Rtti& rtti);
    /// set the name of the stage this view will be attached to (stage must already exist!)
    void SetStageName(const Util::StringAtom& stageName);
    /// set the name of the frame-shader this view will use for rendering
    void SetFrameShaderName(const Resources::ResourceId& frameShaderName);
    /// set to true if this is the default view
    void SetDefaultView(bool b);
    /// setup the view (waits for completion)
    void Setup();
    /// discard the view (waits for completion)
    void Discard();

    Util::StringAtom name;
    const Core::Rtti* viewClass;
    Util::StringAtom stageName;
    Resources::ResourceId frameShaderName;
    bool isDefaultView;
    Ptr<Threading::ObjectRef> objectRef;
    Ptr<CameraEntity> cameraEntity;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
View::IsValid() const
{
    return this->objectRef.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline void
View::SetName(const Util::StringAtom& n)
{
    this->name = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
View::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
View::SetViewClass(const Core::Rtti& rtti)
{
    this->viewClass = &rtti;
}

//------------------------------------------------------------------------------
/**
*/
inline const Core::Rtti& 
View::GetViewClass() const
{
    n_assert(0 != this->viewClass);
    return *this->viewClass;
}

//------------------------------------------------------------------------------
/**
*/
inline void
View::SetStageName(const Util::StringAtom& n)
{
    this->stageName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
View::GetStageName() const
{
    return this->stageName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
View::SetFrameShaderName(const Resources::ResourceId& r)
{
    this->frameShaderName = r;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
View::GetFrameShaderName() const
{
    return this->frameShaderName;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
View::IsDefaultView() const
{
    return this->isDefaultView;
}

} // namespace Graphics
//------------------------------------------------------------------------------
