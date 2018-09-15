#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class InputClass;
class GraphicsClass;
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool initialize();
	void shutdown();
	void run();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void initializeWIndow(int&, int&);
	void shutdownWIndow();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	InputClass* m_Input;
	GraphicsClass* m_Graphics;
};


