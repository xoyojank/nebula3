//------------------------------------------------------------------------------
//  fmodaudioapplication.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "fmodaudioapplication.h"
#include "math/matrix44.h"
#include "input/gamepad.h"
#include "io/ioserver.h"
#include "util/stringatom.h"
#include "io/fswrapper.h"
#include "audio2/audio2interface.h"
#include "audio2/audio2server.h"
#include "audio2/audio2player.h"
#include "audio2/fmodeventid.h"
#include "audio2/audio2emitter.h"
#include "audio2/audio2listener.h"
#include "audio2/fmodeventcategoryid.h"
#include "audio2/fmodeventparameterid.h"
#include "math/vector.h"
#include "io/logfileconsolehandler.h"
#include "jobs/jobsystem.h"


using namespace Math;
using namespace Base;
using namespace Test;
#if USE_HTTP
using namespace Http;
#endif
using namespace Audio2;


const Audio2::FmodEventProjectId FModAudioApplication::projectIdFirstTest("first_test");
const Audio2::FmodEventProjectId FModAudioApplication::projectIdExamples("examples");

//static const Math::vector listenerPosition(10.0f, 20.0f, -15.0f);
static const Math::vector listenerPosition(0.0f, 0.0f, 0.0f);

//------------------------------------------------------------------------------
/*
*/
FModAudioApplication::FModAudioApplication()
{
}

//------------------------------------------------------------------------------
/*
*/
FModAudioApplication::~FModAudioApplication()
{
}

//------------------------------------------------------------------------------
/*
*/
bool 
FModAudioApplication::Open()
{
    this->coreServer = Core::CoreServer::Create();
    this->coreServer->SetCompanyName(Util::StringAtom("Radon Labs GmbH"));
    this->coreServer->SetAppName(Util::StringAtom("PS3 Audio Test Simple"));
    this->coreServer->Open();

    this->jobSystem = Jobs::JobSystem::Create();
    this->jobSystem->Setup();

    // setup IO
    this->gameContentServer = IO::GameContentServer::Create();
    this->gameContentServer->SetTitle("RL Test Title");
    this->gameContentServer->SetTitleId("RLTITLEID");
    this->gameContentServer->SetVersion("1.00");
    this->gameContentServer->Setup();

	this->assignRegistry = IO::AssignRegistry::Create();
	this->assignRegistry->Setup();

    this->ioServer = IO::IoServer::Create();
    this->ioServer->MountStandardArchives();
    
    // attach a log file console handler
    #if __WIN32__
    Ptr<IO::LogFileConsoleHandler> logFileHandler = IO::LogFileConsoleHandler::Create();
    IO::Console::Instance()->AttachHandler(logFileHandler.upcast<IO::ConsoleHandler>());
    #endif

#if USE_HTTP
    // setup HTTP server
    this->httpInterface = Http::HttpInterface::Create();
    this->httpInterface->Open();
    this->httpServerProxy = HttpServerProxy::Create();
    this->httpServerProxy->Open();
#endif

    this->masterTime.Start();
    this->inputServer = Input::InputServer::Create();
    this->inputServer->Open();

    // setup the audio system
    this->audioInterface = Audio2Interface::Create();
    this->audioInterface->Open();
    this->audioServer = Audio2Server::Create();
    this->audioServer->Open();

    this->audioPlayer = Audio2::Audio2Player::Create();

    const matrix44 listenerTransform = matrix44::translation(listenerPosition);
    Audio2::Audio2Listener::Instance()->SetTransform(listenerTransform);


#if LOAD_PROJECT_FIRST_TEST || LOAD_PROJECT_FIRST_TEST_INSTANCE_TEST
    this->audioServer->LoadEventProject(projectIdFirstTest);
    const Audio2::FmodEventId eventIdPsMono(projectIdFirstTest, "eventgroup0/event_psmono");
#endif
#if LOAD_PROJECT_FIRST_TEST
    this->audioEmitterPsMono = Audio2::Audio2Emitter::Create();
    this->audioEmitterPsMono->Setup(eventIdPsMono);
    //this->audioEmitterPsMono->Set3DAttributes(vector(1.0f, 0.0f, -1.0f), vector::nullvec());
    //this->audioEmitterPsMono->Set3DAttributes(vector(10.0f, 20.0f, -15.0f), vector::nullvec());
    this->emitterWatched = this->audioEmitterPsMono;
#endif
#if LOAD_PROJECT_FIRST_TEST_INSTANCE_TEST
    int index;
    for(index = 0; index < PSMONO_INSTANCE_COUNT; ++index)
    {
        this->audioEmittersPsMonoInstance[index] = Audio2::Audio2Emitter::Create();
        this->audioEmittersPsMonoInstance[index]->Setup(eventIdPsMono);
//        this->audioEmittersPsMonoInstance[index]->Set3DAttributes(vector(
//            (float)(index - (PSMONO_INSTANCE_COUNT/2)), 0.0f, -1.0f), vector::nullvec());
    }
    this->emitterWatched = this->audioEmittersPsMonoInstance[0];
#endif

#if LOAD_PROJECT_EXAMPLES
    this->audioServer->LoadEventProject(projectIdExamples);
    const Audio2::FmodEventId eventIdCar(projectIdExamples, "AdvancedTechniques/Car");
    this->audioEmitterCar = Audio2::Audio2Emitter::Create();
    this->audioEmitterCar->Setup(eventIdCar);
    this->audioEmitterCar->SetParameter("load", 1.0f);
    this->audioEmitterCar->SetParameter("RPM", 0.0f);
    this->emitterWatched = this->audioEmitterCar;
#endif

    return true;
}

