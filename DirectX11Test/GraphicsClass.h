#pragma once
#include <windows.h>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.f;
const float SCRREN_NEAR = 0.1f;

class D3DClass;
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool initialize(int screenWidth, int screenHeight, HWND hwnd);
	void shutdown();
	bool Frame();
private:
	bool Render();
private:
	D3DClass* m_D3D;
};

