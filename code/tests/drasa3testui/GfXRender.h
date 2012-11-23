#pragma once
//------------------------------------------------------------------------------
/**
    @class SUI::SUIRenderModule

    The central client-side object for the SimpleUI system.

    (C) 2009 Radon Labs GmbH
*/
#include "core/singleton.h"
#include "util/stringatom.h"
#include "resources/resourceid.h"
#include "frame/framebatch.h"
#include "GFxTutorial.h"
#include "input/inputevent.h"

//------------------------------------------------------------------------------
namespace ScaleForms
{
class GfxRender : public Core::RefCounted
{
    __DeclareClass(GfxRender);
    __DeclareSingleton(GfxRender);
public:
	/// constructor
	GfxRender();
	/// destructor
	virtual ~GfxRender();

	/// setup the server
	void Setup();
	/// discard the server
	void Discard();
	/// return true if server has been setup
	bool IsValid() const;

	/// render SUI system, may be called several times per frame
	void Render(const Ptr<Frame::FrameBatch>& frameBatch);
	/// process input events on all visible layout
	bool ProcessInputEvents(const Util::Array<GFxMouseEvent>& inputEvents);

private:
	Ptr<GFxTutorial> gfxTutorial;
	IDirect3DStateBlock9*	pStateBlock;
	bool isValid;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
GfxRender::IsValid() const
{
    return this->isValid;
}

} // namespace SUI
//------------------------------------------------------------------------------
