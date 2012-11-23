//------------------------------------------------------------------------------
//  simpleaudioapplication.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "simpleaudioapplication.h"
#include "math/matrix44.h"
#include "io/ps3/ps3fswrapper.h"
#include "input/gamepad.h"


#define CELL_SDK                "c:/usr/local/cell"
#define BANK_FILE               SYS_HOST_ROOT "/" CELL_SDK "/sample_data/sound/scream/AudioTestSimple.bnk"
//#define BANK_FILE             SYS_HOST_ROOT "/" CELL_SDK "/sample_data/sound/scream/TestBank.bnk"

using namespace Test;
using namespace Audio;
using namespace Math;
using namespace Base;
using namespace CoreAudio;
using namespace Http;

#define TEST_SOUND_START_STOP	0
#define TEST_3D_PAN				0
#define TEST_3D_VOLUME			0
#define TEST_3D_DOPPLER			0
#define TEST_3D_ALL				1

const Resources::ResourceId SimpleAudioApplication::soundBankResource(BANK_FILE);
const CoreAudio::SoundBankId SimpleAudioApplication::soundBankId("simple");

//------------------------------------------------------------------------------
/*
*/
SimpleAudioApplication::SimpleAudioApplication()
{
}

//------------------------------------------------------------------------------
/*
*/
SimpleAudioApplication::~SimpleAudioApplication()
{
}

//------------------------------------------------------------------------------
/*
*/
bool 
SimpleAudioApplication::Open()
{
    this->coreServer = Core::CoreServer::Create();
    this->coreServer->SetCompanyName(Util::StringAtom("Radon Labs GmbH"));
    this->coreServer->SetAppName(Util::StringAtom("PS3 Audio Test Simple"));
    this->coreServer->Open();

    this->assignRegistry = IO::AssignRegistry::Create();
    this->assignRegistry->Setup();

    // setup HTTP server
    this->httpInterface = Http::HttpInterface::Create();
    this->httpInterface->Open();

    this->httpServerProxy = HttpServerProxy::Create();
    this->httpServerProxy->Open();

    this->inputServer = Input::InputServer::Create();
    this->inputServer->Open();

    // setup the audio system
    this->audioInterface = AudioInterface::Create();
    this->audioInterface->Open();
    this->audioServer = AudioServer::Create();
    this->audioServer->Open();
    this->audioPlayer = AudioPlayer::Create();

    // load sound banks
    bool res = this->audioServer->LoadSoundBank(soundBankResource, soundBankId, true);
    n_assert(res);

    // setup a few audio emitters
    this->emitter = AudioEmitter3D::Create();
    this->emitter->Setup();
#if TEST_3D_DOPPLER
	this->emitter->SetTransform(matrix44::translation(10.0f, 0.0f, 0.0f));
#else
    this->emitter->SetTransform(matrix44::translation(0.0f, 0.0f, 0.0f));
#endif
    this->emitter->SetVelocity(vector(0.0f, 0.0f, 0.0f));
#if !TEST_SOUND_START_STOP
    //this->emitter->Play(CueId(CueName("highway"/*"crowd_outside"*/), this->soundBankId));
	this->emitter->Play(CueId(CueName("typewriter"), this->soundBankId));
#endif

    AudioListener::Instance()->SetTransform(matrix44::identity());
    AudioListener::Instance()->SetVelocity(vector(0.0f, 0.0f, 0.0f));

    return true;
}

//------------------------------------------------------------------------------
/*
*/
void 
SimpleAudioApplication::Close()
{
    this->emitter = 0;
    
    this->audioPlayer = 0;

    this->audioServer->Close();
    this->audioServer = 0;

    this->audioInterface->Close();
    this->audioInterface = 0;

    this->inputServer->Close();
    this->inputServer = 0;

    this->httpServerProxy->Close();
    this->httpServerProxy = 0;

    this->httpInterface->Close();
    this->httpInterface = 0;

    this->assignRegistry->Discard();
    this->assignRegistry = 0;

    this->coreServer->Close();
    this->coreServer = 0;
}

//------------------------------------------------------------------------------
/*
*/
void 
SimpleAudioApplication::Run()
{
    // waiting for game pad
    const Ptr<Input::GamePad> &gamePad = inputServer->GetDefaultGamePad(0);
    n_printf("waiting for game pad ... ");
    std::fflush(stdout);

    int state = 0;
    while(true)
    {
        inputServer->BeginFrame();
        inputServer->OnFrame();
        switch(state)
        {
            case 0:
                if(gamePad->IsConnected())
                {
                    ++state;
                    n_printf("connected\n");
                }
                break;

            case 1:								
				if(gamePad->ButtonUp(GamePadBase::CircleButton)) return;
#if TEST_SOUND_START_STOP
                ProcessInput_TestSoundStartStop();
#elif TEST_3D_PAN
                ProcessInput_Test3DPan();
#elif TEST_3D_VOLUME
                ProcessInput_Test3DVolume();
#elif TEST_3D_DOPPLER
                ProcessInput_Test3DDoppler();
#elif TEST_3D_ALL
                ProcessInput_Test3DAll();
#endif
                break;
        }

        this->audioServer->OnFrame();
        this->inputServer->EndFrame();

        this->httpServerProxy->HandlePendingRequests();
    }
}

