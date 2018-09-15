#pragma once

#include <DirectXMath.h>
#include <dxgi.h>
#include <d3d11.h>


class D3DClass
{
public:
	D3DClass();
private:
	D3DClass(const D3DClass&);
public:
	~D3DClass();

	bool initialize(int, int, bool, HWND, bool, float, float);
	void shutdown();

	void beginScene(float, float, float, float);
	void endScene();

	ID3D11Device*  getDevice();
	ID3D11DeviceContext* getDeviceContext();

	void getProjectionMatrix(DirectX::XMMATRIX&);
	void getWorldMatrix(DirectX::XMMATRIX&);
	void getOrthoMatrix(DirectX::XMMATRIX&);

	void getVideoCardInfo(char*, int&);

private:
	bool m_vsynce_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChan;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMMATRIX m_orthMatrix;
};

