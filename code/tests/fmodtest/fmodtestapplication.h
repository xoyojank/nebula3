#pragma once
#ifndef TEST_FMODTESTAPPLICATION_H
#define TEST_FMODTESTAPPLICATION_H
//------------------------------------------------------------------------------
/**
    @class Test::FmodTestApplication
  
    Testapplication for FMOD
    
    (C) 2010 Radon Labs GmbH
*/    
#include "apprender/viewerapplication.h"
#include "audio2/fmodeventprojectid.h"
#include "audio2/audio2interface.h"
#include "audio2/audio2server.h"
#include "audio2/audio2emitter.h"
#include "audio2/audio2player.h"
#include "audio2/audio2listener.h"

//------------------------------------------------------------------------------
namespace Test
{
class FmodTestApplication : public App::ViewerApplication
{
public:
    /// constructor
    FmodTestApplication();
    /// destructor
    virtual ~FmodTestApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();

private:
	/// process input (called before rendering)
    void OnProcessInput();
    /// called when frame should be updated
    void OnUpdateFrame();

	Ptr<Audio2::Audio2Interface> audioInterface;
    Ptr<Audio2::Audio2Server> audioServer;
    Ptr<Audio2::Audio2Player> audioPlayer;
	Ptr<Audio2::Audio2Emitter> emitter;
	Ptr<Audio2::Audio2Listener> listener;
	Ptr<Audio2::Audio2Emitter> programmerSoundEmitter;

	const Audio2::FmodEventProjectId projectId;

	Util::String debugText;
};

} // namespace Test
//------------------------------------------------------------------------------
#endif
