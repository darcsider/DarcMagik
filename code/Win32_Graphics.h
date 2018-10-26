#pragma once
/*=====================================================================================
$File: Win32_Graphics.h
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#ifndef WIN32_GRAPHICS_H
#define WIN32_GRAPHICS_H
#include "Includes.h"

// windows platform specific graphics class
class Win32_Graphics
{
	protected:
		D3D_FEATURE_LEVEL				m_featureLevel;			// DirectX 11 feature level
		ComPtr<ID3D11Device>			m_d3dDevice;			// DirectX 11 device
		ComPtr<ID3D11Device1>			m_d3dDevice1;			// DirectX 11 device (used to set 11 feature level)
		ComPtr<ID3D11DeviceContext>		m_d3dContext;			// Device Context for DirectX 11
		ComPtr<ID3D11DeviceContext1>	m_d3dContext1;			// Device Context for DirectX 11 (used to set 11 feature level)

		ComPtr<IDXGISwapChain>			m_swapChain;			// swap chain for double buffering
		ComPtr<IDXGISwapChain1>			m_swapChain1;			// swap chain for double buffering (used to set 11 feature level)
		ComPtr<ID3D11RenderTargetView>	m_renderTargetView;		// render target view for screen
		ComPtr<ID3D11DepthStencilView>	m_depthStencilView;		// depth buffer view for depth buffer
		ComPtr<ID3D11Texture2D>			m_depthStencil;			// depth buffer

		ComPtr<ID3D11InputLayout>		m_inputLayout;			// layout for how geometry is passed to render
		UINT							m4xMsaaQuality;			// Multi sampling quality level

		int								m_gameWidth;			// width of the game screen
		int								m_gameHeight;			// height of the game screen
		HWND							m_window;				// handle to the window for game
		bool							graphicsInitialized;	// has the graphics engine been initialized?

		unique_ptr<CommonStates>		m_graphicStates;		// graphics states
		unique_ptr<BasicEffect>			m_effectSystem;			// effects system for basic Vertex and Pixel Shader features
		unique_ptr<EffectFactory>		m_effectFactory;		// for building the effects in DirectXTK
		unique_ptr<SpriteBatch>			m_spriteBatch;			// spriteBatch used for rendering 2D sprites to screen
		unique_ptr<SpriteFont>			m_spriteFont;			// spriteFont used for rendering font to screen

		unique_ptr<PrimitiveBatch<VertexPositionColor>> m_primitiveBatch; // primitive batch DirectXTK used to render 3D

		map<string, ComPtr<ID3D11ShaderResourceView>> m_textures;	// collection of the textures used in the game

	public:
		// constructor used for initializing certain objects
		Win32_Graphics();
		// destructor used for releasing objects
		~Win32_Graphics();

		// initialize the DirectX11 specific stuff
		void InitDirectX11();
		// initialize the graphics system
		bool InitializeGraphics(HWND Window, int width, int height);
		// initialize the DirectXTK specific stuff
		bool InitDirectXTKObjects();

		// add a texture to the collection
		bool AddTexture(string filename, string name);
		// create resources that need to be reset if the window size is changed
		void CreateResources();
		// if the device is lost(window is not in focus) then reset objects that need reset
		void OnDeviceLost();

		// begin drawing a scene 2D Sprites
		void BeginScene();
		// clear the screen before rendering something new
		void ClearScene();
		// present all the stuff that has been set to render
		void PresentScene();
		// various methods for drawing objects to the screen 2D Sprites
		void DrawObject(string textureName);
		void DrawObject(string textureName, Vector2 position);
		void DrawObject(string textureName, RECT sourceRect, Vector2 position = Vector2(0.0, 0.0));
		void DrawObject(string textureName, RECT sourceRect, RECT destRect);
		void DrawQuad(Vector2 position, int width, int height, XMFLOAT4 color);
		void DrawQuad(Vector2 position, int width, int height, XMFLOAT4[]);
		// end the scene before presenting it to the screen
		void EndScene();

		// return what the line spacing is for the currently set sprite font
		float GetLineSpacing()
		{
			return m_spriteFont->GetLineSpacing();
		}

		// draw a text string to the screen at the specific location
		void DrawTextToScreen(string text, Vector2 position);

		// return the texture Description for the texture passed in
		D3D11_TEXTURE2D_DESC GetTextureDesc(string textureName);

		// set the width of the screen
		void SetWidth(int width)
		{
			m_gameWidth = width;
		}

		// return the width of the screen
		int GetWidth()
		{
			return m_gameWidth;
		}

		// set the height of the screen
		void SetHeight(int height)
		{
			m_gameHeight = height;
		}

		// return the height of the screen
		int GetHeight()
		{
			return m_gameHeight;
		}

		// return the DirectX11 Device
		ID3D11Device* GetDirectXDevice()
		{
			return m_d3dDevice.Get();
		}

		// return the DirectX11 Device Context
		ID3D11DeviceContext* GetDirectXContext()
		{
			return m_d3dContext.Get();
		}

		// set the handle for the window
		void SetWindowHandle(HWND Window)
		{
			m_window = Window;
		}

		// return the aspect ration of the window
		float AspectRatio()
		{
			return static_cast<float>(m_gameWidth) / m_gameHeight;
		}

		// check to see if the Graphics Card is NVIDIA or AMD
		bool IsNVIDIAChipset();
		bool IsAMDChipset();

		// used to convert standard string to wstring
		// used in DirectXTK stuff
		wstring ConvertSTRtoWSTR(string text)
		{
			wstring convertedText;
			wstring_convert<std::codecvt_utf8<wchar_t>> converter;

			convertedText = converter.from_bytes(text);
			return convertedText;
		}
};
#endif