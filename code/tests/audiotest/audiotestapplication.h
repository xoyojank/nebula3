#pragma once
#ifndef TEST_AUDIOTESTAPPLICATION_H
#define TEST_AUDIOTESTAPPLICATION_H
//------------------------------------------------------------------------------
/**
    @class Test::AudioTestApplication
    
    Simple audio test application.
    
    (C) 2008 Radon Labs GmbH
*/
#include "apprender/viewerapplication.h"
#include "audio/audioserver.h"
#include "audio/audioplayer.h"
#include "audio/audiointerface.h"
#include "audio/audioemitter3D.h"

//------------------------------------------------------------------------------
namespace Test
{
class AudioTestApplication : public App::ViewerApplication
{
public:
    /// constructor
    AudioTestApplication();
    /// destructor
    virtual ~AudioTestApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();

protected:
    /// process input (called before rendering)
    virtual void OnProcessInput();
    /// update world 
    virtual void OnUpdateFrame();
    
    Ptr<Audio::AudioInterface> audioInterface;
    Ptr<Audio::AudioServer> audioServer;
    Ptr<Audio::AudioPlayer> audioPlayer;
    Util::Array<Ptr<Audio::AudioEmitter3D>> audioEmitters;
};

} // namespace Test
//------------------------------------------------------------------------------
#endif
    