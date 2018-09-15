#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class ColorShaderClass
{
private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX project;
	};
public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);
	~ColorShaderClass();

	bool initialize(ID3D11Device* device, HWND hwnd);
	void shutdown();
	bool render(ID3D11DeviceContext* deviceContext, int, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);

private:
	bool initializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* , const WCHAR*);
	void shutdownShader();
	void outputShaderErrorMessage(ID3DBlob*, HWND hwnd, const WCHAR*);

	bool setShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);
	void renderShader(ID3D11DeviceContext* deviceContext, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_inputLayout;
	ID3D11Buffer* m_matrixBuffer;
};

