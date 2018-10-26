#pragma once
/*=====================================================================================
$File: Win32_Audio.h
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#ifndef WIN32_AUDIO_H
#define WIN32_AUDIO_H
#include "Includes.h"

// windows platform specific audio system
class Win32_Audio
{
protected:
	unique_ptr<AudioEngine>			m_audioEngine;		// audio engine object
	unique_ptr<SoundEffect>			m_backgroundMusic;	// sound effect set as the background music in the game
	unique_ptr<SoundEffectInstance> m_backgroundEffect;	// used to be able to play the background music and set properties
	unique_ptr<SoundEffectInstance> m_effect;			// effect variable will hold whatever the current sound effect is
	map<string, SoundEffect>		m_soundEffects;		// container of sound effects tied to a name

public:
	// constructor does nothing right now
	Win32_Audio();
	// destructor does nothing right now
	~Win32_Audio();

	// initialize the windows audio engine
	void InitializeAudio();
	// set the specific file as the background music for the game
	bool SetBackgroundMusic(string fileName);
	// add a sound effect to the container of sound effects
	bool AddSoundEffect(string effectName, string fileName);
	// update the audio engine needed for syncing engine with windows clock and hardware
	void AudioEngineUpdate();
	// play the sound effect that is passed in
	void PlayEffect(string effectName);
	// start playing the background music in the game
	void StartBackgroundMusic();
	// stop playing the background music in the game
	void StopBackgroundMusic();
	// set the volume of the background music
	void SetBackgroundVolume(float value);
	// set the volume for all sound effects in the game
	void SetSoundEffectVolume(float value);

	// method for converting regular string to wstring
	// wstring is needed for some directxTK functionality
	wstring ConvertSTRtoWSTR(string text)
	{
		typedef std::codecvt_utf8<wchar_t> convert_type;
		wstring convertedText;
		wstring_convert<convert_type, wchar_t> converter;

		convertedText = converter.from_bytes(text);
		return convertedText;
	}
};
#endif