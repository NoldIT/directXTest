#include "stdafx.h"
#include "ColorShaderClass.h"
#include <D3DCompiler.h>
#include <fstream>

#pragma comment(lib, "D3DCompiler.lib")


ColorShaderClass::ColorShaderClass()
	:m_vertexShader(nullptr)
	, m_pixelShader(nullptr)
	, m_inputLayout(nullptr)
	, m_matrixBuffer(nullptr)
{
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& other)
	:m_vertexShader(nullptr)
	, m_pixelShader(nullptr)
	, m_inputLayout(nullptr)
	, m_matrixBuffer(nullptr)
{
}


ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// initialize vertex and pixel shader
	 result = initializeShader(device, hwnd, L"color.vs", L"color.ps");
	 return result;
}

void ColorShaderClass::shutdown()
{
	shutdownShader();
	return;
}

bool ColorShaderClass::render(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	bool result;

	// Set the shader parameters that it will use for rendering
	result = setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (false == result)
		return false;

	// Now render the prepared buffers with the shader
	renderShader(deviceContext, indexCount);
	return true;
}

bool ColorShaderClass::initializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFile, const WCHAR* psFile)
{
	HRESULT result;
	ID3DBlob* errorMessage = nullptr;
	ID3DBlob* vertexShaderBuffer = nullptr;
	ID3DBlob* pixelShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// compile the vertex shader code.
	result = D3DCompileFromFile(vsFile, NULL, NULL, "ColorVertexShader", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (nullptr != errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, vsFile);
		}
		// if there was nothing in the error message then it simply could not find the shader file itself
		else 
		{
			MessageBox(hwnd, vsFile, L"Missing shader FIle", MB_OK);
		}
		return false;
	}

	// Compile the pixel shader code
	result = D3DCompileFromFile(psFile, NULL, NULL, "ColorPixelShader", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (nullptr != errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, psFile);
		}
		// if there was nothing in the error message then it simply could not find the shader file itself
		else 
		{
			MessageBox(hwnd, psFile, L"Missing shader FIle", MB_OK);
		}
		return false;
	}

	// Create vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
		return false;

	// Create pixel shader from the buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
		return false;

	// Now setup the layout of the data that goes into shader
	// This setup needs to match the VertexType structure in the model class and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get the number of elements in the layout
	numElements = _countof(polygonLayout);

	// Create the vertex input layout 
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_inputLayout);
	if (FAILED(result))
		return false;
	
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader const buffer from within this class;
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
		return false;
	return true;
}

void ColorShaderClass::shutdownShader()
{
	// Release the matrix constant buffer
	if (nullptr != m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}

	// Release the layout
	if (nullptr != m_inputLayout)
	{
		m_inputLayout->Release();
		m_inputLayout = nullptr;
	}

	// Release the pixel shader
	if (nullptr != m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	// Release the vertex shader
	if (nullptr != m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}
	return;
}

void ColorShaderClass::outputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, const WCHAR* shaderFileName)
{
	char* compileError;
	unsigned long bufferSize, i;
	std::ofstream ofs;

	// Get a pointer to the error message text buffer
	compileError = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to
	ofs.open("shader-error.txt");

	// Write out the error message
	for (i = 0; i < bufferSize; ++i)
	{
		ofs << compileError[i];
	}


	// Close the file
	ofs.close();

	// Release the error message
	errorMessage->Release();
	errorMessage = nullptr;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error Compiling shader. Check shader-error.txt for message.", shaderFileName, MB_OK);

	return;
}

bool ColorShaderClass::setShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX project)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Transepose the matrices to prepare them for shader
	XMMatrixTranspose(world);
	XMMatrixTranspose(view);
	XMMatrixTranspose(project);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	// Get a pointer to the data in the constant buffer
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	// Copy the matrices into the constant buffer
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->project = project;

	// Unlock the constant buffer
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	// Finally set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	return true;
}

void ColorShaderClass::renderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout
	deviceContext->IASetInputLayout(m_inputLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	
	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);
	return;
}