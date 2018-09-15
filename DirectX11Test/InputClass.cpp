#include "stdafx.h"
#include "InputClass.h"


InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


void InputClass::Initialize()
{
	// initialize all the keys to being released and not pressed
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}
	return;
}

void InputClass::keyDown(unsigned int input)
{
	// if a key is pressed then save that state in the key array
	m_keys[input] = true;
	return;
}

void InputClass::keyUp(unsigned int input)
{
	// if a key is released then clear that state in the key array
	m_keys[input] = false;
	return;
}

bool InputClass::isKeyDown(unsigned int input)
{
	return m_keys[input];
}