/*=====================================================================================
$File: Game.cpp
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#include "Game.h"

// constructor set default values
Game::Game() :
	m_window(0),
	m_gameWidth(1920),
	m_gameHeight(1080),
	m_gameInitialized(false),
	m_appPaused(false)
{

}

// destructor release systems that need releasing when things close
Game::~Game()
{
	m_audioSystem.release();
}

// initialize the game and all systems that need initialized
bool Game::GameInitialize(HWND window, int width, int height)
{
	// reset the timer to the current clock time
	m_timer.Reset();
	// set the window handle to what is passed in
	m_window = window;

	// set the game width and height but make sure it is at least 1
	m_gameWidth = max(width, 1);
	m_gameHeight = max(height, 1);

	// initialize the graphics system with the window handle and the current width and height
	RenderManager::GetInstance().InitializeGraphics(m_window, width, height);

	// initialize the audio system
	m_audioSystem = make_unique<AudioSystem>();
	m_audioSystem->InitializeAudioSystem();

	// test code right now these lines will be leaving this part most likely and be moving to the game state to be
	// set when the first state starts
	m_audioSystem->SetBackgroundMusic("..\\Music\\Electro.wav");
	m_audioSystem->AddEffect("heli", "..\\Music\\heli.wav");

	// startup the state manager and build all states
	m_stateManager = new GameStateManager();
	m_stateManager->AddState("BannerParade", new BannerParadeState(m_stateManager, "..\\data\\BannerParade.txt"));
	m_stateManager->AddState("MainMenu", new MainMenuState(m_stateManager, "..\\data\\MainMenu.txt"));
	m_stateManager->AddState("GameStart", new GameStartState(m_stateManager, "..\\data\\NewGame.txt"));
	m_stateManager->ChangeState("BannerParade");

	// if it reaches this point then simply set that the game is initialized and move on
	//m_audioSystem->PlayBackgroundMusic();
	m_gameInitialized = true;
	return true;
}

void Game::GameUpdate()
{
	// update all systems that need to be updates
	m_audioSystem->AudioSystemUpdate();
	InputManager::GetInstance().ProcessCommands();
	m_stateManager->Update(m_timer.DeltaTime());
}

void Game::DrawScene()
{
	// clear the screen and begin drawing sprites and other
	// objects to the screen
	RenderManager::GetInstance().ClearScene();
	RenderManager::GetInstance().BeginScene();
	m_stateManager->Execute();
	RenderManager::GetInstance().EndScene();
	Console::GetInstance().Draw();
	RenderManager::GetInstance().PresentScene();
}

void Game::GameRun()
{
	// update the timer
	m_timer.Tick();

	// if the game has been initialized then begin updating and drawing to the screen
	if (m_gameInitialized)
	{
		if (!m_appPaused)
		{
			GameUpdate();
			DrawScene();
			CalculateFrameStats();
		}
		else
		{
			Sleep(100);
		}
	}
}

void Game::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

#if	WIN32 && _DEBUG
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	static string windowTitle = "DarcMagik 1.0";

	frameCnt++;

	// Compute averages over one second period.
	if ((m_timer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		string output = windowTitle +
			" FPS: " + to_string(fps) +
			" Frame Time: " + to_string(mspf) +
			" (ms)";

		SetWindowText(m_window, output.c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
#endif
}

void Game::KeyboardProcess(UINT message, WPARAM wParam, LPARAM lParam)
{
	// pass on any keyboard input to the DirectXTK Input processing
	Keyboard::ProcessMessage(message, wParam, lParam);
}

void Game::CharactersInput(WPARAM wParam)
{
	Console::GetInstance().TextInput(wParam);
}

void Game::MouseProcess(UINT message, WPARAM wParam, LPARAM lParam)
{
	// pass on any mouse input to the DirectXTK Input processing
	Mouse::ProcessMessage(message, wParam, lParam);
}

void Game::OnActivated()
{
	// the game is active again / the window is active again
	// begin processing the gamepad if needed, start the timer again
	// and set paused to false
	InputManager::GetInstance().GamePadResume();
	m_timer.Start();
	m_appPaused = false;
}

void Game::OnDeactivated()
{
	// the game is not active / the window is not active
	// stop processing game pad, stop the timer and pause the game
	InputManager::GetInstance().GamePadSuspend();
	m_timer.Stop();
	m_appPaused = true;
}

void Game::OnSuspending()
{
	// the game is not active / the window is not active
	// stop processing game pad, stop the timer and pause the game
	InputManager::GetInstance().GamePadSuspend();
	m_timer.Stop();
	m_appPaused = true;
}

void Game::OnResuming()
{
	// the game is active again / the window is active again
	// begin processing the gamepad if needed, start the timer again
	// and set paused to false
	InputManager::GetInstance().GamePadResume();
	m_timer.Start();
	m_appPaused = false;
}

void Game::OnWindowSizeChanged(int width, int height)
{
	// if window size changes make sure its not smaller than 1 in both directions
	// then update the graphics system
	m_gameWidth = max(width, 1);
	m_gameHeight = max(height, 1);
	RenderManager::GetInstance().OnWindowSizeChange();
}