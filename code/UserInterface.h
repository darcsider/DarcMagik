#pragma once
/*=====================================================================================
$File: UserInterface.h
$Date: October 23, 2018
$Creator: Jamie Cooper
=====================================================================================*/
#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include "Includes.h"
#include "RenderManager.h"

class ScreenPanel;

class MessageDialogBox
{
	private:
		string textureName;
		RECT textArea;
};

class ScreenPanel
{
	private:
		int panelWidth;
		int panelHeight;
		int panelWidthSections;
		int panelHeightSections;
		Vector2 panelPosition;
		XMFLOAT4 panelColors[4];
		bool displayPanel;

	public:
		ScreenPanel();
		~ScreenPanel();
		void CreatePanel(int width, int height, Vector2 position, XMFLOAT4 color);
		void CreatePanel(int width, int height, Vector2 position, XMFLOAT4 colors[4]);
		void TogglePannel();
		void DisplayPanel();
};
#endif