//------------------------------------------------------------------------------
/*
*/
void SimpleAudioApplication::ProcessInput_TestSoundStartStop()
{
#define USE_CUES 0

#if USE_CUES
    typedef CueId SoundId ;
    typedef CueName SoundName;
#else
    typedef WaveId SoundId ;
    typedef WaveName SoundName;
#endif

    struct ButtonMapping
    {
        GamePadBase::Button btn;
        SoundId soundId;
    };
    static ButtonMapping map[] =
    {
        {GamePadBase::L1Button,         SoundId(SoundName("bang"), soundBankId)},
        {GamePadBase::L3Button,         SoundId(SoundName("cam_clicks"), soundBankId)},
        {GamePadBase::R1Button,         SoundId(SoundName("cling"), soundBankId)},
        {GamePadBase::R3Button,         SoundId(SoundName("crowd_outside"), soundBankId)},
        {GamePadBase::DPadUpButton,     SoundId(SoundName("glass_breaking"), soundBankId)},
        {GamePadBase::DPadDownButton,   SoundId(SoundName("highway"), soundBankId)},
        {GamePadBase::DPadLeftButton,   SoundId(SoundName("keys_drop"), soundBankId)},
        {GamePadBase::DPadRightButton,  SoundId(SoundName("typewriter"), soundBankId)}
    };
    static const int EVENT_COUNT = sizeof(map) / sizeof(ButtonMapping);
	const Ptr<Input::GamePad> &gamePad = inputServer->GetDefaultGamePad(0);
    for(int i = 0; i < EVENT_COUNT; ++i)
    {
        if(gamePad->ButtonDown(map[i].btn))
        {
#if USE_CUES
            this->audioPlayer->PlayCue(map[i].soundId);
#else
            this->audioPlayer->PlayWave(map[i].soundId, 1, 1.0f, 0.0f);
#endif
        }
        if(gamePad->ButtonUp(map[i].btn))
        {
#if USE_CUES
            this->audioPlayer->StopCue(map[i].soundId);
#else
            this->audioPlayer->StopWave(map[i].soundId);
#endif
        }
    }
}

//------------------------------------------------------------------------------
/*
*/
void SimpleAudioApplication::ProcessInput_Test3DPan()
{
	// test 3D - Panning
    // move emitter with left thumb axis
	const Ptr<Input::GamePad> &gamePad = inputServer->GetDefaultGamePad(0);
    this->emitter->SetTransform(matrix44::translation(
            gamePad->GetAxisValue(Input::GamePad::LeftThumbXAxis), 
            0.0f, 
            -gamePad->GetAxisValue(Input::GamePad::LeftThumbYAxis)));
}

//------------------------------------------------------------------------------
/*
*/
void SimpleAudioApplication::ProcessInput_Test3DVolume()
{
	// test 3D - Volume
	const Ptr<Input::GamePad> &gamePad = inputServer->GetDefaultGamePad(0);
	if(gamePad->ButtonUp(GamePadBase::CrossButton))
	{	
		static int emitter_shift_right_scale = 0;
		float x = 1.0f+(emitter_shift_right_scale)*1.0f;
		n_printf( "emitter x: %f\n", x);
		this->emitter->SetTransform(matrix44::translation(x, 0.0f, 0.0f));
		emitter_shift_right_scale = (emitter_shift_right_scale + 1) % 50;
	}
}

//------------------------------------------------------------------------------
/*
*/
void SimpleAudioApplication::ProcessInput_Test3DDoppler()
{
	// ~200 kmH
	const Ptr<Input::GamePad> &gamePad = inputServer->GetDefaultGamePad(0);
	this->emitter->SetVelocity(vector(55.555554f * gamePad->GetAxisValue(Input::GamePad::LeftThumbXAxis), 0.0f, 0.0f));
}

//------------------------------------------------------------------------------
/*
*/
void SimpleAudioApplication::ProcessInput_Test3DAll()
{
	// listener is at [0, 0, 0]
	// emitter moves to [250, 0, -10] and then back to [-250, 0, -10]
	// emitter moves with 200 kmH
	static system_time_t lastTime = -1;
	float delta_t;
	static bool dirLeftToRight = true;
	// 200 Km/H -> 55.55556 m/s;
	static float speed = 55.55555555555556f;
	static const float distance = 250.0f;
	const vector velocity(dirLeftToRight ? speed : -speed, 0.0f, 0.0f);
	static vector pos(0.0f, 0.0f, -10.0f);

	if(-1.0 == lastTime)
	{
		lastTime = sys_time_get_system_time();
		return;
	}
	system_time_t now = sys_time_get_system_time();
	bool print = (ceilf(now/1000000.0f) - ceilf(lastTime/1000000.0f)) > 0.0;
	delta_t = (now - lastTime) / 1000000.0f;
	lastTime = now;

	pos += velocity * delta_t;
	if(dirLeftToRight)
	{
		if(pos.x() > distance)
		{
			dirLeftToRight = !dirLeftToRight;
			n_printf("going left ...\n");
		}
	}
	else
	{
		if(pos.x() < -distance)
		{
			dirLeftToRight = !dirLeftToRight;
			n_printf("going right ...\n");
		}
	}
	if(print)
	{
		n_printf("x: %f\n", pos.x());
	}
	this->emitter->SetTransform(matrix44::translation(pos.x(), pos.y(), pos.z()));
	this->emitter->SetVelocity(velocity);
}
