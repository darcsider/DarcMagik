/*=====================================================================================
$File: UserInterface.cpp
$Date: October 23, 2018
$Creator: Jamie Cooper
=====================================================================================*/
#include "UserInterface.h"

ScreenPanel::ScreenPanel()
{
	displayPanel = false;
}

ScreenPanel::~ScreenPanel()
{

}

void ScreenPanel::CreatePanel(int width, int height, Vector2 position, XMFLOAT4 color)
{
	if (width > 0)
	{
		panelWidth = width;
	}
	else
	{
		panelWidth = 96;
	}

	if (height > 0)
	{
		panelHeight = height;
	}
	else
	{
		panelHeight = 96;
	}

	panelPosition = position;

	panelColors[0] = color;
	panelColors[1] = color;
	panelColors[2] = color;
	panelColors[3] = color;

	panelWidthSections = panelWidth / 32;
	if (panelWidthSections < 1)
	{
		panelWidthSections = 1;
	}

	panelHeightSections = panelHeight / 32;
	if (panelHeightSections < 1)
	{
		panelHeightSections = 1;
	}
}

void ScreenPanel::CreatePanel(int width, int height, Vector2 position, XMFLOAT4 colors[4])
{
	if (width > 0)
	{
		panelWidth = width;
	}
	else
	{
		panelWidth = 96;
	}

	if (height > 0)
	{
		panelHeight = height;
	}
	else
	{
		panelHeight = 96;
	}

	panelPosition = position;

	panelColors[0] = colors[0];
	panelColors[1] = colors[1];
	panelColors[2] = colors[2];
	panelColors[3] = colors[3];

	panelWidthSections = panelWidth / 32;
	if (panelWidthSections < 1)
	{
		panelWidthSections = 1;
	}

	panelHeightSections = panelHeight / 32;
	if (panelHeightSections < 1)
	{
		panelHeightSections = 1;
	}
}

void ScreenPanel::TogglePannel()
{
	displayPanel = !displayPanel;
}

void ScreenPanel::DisplayPanel()
{
	Vector2 sectionPosition = panelPosition;
	int panelIndex = 0;

	if (displayPanel)
	{
		RenderManager::GetInstance().RenderQuad(panelPosition, panelWidth, panelHeight, panelColors);

	}
}