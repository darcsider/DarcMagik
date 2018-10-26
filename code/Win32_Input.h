#pragma once
/*=====================================================================================
$File: Win32_Input.h
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#ifndef WININPUT_H
#define WININPUT_H
#include "Includes.h"
#include "RenderManager.h"

// enumeration for XBOXOne Game Pads
// used so I can tie Actions to the buttons of the controller
enum XBOXOneGamePad
{
	XBOXOneUp = 1,
	XBOXOneDown = 2,
	XBOXOneLeft = 3,
	XBOXOneRight = 4,
	XBOXDirectionMoveUp = 5,
	XBOXDirectionMoveDown = 6,
	XBOXDirectionMoveLeft = 7,
	XBOXDirectionMoveRight = 8,
	XBOXOneBack = 9,
	XBOXOneStart = 10,
	XBOXOneLeftBumper = 11,
	XBOXOneRightBumper = 12,
	XBOXOneA = 13,
	XBOXOneX = 14,
	XBOXOneY = 15,
	XBOXOneB = 16
};

// enumeration of Mouse Buttons
// used to tie actions to the mouse buttons
enum MouseButtons
{
	LeftButton,
	MiddleButton,
	RightButton
};

// enumeration of Game Actions that
// I can tie keyboard, mouse, and gamepad buttons to
enum GameActions
{
	ActionUp = 1,
	ActionDown = 2,
	ActionLeft = 3,
	ActionRight = 4,
	ActionAccept = 5,
	ActionCancel = 6,
	CharacterAttack = 7,
	CharacterDefense = 8,
	DirectionMoveLeft = 9,
	DirectionMoveRight = 10,
	DirectionMoveUp = 11,
	DirectionMoveDown = 12,
	SystemTest = 13,
	SystemExitEarly = 14,
	SystemConsole = 15
};

// windows platform specific input class
class Win32Input
{
	protected:
		unique_ptr<GamePad> m_gamePad;	// XBOXOne gamepad object
		GamePad::ButtonStateTracker m_gamePadTracker;	// tracker used to get the state of buttons on gamepad

		unique_ptr<Keyboard> m_keyboard;	// directxTK keyboard object
		Keyboard::Keys m_key;				// directTK key object
		Keyboard::KeyboardStateTracker m_keyboardTracker;	// directTK keyboard state tracker

		map<GameActions, XBOXOneGamePad> m_gpBindings;	// collection of actions to gamepad controls bindings

		map<GameActions, Keyboard::Keys> m_keyBindings;	// collection of actions to keyboard keys bindings
		map<GameActions, function<void(bool, GameActions)>> m_gameActionBindings;	// collection of actions to function pointers

		// build a default set of bindings for testing purposes will change to better
		// control scheme
		void BuildDefaultBindings();

	public:
		// constructor initializes DirectXTK input objects
		Win32Input();
		// destructor does nothing currently
		~Win32Input();
		// clear out all currnet function pointers
		void ClearFunctionPointers();
		// process the current set of commands
		void ProcessCommands();

		// add a new game action binding
		void AddGameActionBinding(GameActions action, function<void(bool,GameActions)> funcPoint);

		// process keyboard commands
		void ProcessKeyboard();
		// process gamepad commands
		void ProcessGamePad();

		// if the game looses focus stop reading game pad
		void GamePadSuspend()
		{
			m_gamePad->Suspend();
		}

		// when game regains focus start reading game pad
		void GamePadResume()
		{
			m_gamePad->Resume();
		}

		// get the gamepad object
		GamePad* GetGamePad()
		{
			return m_gamePad.get();
		}
};
#endif