#pragma once
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::AttachmentManager
    
    Main thread side manager (frontend) for managing attachments. 

    (C) 2008 Radon Labs GmbH
*/
#include "game/manager.h"
#include "core/singleton.h"
#include "util/string.h"
#include "math/matrix44.h"
#include "internalgraphics/attachmentserver.h"

// forward decl
namespace Graphics
{
    class GraphicsEntity;
};

namespace Game
{
    class Entity;
};

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class AttachmentManager : public Game::Manager
{
    __DeclareClass(AttachmentManager);
    __DeclareSingleton(AttachmentManager);

public:
	/// constructor
    AttachmentManager();
    /// destructor
    virtual ~AttachmentManager();

    /// trigger
    virtual void OnFrame();

    /// add a attachment
    void AddGraphicsAttachment( const Util::StringAtom& jointName,
                                const Util::StringAtom& baseEntityId,
                                const Util::StringAtom& entityToAttachResId,
                                const Math::matrix44& offset,
                                bool keepLocal,
                                InternalGraphics::AttachmentServer::AttachmentRotation rotation);

    /// attach a attachment temporary
    void AddGraphicsAttachmentTemporary( const Util::StringAtom& jointName,
                                         const Util::StringAtom& baseEntityId,
                                         const Util::StringAtom& entityToAttachResId,
                                         const Math::matrix44& offset,
                                         bool keepLocal,
                                         InternalGraphics::AttachmentServer::AttachmentRotation rotation,
                                         Timing::Time duration);

    /// add a attachment
    void AddGraphicsAttachment( const Util::StringAtom& jointName,
                                const Ptr<Game::Entity>& baseEntityPtr,
                                const Util::StringAtom& entityToAttachResId,
                                const Math::matrix44& offset,
                                bool keepLocal,
                                InternalGraphics::AttachmentServer::AttachmentRotation rotation);

    /// attach a attachment temporary
    void AddGraphicsAttachmentTemporary( const Util::StringAtom& jointName,
                                         const Ptr<Game::Entity>& baseEntityPtr,
                                         const Util::StringAtom& entityToAttachResId,
                                         const Math::matrix44& offset,
                                         bool keepLocal,
                                         InternalGraphics::AttachmentServer::AttachmentRotation rotation,
                                         Timing::Time duration);

    /// clear attachments
    void ClearAttachments();

    /// remove all attachments on given base entity
    void ClearAttachmentsOnEntity(const Ptr<Game::Entity>& baseEntity);
    
private:
    /// attachment description
    struct Attachment
    {
        Ptr<Graphics::GraphicsEntity> baseEntity;
        Ptr<Graphics::GraphicsEntity> attachedEntity;
        Util::StringAtom joint;        
        Math::matrix44 offset;
        bool keepLocal;
        InternalGraphics::AttachmentServer::AttachmentRotation rotation;
        Timing::Time duration;
        Timing::Time startTime;
        Util::StringAtom newJoint;
    };
    /// get a graphics entity by base entity
    Ptr<Graphics::GraphicsEntity> GetAttachmentGfxEntity(const Ptr<Graphics::GraphicsEntity>& gfxBaseEntity, const Util::StringAtom& jointName) const;

    /// check if we have to delete some attachments
    void CheckTemporaryAttachments();

    /// send final attachment to render thread
    void SendAttachmentMessage( const Attachment& attachment );

    /// send final detachment to render thread
    void SendDetachmentMessage( const Attachment& detachment );

    Util::Array<Attachment> attachments;                 // they are still attached
}; 

} // namespace GraphicsFeature
//------------------------------------------------------------------------------


    
    