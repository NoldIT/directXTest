#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class ModelClass
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool initialize(ID3D11Device* device);
	void shutdown();
	void Render(ID3D11DeviceContext*);

	int getIndexCount();

private:
	bool initializeBuffer(ID3D11Device*);
	void shutdownBuffer();
	void renderBuffer(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	int m_vertexCount;
	int m_indexCount;
};

