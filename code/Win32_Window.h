#pragma once
/*=====================================================================================
$File: Win32_Window.h
$Date: March 29, 2017
$Creator: Jamie Cooper
$Notice: (C) Copyright 2017 by Dysfunctional Entertainment LLC. All Rights Reserved.
=====================================================================================*/
#ifndef WIN32_WINDOW_H
#define WIN32_WINDOW_H

#include "Includes.h"
#include "Game.h"

string className = "BloodNoir";
string windowTitle = "Blood Noir 1.0";

std::unique_ptr<Game> m_game;

RECT rc;
HWND m_window;

bool InitWindowClass(HINSTANCE program);

LRESULT CALLBACK WndProc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE Program, _In_opt_ HINSTANCE PreviousProgram, _In_ LPWSTR CommandLine, _In_ int ShowCommand);
#endif // !WIN32_WINDOW_H