/*=====================================================================================
$File: Console.cpp
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#include "Console.h"

Console::Console()
{
	// left blank for now
}

Console::~Console()
{
	// left blank for now
}

// initialize the console window and setup initial input show/hide of the console window
bool Console::Initialize(Vector2 startPosition, int width, int height, XMFLOAT4 backColor)
{
	m_consolePosition = startPosition;
	m_width = width;
	m_height = height;
	m_backgroundColor = backColor;
	m_lineSpacing = RenderManager::GetInstance().GetLineSpacing();
	m_promptPosition = Vector2((startPosition.x + 10), (startPosition.x + 10));
	m_textPosition = Vector2(m_promptPosition.x, (m_promptPosition.y + m_lineSpacing));
	m_startTextPosition = Vector2((m_promptPosition.x + 10), m_promptPosition.y);
	m_textInput.clear();
	m_newline = false;
	m_visible = false;
	m_commandMap.clear();
	m_commandString.clear();
	m_prompt.clear();
	m_history.clear();

	m_prompt = "|:>";

	function<void(bool, GameActions)> funcPoint = bind(&Console::HideShow, this, placeholders::_1, placeholders::_2);
	InputManager::GetInstance().AddKeyboardActionBinding(SystemConsole, funcPoint);

	return true;
}

// add a command to the container of commands for the cnosole window
// ties a function pointer to a string to define what the command does
void Console::AddCommand(string commandName, function<void()> funcPoint)
{
	m_commandMap.insert(pair<string, function<void()>>(commandName, funcPoint));
}

// process the command passed in, check to see if the command even exists and if
// so then call the function pointer associated with it.
void Console::ProcessCommand(string commandName)
{
	if (!commandName.empty())
	{
		auto functionIndex = m_commandMap.find(commandName);
		if (functionIndex != m_commandMap.end())
		{
			functionIndex->second();
		}
	}
}

// draw the console window to the screen
void Console::Draw()
{
	string consoleLine;
	if (m_visible)
	{
		// draw a quad to the screen at the specific coordinates
		RenderManager::GetInstance().RenderQuad(m_consolePosition, m_width, m_height, m_backgroundColor);

		// set the textPosition based on the promptPosition and linespacing
		m_textPosition = Vector2(m_promptPosition.x, (m_promptPosition.y + m_lineSpacing));

		// if the history of text commands isnt empty draw all previous commands that fit
		// on the screen to the area below the text entry prompt
		if (!m_history.empty())
		{
			for (int line = 0; line < m_history.size(); ++line)
			{
				m_textPosition = Vector2(m_promptPosition.x, (m_promptPosition.y + m_lineSpacing));
				m_textPosition.y += (m_lineSpacing * float(line));

				consoleLine = m_history[line];
				consoleLine.resize(remove_if(consoleLine.begin(), consoleLine.end(), [](char x) {return !isalnum(x) && !isspace(x);}) - consoleLine.begin());
				m_history[line] = consoleLine;
				RenderManager::GetInstance().RenderText(m_history[line], m_textPosition);
			}
		}

		m_prompt = "|:>";
		m_prompt += m_textInput;

		// draw whatever text needs to be drawn to the screen and then end the scene in order
		// to push the text to the screen
		consoleLine = m_prompt;
		consoleLine.resize(remove_if(consoleLine.begin(), consoleLine.end(), [](char x) {return !isalnum(x) && !isspace(x); }) - consoleLine.begin());
		m_prompt = consoleLine;
		RenderManager::GetInstance().RenderText(m_prompt, m_promptPosition);
	}
}

// process the text being typed in from the keyboard when the console window is open
void Console::TextInput(WPARAM wParam)
{
	static int element = 0;

	// process newline input correctly
	if (m_newline)
	{
		m_textInput.clear();
		m_newline = false;
	}

	// check for backspace in order to delete character
	if (wParam == '\b')
	{
		if (m_textInput.length() > 0)
			m_textInput.erase(m_textInput.size() - 1);
	}

	// check for Escape button in order to clear the text being entered
	if (wParam == Keyboard::Escape)
	{
		m_textInput = "";
	}

	if (wParam != Keyboard::Escape)
	{
		m_textInput += wParam;
	}

	// look for a newline character meaning the person pressed enter
	if ((char)wParam == '\r')
	{
		m_textInput.erase(m_textInput.size() - 1);
		ProcessCommand(m_textInput);
		m_prompt = "|:>";
		if (m_history.size() < MAXLINES && !m_textInput.empty())
			m_history.push_back(m_textInput);
		else if (m_history.size() >= MAXLINES && !m_textInput.empty())
		{
			m_history.erase(m_history.begin());
			m_history.push_back(m_textInput);
		}
		m_textInput.clear();
		m_newline = true;
	}
}