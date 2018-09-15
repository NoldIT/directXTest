#include "stdafx.h"
#include "GraphicsClass.h"
#include "D3DClass.h"



GraphicsClass::GraphicsClass()
	:m_D3D(nullptr)
{
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
	:m_D3D(other.m_D3D)
{
}


GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result = false;
	m_D3D = new D3DClass;
	if (nullptr == m_D3D)
		return false;

	// initialize D3D object
	result = m_D3D->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCRREN_NEAR);
	if (false == result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	return true;
}

void GraphicsClass::shutdown()
{
	if (m_D3D)
	{
		m_D3D->shutdown();
		delete m_D3D;
		m_D3D = nullptr;
	}
	return;
}

bool GraphicsClass::Frame()
{
	// render graphic scene.
	bool result = Render();
	return result;
}


bool GraphicsClass::Render()
{
	// clear the buffers to begin the scene.
	m_D3D->beginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// present the rendered scene to the screen
	m_D3D->endScene();
	return true;
}