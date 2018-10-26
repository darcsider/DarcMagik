#pragma once
/*=====================================================================================
$File: Game.h
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#ifndef GAME_H
#define GAME_H
#include "Includes.h"
#include "GameTiming.h"
#include "RenderManager.h"
#include "World.h"
#include "Console.h"
#include "InputManager.h"
#include "AudioSystem.h"
#include "GameStateManager.h"

class Game
{
	private:
		GameTiming					m_timer;			// main timer used in the game for animations, etc...
		GameTiming					m_eventTimer;

		vector<int>					handles;
		bool						m_appPaused;		// bool that tells if the game is paused or not

		int							m_gameWidth;		// what the game screen's width is supposed to be
		int							m_gameHeight;		// what the game screen's height is supposed to be
		bool						m_gameInitialized;	// is the game initialized and ready to start processing?
		HWND						m_window;			// handle to the window
		
		int							backgroundIndex;

		GameStateManager*			m_stateManager;		// pointer to the MainStateManager for the game
		unique_ptr<AudioSystem>		m_audioSystem;		// Platform specific Audio System for game

	public:
		// constructor sets basic entry point values
		Game();

		// destructor releases systems that need to be released
		~Game();
		
		// initialize the game class taking the handle to the window, as well as the specified width and height
		bool GameInitialize(HWND window, int width, int height);

		// update the game including all systems that need to be updated
		void GameUpdate();

		// run the game, this is called by the platform specific class
		void GameRun();

		// pause the game when requested
		void PauseGame()
		{
			m_appPaused = true;
			m_timer.Stop();
		}

		// un-pause the game when requested
		void UnPauseGame()
		{
			m_appPaused = false;
			m_timer.Start();
		}

		// close the game when input is entered
		// this will probably go away in the form that it is now because
		// once game is further along don't want the player to be able to just close game without
		// prompt to save
		void CloseGame(bool notUsed)
		{
			PostQuitMessage(0);
		}

		// simply a test method will be going away just wanted to verify that I could fire off an effect
		// with an input
		void TestSound(bool notUsed)
		{
			m_audioSystem->PlayEffect("heli");
		}

		// draw the scene to the screen and all systems that need to draw
		void DrawScene();

		// calculate the FPS, and MS per frame
		void CalculateFrameStats();

		// when the window becomes active
		void OnActivated();

		// when the window looses focus
		void OnDeactivated();

		// something happened and the window is not the active window on the platform
		void OnSuspending();

		// window is now active again so begin processing things again
		void OnResuming();

		// the size of the window is changing so update systems with new size
		void OnWindowSizeChanged(int width, int height);

		// process keyboard input passing the values along to the necessary systems
		void KeyboardProcess(UINT message, WPARAM wParam, LPARAM lParam);

		// process individual character input, mainly only used for console window
		void CharactersInput(WPARAM wParam);

		// process mouse input passing it along to the platform specific input processing
		void MouseProcess(UINT message, WPARAM wParam, LPARAM lParam);

		// default size of a window this is the size we would prefer the game to run at.
		void GetDefaultSize(int& width, int& height) const
		{
			width = 1920;
			height = 1080;
		}

		// set the window width to what is passed in
		void SetWindowWidth(int width)
		{
			m_gameWidth = width;
		}

		// get the current window width
		int GetWindowWidth()
		{
			return m_gameWidth;
		}

		// set the window height to what is passed in
		void SetWindowHeight(int height)
		{
			m_gameHeight = height;
		}

		// get the current window height
		int GetWindowHeight()
		{
			return m_gameHeight;
		}
};
#endif // !GAME_H