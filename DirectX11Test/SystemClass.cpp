#include "stdafx.h"
#include "SystemClass.h"
#include "InputClass.h"
#include "GraphicsClass.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

SystemClass::SystemClass()
	:
	m_Input(nullptr),
	m_Graphics(nullptr)
{
}

SystemClass::SystemClass(const SystemClass& other)
	:
	m_Input(other.m_Input),
	m_Graphics(other.m_Graphics)
{
}


SystemClass::~SystemClass()
{
}

bool SystemClass::initialize()
{
	int screenSizeWidth = 0;
	int screenSizeHeight = 0;
	bool result = false;

	// initialize the windows API
	initializeWIndow(screenSizeWidth, screenSizeHeight);

	// create the input object. This object will be used to handle reading the keyboard input from the use;
	m_Input = new InputClass;
	if (nullptr == m_Input)
		return false;

	// initialize the input object
	m_Input->Initialize();

	// create graphic object. This object will handle rendering all the graphics for this application
	m_Graphics = new GraphicsClass;
	if (nullptr == m_Graphics)
		return false;

	// initialize graphic object.
	result = m_Graphics->initialize(screenSizeWidth, screenSizeHeight, m_hwnd);
	if (false == result)
		return false;
	return true;
}

void SystemClass::shutdown()
{
	// Release graphics object.
	if (nullptr != m_Graphics)
	{
		m_Graphics->shutdown();
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	// Release input object;
	if (nullptr != m_Input)
	{
		delete m_Input;
		m_Input = nullptr;
	}

	// shutdown window
	shutdownWIndow();
	return;
}

void SystemClass::run()
{
	MSG msg;
	bool done = false;
	bool result = false;

	ZeroMemory(&msg, sizeof(msg));
	while (false == done)
	{
		// Handle the window message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// if window signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing 
			result = Frame();
			if (false == result)
			{
				done = true;
			}
		}
	}
	return;
}

bool SystemClass::Frame()
{
	bool result = false;

	// Check if the user pressed escape and wants to exit the application
	if (m_Input->isKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Do the frame processing for the graphics object
	result = m_Graphics->Frame();
	return result;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard
	case WM_KEYDOWN:
	{
		// if a key is pressed send it to the input object so it can record that state
		m_Input->keyDown((unsigned int)wparam);
		return 0;
	}
	case WM_KEYUP:
	{
		// if a key is pressed send it to the input object so it can record that state
		m_Input->keyUp((unsigned int)wparam);
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

static SystemClass* ApplicationHandle = nullptr;

void SystemClass::initializeWIndow(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get an external pointer to this object;	
	ApplicationHandle = this;

	// Get the instance of this application
	m_hinstance = GetModuleHandle(NULL);
	
	// Give the application name
	m_applicationName = L"Engine";

	// set the windows class with default setting
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register window class
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// setup the screen setting depending on whether it is running in full screen or in windowed mode
	if (FULL_SCREEN)
	{
		// if full screen set the screen to maximun size of the users desktop and 32bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSHEIGHT | DM_PELSWIDTH;

		// change the display setting to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		// Set the position of the window left top.
		posX = posY = 0;
	}
	else
	{
		// if windowed then set it to 800*600 resolution.
		screenWidth = 800;
		screenHeight = 600;
		// place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// create the window with the screen setting and get the handle to it
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor
	ShowCursor(false);

	return;
}

void SystemClass::shutdownWIndow()
{
	// Show mouse cursor
	ShowCursor(true);

	// FIx the display setting if leaving full screen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}
	
	// Remove the window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;
	// Remove the application instance
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class;
	ApplicationHandle = NULL;
	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// check if the window is being destroyed
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

		// check if the window is being closed
	case WM_QUIT:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	return 0;
}