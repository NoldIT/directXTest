#include "stdafx.h"
#include "ModelClass.h"



ModelClass::ModelClass()
	:m_vertexBuffer(nullptr)
	,m_indexBuffer(nullptr)
	,m_vertexCount(0)
	,m_indexCount(0)
{
}
ModelClass::ModelClass(const ModelClass&)
{
}


ModelClass::~ModelClass()
{
}

bool ModelClass::initialize(ID3D11Device* device)
{
	bool result;
	// Initialize vertex and index buffer that hold the geometry for the triangle.
	result = initializeBuffer(device);
	return result;
}

void ModelClass::shutdown()
{
	// release vertex and index buffer
	shutdownBuffer();
	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put vertex and index buffers on the graphics pipeline to prepare them for drawing
	renderBuffer(deviceContext);
	return;
}

int ModelClass::getIndexCount()
{
	return m_indexCount;
}

bool ModelClass::initializeBuffer(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array;
	m_vertexCount = 3;
	// set the number of indices in the index array;
	m_indexCount = 3;

	// Create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (nullptr == vertices)
		return false;

	// Create index array
	indices = new unsigned long[m_indexCount];
	if (nullptr == indices)
		return false;

	// Load the vertex array with data
	vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f); // bottom left  
	vertices[0].color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);
	
	vertices[1].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f); // Top middle
	vertices[1].color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);

	vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f); // Bottom right
	vertices[2].color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);

	// Load the index array with data
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	// Set up the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the sub resource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now Create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
		return false;

	// Setup the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the sub resource structure a pointer to the index data;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
		return false;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[]vertices;
	vertices = nullptr;
	delete[]indices;
	indices = nullptr;

	return true;
}

void ModelClass::shutdownBuffer()
{
	// Release index buffer
	if (nullptr != m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	if (nullptr != m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
	return;
}

void ModelClass::renderBuffer(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set the vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assember so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);


	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return;
}