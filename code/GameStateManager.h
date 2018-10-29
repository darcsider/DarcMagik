#pragma once
/*=====================================================================================
$File: GameStateManager.h
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H
#include "Includes.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "World.h"
#include "Actor.h"
#include "Console.h"

class GameStateManager;

/*
GameStates
BannerParadeState - used to show off the company logo as well as any other logos needed like Sony, Microsoft, etc...
MainMenuState - After the BannerParadeState is done it loads into MainMenuState showing the Title Screen and the menu
CoreGameState - This is the state where most of the gameplay will take place, once the player either selects new game
or loads a game save it will load into this state and other systems and objects will be loaded.
*/

// basic abstract virtual class gamestate
// not to be instantiated, must be inherited and built
class GameState
{
	protected:
		GameStateManager *m_stateManager;		// Pointer to State Manager
		string m_textFile;						// text file used to build the state

	public:
		// constructor
		GameState() {}
		// destructor
		virtual ~GameState() {}
		// build the state pure virtual
		virtual void BuildState() = 0;
		// input was received process if the state needs to pure virtual
		virtual void InputCallBack(bool pressed, GameActions action) = 0;
		// setup the input the state needs to handle pure virtual
		virtual void SetupInput() = 0;
		// update the state pure virtual
		virtual void Update(float delta) = 0;
		// execute/ process what needs to be done in this state pure virtual
		virtual void Execute() = 0;
		// when the state is entered / called while active pure virtual
		virtual void OnEnter() = 0;
		// when the state is leaving / being removed pure virtual
		virtual void OnExit() = 0;
};

class BannerParadeState : public GameState
{
	protected:
		map<string, bool> m_banners;	// "banners"/ logo's at beginning of game
		string m_currentBanner;			// what is the current banner on screen
		float m_bannerTimer;			// how long has the current banner been on screen
		float m_bannerDelay;			// how long should the current banner be on screen before moving to next

	public:
		// default constructor does nothing
		BannerParadeState();
		// constructor takes a pointer to the GameStateManager and the filename of text file to build state
		BannerParadeState(GameStateManager *stateManager, string filename);
		// destructor does nothing
		virtual ~BannerParadeState();
		// build the state from the textfile set for the state
		void BuildState();
		// if this state needs to process input it is received here
		virtual void InputCallBack(bool pressed, GameActions action);
		// setup any input commands here
		virtual void SetupInput();
		// update the banner parade state
		virtual void Update(float delta);
		// execute / process what needs done in the banner parade
		virtual void Execute();
		// when the state is entered / called while active pure virtual
		virtual void OnEnter();
		// when the state is leaving / being removed pure virtual
		virtual void OnExit();
};

class MainMenuState : public GameState
{
	protected:
		float m_mainMenuDelay;				// this will be used later to show a gameplay video if player sits at MainMenu to long
		string m_mainMenuBackground;		// string for the main menu background texture
		vector<string> m_menuTextures;		// vector of the text for the menu options and their selected counter parts
		vector<string> m_renderTextures;	// vector of the current textures to be rendered on the screen.
		string m_pressAnyKey;				// text saying press any key to start
		int m_currentSelection;				// currently selected menu option
		bool anyKeyPressed;					// have we moved beyond the press any key screen
		bool initialized;

	public:
		// default constructor does nothing right now
		MainMenuState();
		// constructor takes a pointer to the GameStateManager and the filename of text file to build state
		MainMenuState(GameStateManager *stateManager, string filename);
		// destructor that cleans up the class when the object is being deleted
		virtual ~MainMenuState();
		// build the state from the textfile set for the state
		void BuildState();
		// if this state needs to process input it is received here
		virtual void InputCallBack(bool pressed, GameActions action);
		// setup any input commands here
		virtual void SetupInput();
		// update the banner parade state
		virtual void Update(float delta);
		// execute / process what needs done in the banner parade
		virtual void Execute();
		// when the state is entered / called while active pure virtual
		virtual void OnEnter();
		// when the state is leaving / being removed pure virtual
		virtual void OnExit();
};

class CoreGameState : public GameState
{
	protected:
		GameWorld *m_currentWorld;

	public:
		// default constructor does nothing right now
		CoreGameState();
		// constructor takes a pointer to the GameStateManager and the filename of text file to build state
		CoreGameState(GameStateManager *stateManager, string filename);
		// destructor that cleans up the class when the object is being deleted
		virtual ~CoreGameState();
		// build the state from the textfile set for the state
		void BuildState();
		// if this state needs to process input it is received here
		virtual void InputCallBack(bool pressed, GameActions action);
		// setup any input commands here
		virtual void SetupInput();
		// update the banner parade state
		virtual void Update(float delta);
		// execute / process what needs done in the banner parade
		virtual void Execute();
		// when the state is entered / called while active pure virtual
		virtual void OnEnter();
		// when the state is leaving / being removed pure virtual
		virtual void OnExit();
};

// New Game State in its current shape is not representative of what it will be in final version
// this is here so I can test out certain features as I'm developing them.
class GameStartState : public GameState
{
	protected:
		GameWorld *testWorld;
		Actor *testCharacter;
		Sprite *testCharacterSprite;

	public:
		// default constructor does nothing right now
		GameStartState();
		// constructor takes a function pointer and the filename of text file to build state
		GameStartState(GameStateManager *stateManager, string filename);
		// destructor that cleans up the class when the object is being deleted
		virtual ~GameStartState();
		// build the state from the textfile set for the state
		void BuildState();
		// if this state needs to process input it is received here
		virtual void InputCallBack(bool pressed, GameActions action);
		// setup any input commands here
		virtual void SetupInput();
		// update the banner parade state
		virtual void Update(float delta);
		// execute / process what needs done in the banner parade
		virtual void Execute();
		// when the state is entered / called while active pure virtual
		virtual void OnEnter();
		// when the state is leaving / being removed pure virtual
		virtual void OnExit();
};

class GameStateManager
{
	protected:
		GameState*				m_currentState;			// holds the currentState in the manager
		map<string, GameState*>	m_gameStates;			// collection of GameStates

	public:
		// Constructor set default value for state change left blank otherwise
		GameStateManager();
		// destructor left blank
		~GameStateManager();
		// change the current state to the state type specified
		void ChangeState(string state);
		// add a state to the StateManager
		void AddState(string stateName, GameState* newState);
		// update the current state
		void Update(float delta);
		// execute / process the current state
		void Execute();
};
#endif