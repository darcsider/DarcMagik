/*=====================================================================================
$File: UserInterface.cpp
$Date: October 27th, 2018
$Creator: Jamie Cooper
=====================================================================================*/
#include "UserInterface.h"

RPGDialogBox::RPGDialogBox()
	:m_boxHeight(0), m_boxWidth(0)
{

}

RPGDialogBox::~RPGDialogBox()
{

}

void RPGDialogBox::BuildDialogBox()
{
	m_tileSize.x = 32;
	m_tileSize.y = 32;
	
	m_textArea.left = 100;
	m_textArea.right = 300;
	m_textArea.top = 100;
	m_textArea.bottom = 300;

	m_boxWidth = 200;
	m_boxHeight = 200;
	m_position.x = 100;
	m_position.y = 100;
	m_sections.x = (m_textArea.right - m_textArea.left) / m_tileSize.x;
	m_sections.y = (m_textArea.bottom - m_textArea.top) / m_tileSize.y;

	RECT sourceRectangle;
	sourceRectangle.left = 0;
	sourceRectangle.right = sourceRectangle.left + m_tileSize.x;
	sourceRectangle.top = 0;
	sourceRectangle.bottom = sourceRectangle.top + m_tileSize.y;

	/*for (int row = 0; row < m_sections.y; row++)
	{
		/*sourceRectangle.left = row * m_tileSize.x;
		sourceRectangle.right = sourceRectangle.left + m_tileSize.x;
		sourceRectangle.top = row * m_tileSize.y;
		sourceRectangle.bottom = sourceRectangle.top + m_tileSize.y;
		RenderManager::GetInstance().RenderObject("MessageBox", sourceRectangle, m_position);
		m_position.x = 100;
		m_position.y = 100 + m_tileSize.y;
		for (int column = 0; column < m_sections.x; column++)
		{
			sourceRectangle.left = m_tileSize.x * column;
			sourceRectangle.right = sourceRectangle.left + m_tileSize.x;
			sourceRectangle.top = row * m_tileSize.y;
			sourceRectangle.bottom = sourceRectangle.top + m_tileSize.y;
			RenderManager::GetInstance().RenderObject("MessageBox", sourceRectangle, m_position);
			m_position.x += (column * m_tileSize.x);
		}
	}*/
}

/*void RPGDialogBox::Clear()
{
	if (m_buffer)
	{
		memset(m_buffer.get(), 0, sizeof(wchar_t) * (m_columns + 1) * m_rows);
	}

	m_currentColumn = m_currentLine = 0;
}

void RPGDialogBox::Render()
{
	wstring LineOfText;
	std::lock_guard<std::mutex> lock(m_mutex);

	float lineSpacing = RenderManager::GetInstance().GetLineSpacing();;

	float x = float(m_textArea.left);
	float y = float(m_textArea.top);

	unsigned int textLine = unsigned int(m_currentLine - m_rows + m_rows + 1) % m_rows;

	for (unsigned int line = 0; line < m_rows; ++line)
	{
		XMFLOAT2 pos(x, y + lineSpacing * float(line));

		if (*m_lines[textLine])
		{
			RenderManager::GetInstance().RenderText(m_lines[textLine], pos);
		}

		textLine = unsigned int(textLine + 1) % m_rows;
	}
}

void RPGDialogBox::ProcessString(const wchar_t* str)
{
	if (!m_lines)
		return;

	float width = float(m_textArea.right - m_textArea.left);

	for (const wchar_t* ch = str; *ch != 0; ++ch)
	{
		if (*ch == '\n')
		{
			IncrementLine();
			continue;
		}

		bool increment = false;

		if (m_currentColumn >= m_columns)
		{
			increment = true;
		}
		else
		{
			m_lines[m_currentLine][m_currentColumn] = *ch;

			auto fontSize = RenderManager::GetInstance().FontMeasureString(m_lines[m_currentLine]);
			if (XMVectorGetX(fontSize) > width)
			{
				m_lines[m_currentLine][m_currentColumn] = L'\0';

				increment = true;
			}
		}

		if (increment)
		{
			IncrementLine();
			m_lines[m_currentLine][0] = *ch;
		}

		++m_currentColumn;
	}
}

void RPGDialogBox::IncrementLine()
{
	if (!m_lines)
		return;

	m_currentLine = (m_currentLine + 1) % m_rows;
	m_currentColumn = 0;
	memset(m_lines[m_currentLine], 0, sizeof(wchar_t) * (m_columns + 1));
}

void RPGDialogBox::Write(string textString)
{
	wstring text = RenderManager::GetInstance().ConvertSTRtoWSTR(textString);
	std::lock_guard<std::mutex> lock(m_mutex);

	ProcessString(text.c_str());
}

void RPGDialogBox::WriteLine(string textString)
{
	wstring text = RenderManager::GetInstance().ConvertSTRtoWSTR(textString);
	std::lock_guard<std::mutex> lock(m_mutex);

	ProcessString(text.c_str());
	IncrementLine();
}

void RPGDialogBox::SetWindow(const RECT& layout)
{
	m_textArea = layout;

	float lineSpacing = RenderManager::GetInstance().GetLineSpacing();
	unsigned int rows = std::max<unsigned int>(1, static_cast<unsigned int>(float(layout.bottom - layout.top) / lineSpacing));

	RECT fontLayout = RenderManager::GetInstance().FontMeasureBounds();
	unsigned int columns = std::max<unsigned int>(1, static_cast<unsigned int>(float(layout.right - layout.left) / float(fontLayout.right - fontLayout.left)));

	std::unique_ptr<wchar_t[]> buffer(new wchar_t[(columns + 1) * rows]);
	memset(buffer.get(), 0, sizeof(wchar_t) * (columns + 1) * rows);

	std::unique_ptr<wchar_t*[]> lines(new wchar_t*[rows]);
	for (unsigned int line = 0; line < rows; ++line)
	{
		lines[line] = buffer.get() + (columns + 1) * line;
	}

	if (m_lines)
	{
		unsigned int c = std::min<unsigned int>(columns, m_columns);
		unsigned int r = std::min<unsigned int>(rows, m_rows);

		for (unsigned int line = 0; line < r; ++line)
		{
			memcpy(lines[line], m_lines[line], c * sizeof(wchar_t));
		}
	}

	std::swap(columns, m_columns);
	std::swap(rows, m_rows);
	std::swap(buffer, m_buffer);
	std::swap(lines, m_lines);
}

void RPGDialogBox::BuildDialogBox(vector<string> text)
{
	m_boxStrings = text;

	for (int i = 0; i < m_boxStrings.size(); i++)
	{
		WriteLine(m_boxStrings[i]);
	}
}

void RPGDialogBox::DisplayDialogBox()
{
	if (m_hasCharImage)
		Render();
}*/