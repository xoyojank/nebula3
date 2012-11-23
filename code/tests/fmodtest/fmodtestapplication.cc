//------------------------------------------------------------------------------
//  fmodtestapplication.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "fmodtestapplication.h"
#include "input/keyboard.h"
#include "audio2/fmodeventparameterid.h"
#include "debugrender/debugrender.h"

namespace Test
{
using namespace Resources;
using namespace Math;
using namespace Util;
using namespace Graphics;
using namespace Audio2;
using namespace Input;

//------------------------------------------------------------------------------
/**
*/
FmodTestApplication::FmodTestApplication() : projectId("godsend")
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FmodTestApplication::~FmodTestApplication()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
FmodTestApplication::Open()
{
    n_assert(!this->IsOpen());
    if (ViewerApplication::Open())
    {
        // setup the audio system
		this->audioInterface = Audio2Interface::Create();
		this->audioInterface->Open();
		this->audioServer = Audio2Server::Create();
		this->audioServer->Open();
        this->audioPlayer = Audio2::Audio2Player::Create();
		this->listener = Audio2::Audio2Listener::Instance();

		// load fmod project
        this->audioServer->LoadEventProject(this->projectId);
        
		// play sound using the emitter
        const Audio2::FmodEventId eventId0(this->projectId, "music/music_mainmenu");
		this->emitter = Audio2::Audio2Emitter::Create();
        this->emitter->Setup(eventId0);
		this->emitter->SetVolume(0.25f);
        this->emitter->Play();
		this->debugText = "play: music/music_mainmenu";

		// setup a "programmer sound"
		const Audio2::FmodEventId eventId1(this->projectId, "effects/programmer_sound");
		this->programmerSoundEmitter = Audio2::Audio2Emitter::Create();
		this->programmerSoundEmitter->Setup(eventId1);
		
		// enable auditioning feature
		// With auditioning eventsproperties can be altered in the FMOD-Designer 
        // during the the application is running and will be applied immediatly
        // over a TCP connection.
		this->audioServer->SetAuditioningEnabled(true);

		return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
FmodTestApplication::Close()
{
	this->programmerSoundEmitter->Discard();
    this->emitter->Discard();
    this->audioServer->Close();
    this->audioServer = NULL;
    this->audioInterface->Close();
    this->audioInterface = NULL;
    this->audioPlayer = NULL;

	ViewerApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
FmodTestApplication::OnProcessInput()
{
    const Ptr<Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
    
	// test 2D-sound
    if (keyboard->KeyDown(Key::Key1))
    {
		const Audio2::FmodEventId eventId(this->projectId, "effects/enemy_shout");
		this->audioPlayer->PlayEvent(eventId, 1.0f);
		this->debugText = "play: effects/enemy_shout";
    }
	
	// test 3D-sound
	// listener is located at (0, 3, 10) and looks at (0, 0, 0)
	// audiosource is located at (-5, 0, 0), hearable at the left stereo channel
	if (keyboard->KeyDown(Key::Key2))
    {
        const Audio2::FmodEventId eventId(this->projectId, "effects/enemy_appear");
		matrix44 transform = matrix44::translation(-5.0f, 0.0f, 0.0f);
		this->audioPlayer->PlayEvent(eventId, transform, vector(0.0f, 0.0f, 0.0f), 1.0f);
		this->debugText = "play: effects/enemy_appear";
    }
    
	// test "programmer sound"
	// play sound directly from the filesystem
	// change volume over the event
	if (keyboard->KeyDown(Key::Key3))
    {
        this->programmerSoundEmitter->PlayProgrammerSound("..\\..\\work\\audio\\wavs\\crowd_outside_1.wav");
		this->debugText = "play: ../../work/audio/wavs/crowd_outside_1.wav";
    }
	if (keyboard->KeyDown(Key::Key4))
    {
		this->programmerSoundEmitter->SetVolume(0.25f);
    }

	ViewerApplication::OnProcessInput();
}

//------------------------------------------------------------------------------
/**
*/
void
FmodTestApplication::OnUpdateFrame()
{
	Util::String text = "Press '1' to play 2D-sound\n"
		                "Press '2' to play 3D-sound\n"
						"Press '3' to play programmer sound\n"
						"Press '4' to change volume on programmer sound to 0.25f\n";
    _debug_text(text, Math::float2(0.01f, 0.01f), Math::float4(1.0f, 0.5f, 0.0f, 1.0f));
	_debug_text(this->debugText, Math::float2(0.01f, 0.15f), Math::float4(0.0f, 1.0f, 0.0f, 1.0f));

	this->listener->SetTransform(this->camera->GetTransform());
    this->audioServer->OnFrame();
    ViewerApplication::OnUpdateFrame();
}

} // namespace Test