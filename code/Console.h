#pragma once
/*=====================================================================================
$File: Console.h
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#ifndef _CONSOLE_H              // Prevent multiple definitions if this 
#define _CONSOLE_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "Includes.h"
#include "RenderManager.h"
#include "InputManager.h"

#pragma warning(disable : 4018)

class Console
{
	private:
		string							m_commandString;		// string holding the command entered in by the user
		string							m_prompt;				// the prompt that displays on the screen
		string							m_textInput;			// the text that is being entered in by the keyboard
		XMFLOAT4						m_backgroundColor;		// the color for the background of the console
		Vector2							m_textPosition;			// position of current line of text on the screen
		Vector2							m_consolePosition;		// position of the console on the screen
		Vector2							m_promptPosition;		// where the prompt will display on the screen
		Vector2							m_startTextPosition;	// position of text that the user is typing in
		RECT							m_textLayout;			// layout of the text within the console window
		unsigned int					m_width;				// width of the console
		unsigned int					m_height;				// height of the console
		float							m_lineSpacing;			// what is the spacing between lines for the active font
		char							m_characterInput;		// recent character entered in (not sure if I'm going to use this)
		bool							m_newline;				// did the end user hit enter? yes=process command, no=keep inputing
		bool							m_visible;				// should the console be displayed or not?
		const int						MAXLINES = 8;			// max number of lines allowed to be in history
		vector<string>					m_history;				// history of commands entered in
		map<string, function<void()>>	m_commandMap;			// map holding the commands and function pointers to
																// the function that is being used to process that command

	public:
		// Constructor
		Console();

		// Destructor
		virtual ~Console();

		// create Singleton Rendering manager to be used in all systems
		static Console& GetInstance()
		{
			static Console* gpSingleton = NULL;
			if (gpSingleton == NULL)
			{
				gpSingleton = new Console();
			}
			return *gpSingleton;
		}

		// Initialize the Console
		bool Initialize(Vector2 startPosition, int width, int height, XMFLOAT4 backColor);

		// draw the console to the screen and any text being typed
		void Draw();

		// hide or show the console window from the screen when button pressed
		void HideShow(bool notUsed, GameActions action)
		{
			if (notUsed)
			{
				m_visible = !m_visible;
				m_textInput.clear();
			}
		}

		// get whether or not the window is currently set to visible
		bool IsVisible()
		{
			return m_visible;
		}

		// get the text currently being typed in and add it to buffer
		void TextInput(WPARAM wParam);

		// add the specific command to the set of commands that can be typed into console
		void AddCommand(string commandName, function<void()> funcPoint);

		// process the specific command being passed in
		void ProcessCommand(std::string CommandName);
};
#endif