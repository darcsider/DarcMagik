#pragma once
/*=====================================================================================
$File: RenderManager.h
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H
#include "Includes.h"
#include "Win32_Graphics.h"

class RenderManager
{
	private:
		unique_ptr<Win32_Graphics>	m_win32_Graphics;	// windows platform specific graphics object
		int							m_gameWidth;		// width of the game screen
		int							m_gameHeight;		// height of the game screen
		//unique_ptr<PS4Graphics>	m_ps4_Graphics;		// PS4 platform specific graphics object
		//unique_ptr<XBONEGraphics> m_XBONEGraphics;	// XBOXONE platform specific graphics object
		bool						m_RenderReady;		// Rendering has been initialized and ready to start

	public:
		// constructor build platform specific graphics objects
		RenderManager();
		// destructor destroy platform specific graphics objects
		~RenderManager();

		// create Singleton Rendering manager to be used in all systems
		static RenderManager& GetInstance()
		{
			static RenderManager* gpSingleton = NULL;
			if (gpSingleton == NULL)
			{
				gpSingleton = new RenderManager();
			}
			return *gpSingleton;
		}

		// initialize the graphics systems this may be windows specific
		// there may be a version of this that is used for other platforms
		void InitializeRendering();
		void InitializeGraphics(HWND Window, int width, int height);

		// Windows only possibly? will figure out how to do similar thing
		// for other platforms
		void BeginScene();
		void EndScene();
		void ClearScene();
		void PresentScene();

		// add a texture to the container of textures on the specific platform
		void AddTexture(string filename, string name);
		
		// various Render methods overides
		void RenderObject(string name, Vector2 position = Vector2(0.0, 0.0));
		void RenderObject(string name, RECT sourceRect, Vector2 position);
		void RenderObject(string name, RECT sourceRect, RECT destRect);
		void RenderText(string text, Vector2 position);

		// platform specific if the size of the window changes
		// this will only be used on Windows, and possibly linux and macOS
		void OnWindowSizeChange();

		void RenderQuad(Vector2 position, int width, int height, XMFLOAT4 color);
		void RenderQuad(Vector2 position, int width, int height, XMFLOAT4 colors[4]);

		// windows specific get the texture Description for a specific texture
		// will probably write a version similar for other platforms as needed
		D3D11_TEXTURE2D_DESC GetTextureDesc(string name);
		float GetLineSpacing();
		XMVECTOR FontMeasureString(string text);
		RECT FontMeasureBounds()
		{
#if _WIN32
			return m_win32_Graphics->FontMeasureBounds();
#endif
			return RECT();
		}

		// check to see if it is a NVIDIA chip or AMD
		bool IsNVIDIAChipset();
		bool IsAMDChipset();

		// return the GameWidth
		int GetGameWidth()
		{
			return m_gameWidth;
		}

		// return the GameHeight
		int GetGameHeight()
		{
			return m_gameHeight;
		}

		// return true if the graphics system has been initialized and rendering can now be done.
		bool IsReadyToRender()
		{
			return m_RenderReady;
		}

		wstring ConvertSTRtoWSTR(string text)
		{
#if _WIN32
			return m_win32_Graphics->ConvertSTRtoWSTR(text);
#endif
			return wstring();
		}
};
#endif