#pragma once
/*=====================================================================================
$File: UserInterface.h
$Date: October 27th, 2018
$Creator: Jamie Cooper
=====================================================================================*/
#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include "Includes.h"
#include "RenderManager.h"

// a collection of classes that will be used for various User Interface needs like dialogue boxes, etc...
class RPGDialogBox
{
	protected:
		RECT m_textArea;
		string m_textureName;
		Vector2 m_tileSize;
		int m_boxWidth;
		int m_boxHeight;
		Vector2 m_sections;
		Vector2 m_position;
		/*string						m_fileName;
		RECT							m_textArea;
		bool							m_hasCharImage;
		RECT							m_charImageArea;
		std::unique_ptr<wchar_t[]>		m_buffer;
		std::unique_ptr<wchar_t*[]>		m_lines;
		std::vector<wchar_t>			m_tempBuffer;
		vector<string>					m_boxStrings;
		unsigned int					m_columns;
		unsigned int					m_rows;
		unsigned int					m_currentColumn;
		unsigned int					m_currentLine;
		std::mutex						m_mutex;*/

	public:
		RPGDialogBox();
		~RPGDialogBox();
		void BuildDialogBox();
		/*void Clear();
		void Render();
		void ProcessString(const wchar_t* str);
		void IncrementLine();
		void DisplayDialogBox();
		void Write(string textString);
		void WriteLine(string textString);
		void SetWindow(const RECT& layout);
		void Format(const wchar_t* strFormat, ...);
		void BuildDialogBox(vector<string> text);

		void DisplayToScreen()
		{
			m_hasCharImage = !m_hasCharImage;
		}*/
};
#endif