//------------------------------------------------------------------------------
/*
*/
void 
FModAudioApplication::Close()
{    
    this->emitterWatched = 0;

#if LOAD_PROJECT_FIRST_TEST
    this->audioEmitterPsMono->Discard();
    this->audioEmitterPsMono = 0;
#endif

#if LOAD_PROJECT_FIRST_TEST_INSTANCE_TEST
    int index;
    for(index = 0; index < PSMONO_INSTANCE_COUNT; ++index)
    {
        this->audioEmittersPsMonoInstance[index]->Discard();
        this->audioEmittersPsMonoInstance[index] = 0;
    }
#endif

#if LOAD_PROJECT_EXAMPLES
    this->audioEmitterCar->Discard();
    this->audioEmitterCar = 0;
#endif

    this->audioPlayer = 0;

#if LOAD_PROJECT_FIRST_TEST || LOAD_PROJECT_FIRST_TEST_INSTANCE_TEST
    this->audioServer->UnloadEventProject(projectIdFirstTest);
#endif
#if LOAD_PROJECT_EXAMPLES
    this->audioServer->UnloadEventProject(projectIdExamples);
#endif

    this->audioServer->Close();
    this->audioServer = 0;

    this->audioInterface->Close();
    this->audioInterface = 0;

    this->inputServer->Close();
    this->inputServer = 0;
    this->masterTime.Stop();

#if USE_HTTP
    this->httpServerProxy->Close();
    this->httpServerProxy = 0;

    this->httpInterface->Close();
    this->httpInterface = 0;
#endif

    this->ioServer->UnmountStandardArchives();
    this->ioServer = 0;

    this->assignRegistry->Discard();
    this->assignRegistry = 0;

    this->gameContentServer->Discard();
    this->gameContentServer = 0;

    this->jobSystem->Discard();
    this->jobSystem = 0;

    this->coreServer->Close();
    this->coreServer = 0;
}

//------------------------------------------------------------------------------
/*
*/
void 
FModAudioApplication::Run()
{
    // waiting for game pad
    const Ptr<Input::GamePad> &gamePad = inputServer->GetDefaultGamePad(0);

    while(true)
    {
        inputServer->BeginFrame();
        inputServer->OnFrame();

        this->inputServer->EndFrame();
        if(!gamePad->IsConnected())
        {
            static bool firstTime = true;
            if(firstTime)
            {
                n_printf("waiting for game pad ... ");
                std::fflush(stdout);
                firstTime = false;
            }
        }
        else
        {
            if(gamePad->ButtonDown(Input::GamePad::StartButton)) return;

            this->ProcessInput_Test();
            //this->ProcessInput_Test3DAll();
        }

        this->audioServer->OnFrame();
#if USE_HTTP
        this->httpServerProxy->HandlePendingRequests();
#endif

        if(this->emitterWatched.isvalid())
        {
            static Audio2::FmodEventState lastState = (Audio2::FmodEventState)0;
            Audio2::FmodEventState state = this->emitterWatched->GetState();
            if(state != lastState)
            {
                n_printf("state\n"
                         "  playing      : %s\n"
                         "  paused       : %s\n"
                         "  voices stolen: %s\n",
                    (state & EventPlaying) ? "yes" : "no",
                    (state & EventPaused) ? "yes" : "no",
                    (state & EventVoicesStolen) ? "yes" : "no");
            }
            lastState = state;
        }

        Core::SysFunc::Sleep(0.01);
    }
}

