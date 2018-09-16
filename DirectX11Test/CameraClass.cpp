#include "stdafx.h"
#include "CameraClass.h"


CameraClass::CameraClass(const CameraClass& other)

{
}

CameraClass::CameraClass()
{
	m_positionX = 0.f;
	m_positionY = 0.f;
	m_positionZ = 0.f;
	m_rotationX = 0.f;
	m_rotationY = 0.f;
	m_rotationZ = 0.f;
}


CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float posX, float posY, float posZ)
{
	m_positionX = posX;
	m_positionY = posY;
	m_positionZ = posZ;
	return;
}

void CameraClass::SetRotation(float rotX, float rotY, float rotZ)
{
	m_rotationX = rotX;
	m_rotationY = rotY;
	m_rotationZ = rotZ;
	return;
}

DirectX::XMFLOAT3 CameraClass::GetPosition()
{
	return DirectX::XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

DirectX::XMFLOAT3 CameraClass::GetRotation()
{
	return DirectX::XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Render()
{
	DirectX::XMFLOAT3 up, position, lookAt;
	float yaw, pitch, roll;
	DirectX::XMMATRIX rotationMatrix;

	// Setup the vector that point upwards.
	up.x = 0.f;
	up.y = 1.f;
	up.z = 0.f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.f;
	lookAt.y = 0.f;
	lookAt.z = 1.f;

	// Set the yaw(y) pitch(x) roll (z) rotations in radian
	yaw   = DirectX::XMConvertToRadians(m_rotationX);
	pitch = DirectX::XMConvertToRadians(m_rotationY);
	roll  = DirectX::XMConvertToRadians(m_rotationZ);

	// Create the view matrix from the yaw, pitch, roll values.
	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform up vector and lookAt by the rotation matrix so the view is correctly rotated at the origin.
	DirectX::XMVECTOR upV;
	upV = DirectX::XMLoadFloat3(&up);
	DirectX::XMVector3TransformCoord(upV, rotationMatrix);

	DirectX::XMVECTOR lookAtV;
	lookAtV = DirectX::XMLoadFloat3(&lookAt);
	DirectX::XMVector3TransformCoord(lookAtV, rotationMatrix);

	// Translate the rotated camera to the location of the viewer.
	lookAt.x += position.x;
	lookAt.y += position.y;
	lookAt.z += position.z;

	DirectX::XMVECTOR positionV;
	positionV = DirectX::XMLoadFloat3(&position);
	m_viewMatrix = DirectX::XMMatrixLookAtLH(positionV, lookAtV, upV);

	return;
}

void CameraClass::GetViewMatrix(DirectX::XMMATRIX& out)
{
	out = m_viewMatrix;
	return;
}