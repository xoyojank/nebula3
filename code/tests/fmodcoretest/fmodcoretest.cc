//------------------------------------------------------------------------------
//  fmodcoretest.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/types.h"
#include "system/appentry.h"
#include "core/coreserver.h"
#include "input/inputserver.h"
#include "input/gamepad.h"
#include "io/assignregistry.h"
#include "io/fswrapper.h"
#include "io/gamecontentserver.h"
#include "io/ioserver.h"
#include "audio2/fmodeventprojectid.h"
#include "audio2/fmodeventid.h"


#include "audio2/fmodcoreaudio/fmodaudiodevice.h"
#include "audio2/fmodcoreaudio/fmoderror.h"
#include <../../code/extlibs/fmod/ps3/fmod.hpp>
#include <../../code/extlibs/fmoddesignerapi/ps3/fmod_event.hpp>

#define INPUT_PAD   0

ImplementNebulaApplication();

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const Util::CommandLineArgs& args)
{
	// initialization
    Ptr<Core::CoreServer> coreServer = Core::CoreServer::Create();
	coreServer->SetCompanyName(Util::StringAtom("Radon Labs GmbH"));
    coreServer->SetAppName(Util::StringAtom("PS3Test"));
    coreServer->Open();

    Ptr<IO::GameContentServer> gameContentServer = IO::GameContentServer::Create();
    gameContentServer->SetTitle("RL Test Title");
    gameContentServer->SetTitleId("RLTITLEID");
    gameContentServer->SetVersion("1.00");
    gameContentServer->Setup();

	Ptr<IO::AssignRegistry> assignRegistry = IO::AssignRegistry::Create();
	assignRegistry->Setup();

    Ptr<IO::IoServer> ioServer = IO::IoServer::Create();
    ioServer->MountStandardArchives();

    // initialize FmodCoreAudio
    Ptr<FmodCoreAudio::FmodAudioDevice> audioDevice = FmodCoreAudio::FmodAudioDevice::Create();
	audioDevice->Open();

#if INPUT_PAD
	Ptr<Input::InputServer> inputServer = Input::InputServer::Create();
	inputServer->Open();
	// waiting for game pad
	const Ptr<Input::GamePad> &gamePad = inputServer->GetDefaultGamePad(0);
	n_printf("waiting for game pad ... ");
	std::fflush(stdout);
	while(true)
	{
		inputServer->BeginFrame();
		inputServer->OnFrame();
		inputServer->EndFrame();
		if(gamePad->IsConnected()) break;
	}
	n_printf("connected\n");
#endif


    const Audio2::FmodEventProjectId eventProjectId("first_test");
    const Audio2::FmodEventId eventId(eventProjectId, "eventgroup0/event_psmono");

    FMOD_RESULT result;
#if 0
    audioDevice->LoadEventProject("first_test");
    FMOD::Event *event = audioDevice->GetEvent(eventId);
#else
    FMOD::EventSystem *eventSystem = audioDevice->GetEventSystem();
    FMOD::Event *event;

    FMOD::EventProject *eventProject;
    result = eventSystem->load("first_test.fev", 0, &eventProject);
    FMOD_CHECK_ERROR(result);

    result = eventSystem->getEvent("first_test/eventgroup0/event_psmono", FMOD_EVENT_DEFAULT, &event);
    FMOD_CHECK_ERROR(result);
#endif
    result = event->start();
    FMOD_CHECK_ERROR(result);

    FMOD_EVENT_STATE eventStateLast = -1000000;
    while(true)
    {
        FMOD_EVENT_STATE eventState;
        result = event->getState(&eventState);
        FMOD_CHECK_ERROR(result);
        if(eventStateLast != eventState)
        {
            n_printf("state: ");
            if(eventState & FMOD_EVENT_STATE_READY)             n_printf("FMOD_EVENT_STATE_READY ");
            if(eventState & FMOD_EVENT_STATE_LOADING)           n_printf("FMOD_EVENT_STATE_LOADING ");
            if(eventState & FMOD_EVENT_STATE_ERROR)             n_printf("FMOD_EVENT_STATE_ERROR ");
            if(eventState & FMOD_EVENT_STATE_PLAYING)           n_printf("FMOD_EVENT_STATE_PLAYING ");
            if(eventState & FMOD_EVENT_STATE_CHANNELSACTIVE)    n_printf("FMOD_EVENT_STATE_CHANNELSACTIVE ");
            if(eventState & FMOD_EVENT_STATE_INFOONLY)          n_printf("FMOD_EVENT_STATE_INFOONLY ");
            if(eventState & FMOD_EVENT_STATE_STARVING)          n_printf("FMOD_EVENT_STATE_STARVING ");
            n_printf("\n");
        }        
        eventStateLast = eventState;
        //if(!(eventState & FMOD_EVENT_STATE_CHANNELSACTIVE)) break;
    }
    event->release();

#if 0
	{
        const Resources::ResourceId soundBankResId("audio:bank0.fsb");
		const Audio2::SoundBankId soundBankId("testbank");
		const Audio2::SoundId soundId(Audio2::SoundName("event00"), soundBankId);

		// load a soundbank
		bool res = audioDevice->LoadSoundBank(soundBankResId, soundBankId, false);
		n_assert(res);

		Ptr<FmodCoreAudio::SoundBank> soundBank = audioDevice->GetSoundBankById(soundBankId);
		n_assert(soundBank.isvalid());
        n_printf( "number sound in bank: %d\n", soundBank->GetNumSounds() );

        Ptr<FmodCoreAudio::Sound> sound = soundBank->GetSound("event00");
        n_assert(sound.isvalid());
        sound->Play();
        while(true)
        {

        }

		// play a sound from soundbank
		//Ptr<CoreAudio::Cue> sound = audioDevice->PlayCue(cueId, true, false, Math::matrix44(), Math::vector());
		Ptr<CoreAudio::Wave> sound = audioDevice->PlayWave(waveId, true, 20, 1.0f, 0.0f);

		while(true)
		{
			inputServer->BeginFrame();
			inputServer->OnFrame();

/*
			float vol = gamePad->GetAxisValue(Input::GamePad::LeftThumbXAxis);
			vol += 1.0f;
			vol *= 0.5f;
			sound->SetVolume(vol);

			float pitch = gamePad->GetAxisValue(Input::GamePad::RightThumbXAxis);
			sound->SetPitch(pitch);
*/
			int pan = (int)(gamePad->GetAxisValue(Input::GamePad::LeftThumbXAxis) * 90.0f);
			if(pan < 0 ) pan += 360;
/*
			int pan = (int)(gamePad->GetAxisValue(Input::GamePad::LeftThumbXAxis) * 360.0f);
			if(pan < 0 ) pan += 360;
			if(pan == 360) pan = 0;
*/
			n_assert(pan >= 0);
			n_assert(pan < 360);
			sound->SetPan(pan);

			inputServer->EndFrame();

			audioDevice->OnFrame();		

			if(sound->IsStopped()) break;
		}

        soundBank = 0;
		audioDevice->UnloadSoundBank(soundBankId);
	}
#endif // if 0

#if INPUT_PAD
	inputServer->Close();
	inputServer = 0;
#endif

	audioDevice->Close();
	audioDevice = 0;

    ioServer = 0;

	assignRegistry->Discard();
	assignRegistry = 0;

    gameContentServer->Discard();
    gameContentServer = 0;

    coreServer->Close();
	coreServer = 0;
}
