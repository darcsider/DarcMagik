/*=====================================================================================
$File: Win32_Input.cpp
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#include "Win32_Input.h"

Win32Input::Win32Input()
{
	// initialize keyboard and gamepad objects
	m_keyboard = make_unique<Keyboard>();
	m_gamePad = make_unique<GamePad>();

	// build the defualt bindings for the game
	BuildDefaultBindings();

	// initialize variables to default values
	m_key = Keyboard::Keys::Escape;
}

Win32Input::~Win32Input()
{

}

void Win32Input::BuildDefaultBindings()
{
	// Not final key bindings just for testing purposes!!!
	// TODO: finalize key binding defaults
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionUp, Keyboard::Keys::Up));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionDown, Keyboard::Keys::Down));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionLeft, Keyboard::Keys::Left));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionRight, Keyboard::Keys::Right));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionAccept, Keyboard::Keys::Enter));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionCancel, Keyboard::Keys::LeftShift));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(CharacterAttack, Keyboard::Keys::F));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(CharacterDefense, Keyboard::Keys::G));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(SystemTest, Keyboard::Keys::RightShift));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(SystemExitEarly, Keyboard::Keys::Escape));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(SystemConsole, Keyboard::Keys::OemTilde));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(DirectionMoveUp, Keyboard::Keys::W));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(DirectionMoveDown, Keyboard::Keys::S));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(DirectionMoveLeft, Keyboard::Keys::A));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(DirectionMoveRight, Keyboard::Keys::D));
	
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(ActionUp, XBOXOneUp));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(ActionDown, XBOXOneDown));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(ActionLeft, XBOXOneLeft));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(ActionRight, XBOXOneRight));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(ActionAccept, XBOXOneStart));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(ActionCancel, XBOXOneBack));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(CharacterAttack, XBOXOneA));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(CharacterDefense, XBOXOneB));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(SystemTest, XBOXOneLeftBumper));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(SystemExitEarly, XBOXOneRightBumper));
}

void Win32Input::ClearFunctionPointers()
{
	// clear the function pointer to game actions
	m_gameActionBindings.clear();
}

void Win32Input::ProcessCommands()
{
	// process keyboard and gamepad commands
	ProcessKeyboard();
	ProcessGamePad();
}

void Win32Input::AddGameActionBinding(GameActions action, function<void(bool,GameActions)> funcPoint)
{
	// add the new game action to function pointer binding
	m_gameActionBindings.insert(pair<GameActions, function<void(bool,GameActions)>>(action, funcPoint));
}

void Win32Input::ProcessKeyboard()
{
	// loop through all keyboard actions bindings and check if they are pressed
	// if so then call function pointer and pass in true
	// if not true then still call function pointer and pass in false
	map<GameActions, Keyboard::Keys>::iterator keyboardInputIterator;

	auto keyboardState = Keyboard::Get().GetState();
	m_keyboardTracker.Update(keyboardState);

	for (keyboardInputIterator = m_keyBindings.begin(); keyboardInputIterator != m_keyBindings.end(); keyboardInputIterator++)
	{
		auto funcPointIndex = m_gameActionBindings.find((keyboardInputIterator)->first);

		if (funcPointIndex != m_gameActionBindings.end())
		{
			if ((m_keyboardTracker.IsKeyPressed((keyboardInputIterator)->second)))
			{
				funcPointIndex->second(true, funcPointIndex->first);
			}

			if (m_keyboardTracker.IsKeyReleased((keyboardInputIterator)->second))
			{
				funcPointIndex->second(false, funcPointIndex->first);
			}
		}
	}
}

void Win32Input::ProcessGamePad()
{
	// loop through the game pad bindings and check if they are pressed
	// if true then call function pointer and pass in true
	// if not true then call function pointer and pass in false
	auto state = GamePad::Get().GetState(0);

	map<GameActions, XBOXOneGamePad>::iterator gamepadInputIterator;

	if (state.IsConnected())
	{
		m_gamePadTracker.Update(state);

		for (gamepadInputIterator = m_gpBindings.begin(); gamepadInputIterator != m_gpBindings.end(); gamepadInputIterator++)
		{
			auto funcPointIndex = m_gameActionBindings.find((gamepadInputIterator)->first);

			if ((gamepadInputIterator)->second == XBOXOneUp && m_gamePadTracker.dpadUp == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneUp && m_gamePadTracker.dpadUp == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneDown && m_gamePadTracker.dpadDown == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneDown && m_gamePadTracker.dpadDown == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneLeft && m_gamePadTracker.dpadLeft == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneLeft && m_gamePadTracker.dpadLeft == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneRight && m_gamePadTracker.dpadRight == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneRight && m_gamePadTracker.dpadRight == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneBack && m_gamePadTracker.back == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneBack && m_gamePadTracker.back == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneStart && m_gamePadTracker.start == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneStart && m_gamePadTracker.start == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneLeftBumper && m_gamePadTracker.leftShoulder == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneLeftBumper && m_gamePadTracker.leftShoulder == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneRightBumper && m_gamePadTracker.rightShoulder == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneRightBumper && m_gamePadTracker.rightShoulder == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneA && m_gamePadTracker.a == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneA && m_gamePadTracker.a == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneX && m_gamePadTracker.x == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneX && m_gamePadTracker.x == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneY && m_gamePadTracker.y == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneY && m_gamePadTracker.y == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneB && m_gamePadTracker.b == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneB && m_gamePadTracker.b == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
		}
	}
	else
	{
		m_gamePadTracker.Reset();
	}
}