/*=====================================================================================
$File: AudioSystem.cpp
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#include "AudioSystem.h"

// constructor instantiate the active platform audio system
AudioSystem::AudioSystem()
{
#if _WIN32
	m_win32Audio = make_unique<Win32_Audio>();
#endif
}

// destructor release the active platform's audio system
AudioSystem::~AudioSystem()
{
#if _WIN32
	m_win32Audio.release();
#endif
}

// initialize the active platforms audio system
void AudioSystem::InitializeAudioSystem()
{
#if _WIN32
	m_win32Audio->InitializeAudio();
#endif
}

// called every frame update the audio engine for the active platform
void AudioSystem::AudioSystemUpdate()
{
#if _WIN32
	m_win32Audio->AudioEngineUpdate();
#endif
}

// set the background music to be played through out menus, levels, etc...
void AudioSystem::SetBackgroundMusic(string file)
{
#if _WIN32
	m_win32Audio->SetBackgroundMusic(file);
#endif
}

// add an effect to the container of effects for the active platforms audio system
void AudioSystem::AddEffect(string name, string file)
{
#if _WIN32
	m_win32Audio->AddSoundEffect(name, file);
#endif
}

// play an effect passed in by name
void AudioSystem::PlayEffect(string name)
{
#if _WIN32
	m_win32Audio->PlayEffect(name);
#endif
}

// start playing the background music that is currently set
void AudioSystem::PlayBackgroundMusic()
{
#if _WIN32
	m_win32Audio->StartBackgroundMusic();
#endif
}

// stop playing the background music
void AudioSystem::StopBackgroundMusic()
{
#if _WIN32
	m_win32Audio->StopBackgroundMusic();
#endif
}

// set the volume level of the background music
void AudioSystem::SetBGVolume(float value)
{
#if _WIN32
	m_win32Audio->SetBackgroundVolume(value);
#endif
}

// set the volume level for all effects
void AudioSystem::SetEffectVolume(float value)
{
#if _WIN32
	m_win32Audio->SetSoundEffectVolume(value);
#endif
}