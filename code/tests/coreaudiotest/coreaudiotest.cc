//------------------------------------------------------------------------------
//  ps3test.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/types.h"
#include "system/appentry.h"
#include "core/coreserver.h"
#include "coreaudio/audiodevice.h"
#include "input/inputserver.h"
#include "input/gamepad.h"
#include "io/assignregistry.h"
#include "io/ps3/ps3fswrapper.h"


ImplementNebulaApplication();

#define CELL_SDK				"c:/usr/local/cell"
#define BANK_FILE				SYS_HOST_ROOT "/" CELL_SDK "/sample_data/sound/scream/TestBank.bnk"
//#define BANK_FILE				"audio:TestBank.bnk"

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
	Ptr<IO::AssignRegistry> assignRegistry = IO::AssignRegistry::Create();
	assignRegistry->Setup();
	Ptr<CoreAudio::AudioDevice> audioDevice = CoreAudio::AudioDevice::Create();
	audioDevice->Open();

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


	{
		const Resources::ResourceId soundBankResId(BANK_FILE);
		const CoreAudio::SoundBankId soundBankId("testibank");
		const CoreAudio::WaveId waveId(CoreAudio::WaveName("sound0"), soundBankId);
		const CoreAudio::CueId cueId(CoreAudio::CueName("sound0"), soundBankId);

		// load a soundbank
		bool res = audioDevice->LoadSoundBank(soundBankResId, soundBankId, false);
		n_assert(res);
		Ptr<CoreAudio::SoundBank> soundBank = audioDevice->GetSoundBankById(soundBankId);
		n_assert(soundBank.isvalid());

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

		audioDevice->UnloadSoundBank(soundBankId);
	}

	inputServer->Close();
	inputServer = 0;

	audioDevice->Close();
	audioDevice = 0;

	assignRegistry->Discard();
	assignRegistry = 0;

	coreServer = 0;
}
