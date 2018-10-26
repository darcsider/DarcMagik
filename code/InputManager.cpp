/*=====================================================================================
$File: InputManager.cpp
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#include "InputManager.h"

InputManager::InputManager()
{
	// instantiate windows specific input object
#if _WIN32
	m_input = make_unique<Win32Input>();
#endif
}

InputManager::~InputManager()
{
	// left blank for now
}

void InputManager::ProcessCommands()
{
	// process commands on windows specific input system
#if _WIN32
	m_input->ProcessCommands();
#endif
}

void InputManager::ClearFunctionPointers()
{
	// clear pointers on the windows specific input system
#if _WIN32
	m_input->ClearFunctionPointers();
#endif
}

void InputManager::AddKeyboardActionBinding(GameActions action, function<void(bool,GameActions)> funcPoint)
{
	// add a keyboard action binding again this method will most likely only be used on Windows
	// unless linux, and macOS support is added
#if _WIN32
	m_input->AddGameActionBinding(action, funcPoint);
#endif
}