/*=====================================================================================
$File: WinAudio.cpp
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#include "Win32_Audio.h"

Win32_Audio::Win32_Audio()
{
	// left blank for now
}

Win32_Audio::~Win32_Audio()
{
	// left blank for now
}

void Win32_Audio::InitializeAudio()
{
	// initialize the audio engine object setting its flags
	AUDIO_ENGINE_FLAGS audioFlags = AudioEngine_Default;
#ifdef _DEBUG
	audioFlags = audioFlags | AudioEngine_Debug;
#endif

	m_audioEngine.reset(new AudioEngine(audioFlags));
}

bool Win32_Audio::SetBackgroundMusic(string fileName)
{
	// reset the background music object and set it equal to the file passed in
	m_backgroundMusic.reset(new SoundEffect(m_audioEngine.get(), ConvertSTRtoWSTR(fileName).c_str()));
	m_backgroundEffect = m_backgroundMusic->CreateInstance();
	return true;
}

bool Win32_Audio::AddSoundEffect(string effectName, string fileName)
{
	// check to make sure that the sound effect being added isn't already part of the collection
	auto soundIndex = m_soundEffects.find(effectName);

	if (soundIndex == m_soundEffects.end())
	{
		// if not then add the sound effect to the container
		wstring file = ConvertSTRtoWSTR(fileName);
		m_soundEffects.insert(map<string, SoundEffect>::value_type(effectName, SoundEffect(m_audioEngine.get(), file.c_str())));
		return true;
	}
	else
	{
		return false;
	}
}

void Win32_Audio::AudioEngineUpdate()
{
	// update the audio engine so that it is still synced with the audio hardware and windows
	if (!m_audioEngine->Update())
	{
		// No audio device is active
		if (m_audioEngine->IsCriticalError())
		{
			MessageBox(NULL, "Something went really wrong audio is broken", "Audio Error", MB_OK);
			return;
		}
	}
}

void Win32_Audio::PlayEffect(string effectName)
{
	// find the sound effect that is being requested
	auto soundIndex = m_soundEffects.find(effectName);

	if (soundIndex != m_soundEffects.end())
	{
		// play the sound effect if it is not currently playing
		m_effect = soundIndex->second.CreateInstance();
		auto soundState = m_effect->GetState();

		if (soundState == STOPPED)
			m_effect->Play();
	}
}

void Win32_Audio::StartBackgroundMusic()
{
	// start playing the background music
	m_backgroundEffect->Play(true);
}

void Win32_Audio::StopBackgroundMusic()
{
	// stop playing the background music
	m_backgroundEffect->Stop();
}

void Win32_Audio::SetBackgroundVolume(float value)
{
	// set the volume specified
	m_backgroundEffect->SetVolume(value);
}

void Win32_Audio::SetSoundEffectVolume(float value)
{
	map<string, SoundEffect>::iterator effectIterator;

	// loop through all sound effects and set the volume for each one to the same value
	for (effectIterator = m_soundEffects.begin(); effectIterator != m_soundEffects.end(); effectIterator++)
	{
		m_effect = effectIterator->second.CreateInstance();
		m_effect->SetVolume(value);
	}
}