//------------------------------------------------------------------------------
/*
*/
void FModAudioApplication::ProcessInput_Test()
{
    // waiting for game pad
    const Ptr<Input::GamePad> &gamePad = inputServer->GetDefaultGamePad(0);
    if(!gamePad->IsConnected()) return;

    vector emitterPos(0.0f, 0.0f, 0.0f);   


    static const FmodEventCategoryId category("GUI");
    this->audioServer->EventCategorySetVolume(category, 
        gamePad->GetAxisValue(Input::GamePad::RightThumbYAxis)*0.5f + 0.5f
        );

#if LOAD_PROJECT_FIRST_TEST_INSTANCE_TEST
    if(gamePad->ButtonDown(Input::GamePad::XButton))
    {
        this->audioEmittersPsMonoInstance[0]->Play();
    }
    if(gamePad->ButtonDown(Input::GamePad::YButton))
    {
        this->audioEmittersPsMonoInstance[1]->Play();
    }
    if(gamePad->ButtonDown(Input::GamePad::AButton))
    {
        this->audioEmittersPsMonoInstance[2]->Play();
    }
    if(gamePad->ButtonDown(Input::GamePad::BButton))
    {
        this->audioEmittersPsMonoInstance[3]->Play();
    }
#endif

#if LOAD_PROJECT_FIRST_TEST

    emitterPos.x() =  gamePad->GetAxisValue(Input::GamePad::LeftThumbXAxis);
    emitterPos.y() =  0.0f;
    emitterPos.z() = -gamePad->GetAxisValue(Input::GamePad::LeftThumbYAxis);
    emitterPos += listenerPosition;
    this->audioEmitterPsMono->Set3DAttributes(emitterPos, vector::nullvec());
    if(gamePad->ButtonDown(Input::GamePad::XButton))
    {
        this->audioEmitterPsMono->Play();
    }
    if(gamePad->ButtonUp(Input::GamePad::YButton))
    {
        this->audioEmitterPsMono->Stop();
    }
    if(gamePad->ButtonDown(Input::GamePad::AButton))
    {
        this->audioEmitterPsMono->Pause();
    }
    if(gamePad->ButtonUp(Input::GamePad::BButton))
    {
        this->audioEmitterPsMono->Resume();
    }

#endif

#if LOAD_PROJECT_EXAMPLES
    if(gamePad->ButtonDown(Input::GamePad::AButton))
    {
        this->audioEmitterCar->Play();
    }
    if(gamePad->ButtonUp(Input::GamePad::BButton))
    {
        this->audioEmitterCar->Stop();
    }
    this->audioEmitterCar->SetParameter("RPM", gamePad->GetAxisValue(Input::GamePad::RightTriggerAxis) * 8800.0f);
#endif
}

//------------------------------------------------------------------------------
/*
*/
void FModAudioApplication::ProcessInput_TestSoundStartStop()
{
    /*
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
            //this->audioPlayer->PlayCue(map[i].soundId);
#else
            //this->audioPlayer->PlayWave(map[i].soundId, 1, 1.0f, 0.0f);
#endif
        }
        if(gamePad->ButtonUp(map[i].btn))
        {
#if USE_CUES
            //this->audioPlayer->StopCue(map[i].soundId);
#else
            //this->audioPlayer->StopWave(map[i].soundId);
#endif
        }
    }
*/
}

//------------------------------------------------------------------------------
/*
*/
void FModAudioApplication::ProcessInput_Test3DPan()
{
	// test 3D - Panning
    // move emitter with left thumb axis
/*
    const Ptr<Input::GamePad> &gamePad = inputServer->GetDefaultGamePad(0);
    this->emitter->SetTransform(matrix44::translation(
            gamePad->GetAxisValue(Input::GamePad::LeftThumbXAxis), 
            0.0f, 
            -gamePad->GetAxisValue(Input::GamePad::LeftThumbYAxis)));
*/
}

//------------------------------------------------------------------------------
/*
*/
void FModAudioApplication::ProcessInput_Test3DVolume()
{
/*
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
*/
}

//------------------------------------------------------------------------------
/*
*/
void FModAudioApplication::ProcessInput_Test3DDoppler()
{
	// ~200 kmH
//	const Ptr<Input::GamePad> &gamePad = inputServer->GetDefaultGamePad(0);
//	this->emitter->SetVelocity(vector(55.555554f * gamePad->GetAxisValue(Input::GamePad::LeftThumbXAxis), 0.0f, 0.0f));
}

//------------------------------------------------------------------------------
/*
*/
void FModAudioApplication::ProcessInput_Test3DAll()
{
	// listener is at [0, 0, 0]
	// emitter moves to [250, 0, -10] and then back to [-250, 0, -10]
	// emitter moves with 200 kmH
	static Timing::Time lastTime = -1;
	Timing::Time delta_t;
	static bool dirLeftToRight = true;
	// 200 Km/H -> 55.55556 m/s;
	static float speed = 55.55555555555556f / 100.0f;
	static const float distance = 250.0f / 100.0f;
	const vector velocity(dirLeftToRight ? speed : -speed, 0.0f, 0.0f);
	static vector pos(0.0f, 0.0f, -10.0f / 100.0f);

	if(-1.0 == lastTime)
	{
		lastTime = masterTime.GetTime();// sys_time_get_system_time();
		return;
	}
	Timing::Time now = masterTime.GetTime();//sys_time_get_system_time();
	bool print = (ceilf((float)now) - ceilf((float)lastTime)) > 0.0;
	delta_t = (now - lastTime);
	lastTime = now;

    pos += velocity * (Math::scalar)delta_t;
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

#if LOAD_PROJECT_FIRST_TEST
    this->audioEmitterPsMono->Set3DAttributes(pos, velocity);
#endif
}
