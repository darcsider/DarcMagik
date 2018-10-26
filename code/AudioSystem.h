#pragma once
/*=====================================================================================
$File: AudioSystem.h
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H
#include "Win32_Audio.h"
class AudioSystem
{
	private:
		unique_ptr<Win32_Audio> m_win32Audio;
		//PS4Audio m_ps4Audio;
		//XBOXONE m_XBONEAudio;

	public:
		// constructor initialize the active platform audio system
		AudioSystem();
		// destructor release the active platform audio system
		~AudioSystem();

		// initialize the active platforms audio system
		void InitializeAudioSystem();
		// called every frame to update the audio system so it stays in sync
		void AudioSystemUpdate();

		// set a background music to be played during menus, levels, etc...
		void SetBackgroundMusic(string file);
		// add an effect that can be played back when requested
		void AddEffect(string name, string file);
		// player the specific effect passed in by name
		void PlayEffect(string name);
		// start playing the background music
		void PlayBackgroundMusic();
		// stop playing the background music
		void StopBackgroundMusic();
		// set the volume level of the background music
		void SetBGVolume(float value);
		// set the volume level of effects this will change it for all effects
		void SetEffectVolume(float value);
};
#endif // !AUDIOSYSTEM_H