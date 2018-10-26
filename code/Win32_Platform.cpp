/*=====================================================================================
$File: Win32_Platform.cpp
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#include "Win32_Platform.h"
#if _WIN32
bool InitWindowClass(HINSTANCE program)
{
	// fill out the WNDCLASSEX structure for the window
	// setting various properties for the window
	WNDCLASSEX WindowClass;
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = WndProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = program;
	WindowClass.hIcon = NULL;		// Will replace this later with an actual icon
	WindowClass.hCursor = LoadCursor(program, IDC_ARROW);
	WindowClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = className.c_str();
	WindowClass.hIconSm = NULL;		// Will replace this later with a small icon

	// check to see if the window class can be created
	if (RegisterClassEx(&WindowClass) == 0)
	{
		return false;
	}

	int w, h;
	m_game->GetDefaultSize(w, h);

	RECT currentDesktop;

	// get the actual window width and height and pass into the game class
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &currentDesktop);
	if (w < currentDesktop.right || w > currentDesktop.right)
	{
		m_game->SetWindowWidth(currentDesktop.right);
		w = currentDesktop.right;
	}

	if (h < currentDesktop.bottom || h > currentDesktop.bottom)
	{
		m_game->SetWindowHeight(currentDesktop.bottom);
		h = currentDesktop.bottom;
	}

	rc.top = 0;
	rc.left = 0;
	rc.right = static_cast<LONG>(w);
	rc.bottom = static_cast<LONG>(h);

	// set the window RECT to whatever value has been calculated in last step
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// create the window with all information that has been built before this step
	m_window = CreateWindow(
		className.c_str(),
		windowTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		program,
		nullptr);

	// if the handle is null then something went wrong and time to bail
	if (!m_window)
		return false;

	return true;
}

int WINAPI wWinMain(_In_ HINSTANCE Program, _In_opt_ HINSTANCE PreviousProgram, _In_ LPWSTR CommandLine, _In_ int ShowCommand)
{
	UNREFERENCED_PARAMETER(PreviousProgram);
	UNREFERENCED_PARAMETER(CommandLine);
	UNREFERENCED_PARAMETER(ShowCommand);

#ifndef _XBOX
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
		return 1;
#endif

	// make sure this is the only instance of this game running on the computer if not then close application
	const char szUniqueNamedMutex[] = "punchdrunksquirrelgames_bloodnoir";
	HANDLE hHandle = CreateMutex(NULL, TRUE, szUniqueNamedMutex);
	if (ERROR_ALREADY_EXISTS == GetLastError())
	{
		// Program already running somewhere
		return(1); // Exit program
	}

	// create the game object
	m_game = make_unique<Game>();

	// initialize the window class
	if (!InitWindowClass(Program))
		return 1;

	// display the window on the screen
	ShowWindow(m_window, SW_MAXIMIZE);
	SetWindowLongPtr(m_window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(m_game.get()));

	GetClientRect(m_window, &rc);

	// initialize the game
	//if (!m_game->GameInitialize(m_window, rc.right - rc.left, rc.bottom - rc.top))
	if (!m_game->GameInitialize(m_window, 1920, 1080))
		return 0;

	// enter the message loop only leaving it when the Quit message is received
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_game->GameRun();
		}
	}
	// if loop exists then reset game object
	m_game.reset();

	// release things that need to be released and return control to windows
	CoUninitialize();
	ReleaseMutex(hHandle); // Explicitly release mutex
	CloseHandle(hHandle); // close handle before terminating
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	static bool s_in_sizemove = false;
	static bool s_in_suspend = false;
	static bool s_minimized = false;

	auto game = reinterpret_cast<Game*>(GetWindowLongPtr(window, GWLP_USERDATA));

	// process messages accordingly
	switch (message)
	{
		case WM_PAINT:
			hdc = BeginPaint(window, &ps);
			EndPaint(window, &ps);
			break;

		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED)
			{
				if (!s_minimized)
				{
					s_minimized = true;
					if (!s_in_suspend && game)
						game->OnSuspending();
					s_in_suspend = true;
				}
			}
			else if (s_minimized)
			{
				s_minimized = false;
				if (s_in_suspend && game)
					game->OnResuming();
				s_in_suspend = false;
			}
			else if (!s_in_sizemove && game)
			{
				game->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
			}
			break;

		case WM_ENTERSIZEMOVE:
			s_in_sizemove = true;
			break;

		case WM_EXITSIZEMOVE:
			s_in_sizemove = false;
			if (game)
			{
				GetClientRect(window, &rc);

				game->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
			}
			break;

		case WM_GETMINMAXINFO:
		{
			auto info = reinterpret_cast<MINMAXINFO*>(lParam);
			info->ptMinTrackSize.x = 1920;
			info->ptMinTrackSize.y = 1080;
		}
		break;

		case WM_ACTIVATEAPP:
			if (game)
			{
				if (wParam)
				{
					game->OnActivated();
					game->MouseProcess(message, wParam, lParam);
					game->KeyboardProcess(message, wParam, lParam);
				}
				else
				{
					game->OnDeactivated();
				}
			}
			break;

		case WM_POWERBROADCAST:
			switch (wParam)
			{
				case PBT_APMQUERYSUSPEND:
					if (!s_in_suspend && game)
						game->OnSuspending();
					s_in_suspend = true;
					return true;

				case PBT_APMRESUMESUSPEND:
					if (!s_minimized)
					{
						if (s_in_suspend && game)
							game->OnResuming();
						s_in_suspend = false;
					}
					return true;
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			game->MouseProcess(message, wParam, lParam);
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			game->KeyboardProcess(message, wParam, lParam);
			break;

		case WM_CHAR:
			game->CharactersInput(wParam);
			break;
		}
		return DefWindowProc(window, message, wParam, lParam);
}
#endif