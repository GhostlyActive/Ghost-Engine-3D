/*
	MIT License

	Ghost Engine 3D (https://github.com/GhostlyActive/Ghost-Engine-3D)

	Copyright (c) 2021, GhostlyActive

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "Input.h"


Input::Input()
{
	// init all keys as being released
	for (int i = 0; i < 256; i++) 
	{
		m_keys[i] = 0;
	}
}

void Input::KeyDown(unsigned int value)
{
	// when key is pressed -> position = 1
	m_keys[value] = 1;
}

void Input::KeyUp(unsigned int value)
{
	// when release -> position = 0
	m_keys[value] = 0;
}


void Input::MouseDown(int posX, int posY)
{
	Beep(posX*posY, 3);
}

// check if movement should be done
void Input::Update(float time)
{
	ForwardMove(time);
	BackwardMove(time);
	HorizontalLeftMove(time);
	HorizontalRightMove(time);
	UpMove(time);
	DownMove(time);
	LeftRotateMove(time);
	RightRotateMove(time);
	UpRotateMove(time);
	DownRotateMove(time);
}

void Input::ForwardMove(float time) 
{
	if (m_keys['W'] == 1)
	{

		m_forwardSpeed += time * 0.01f;

		if (m_forwardSpeed > (time * 0.3f))
		{
			m_forwardSpeed = time * 0.3f;
		}
	}

	else
	{
		m_forwardSpeed -= time * 0.006f;

		if (m_forwardSpeed < 0.0f)
		{
				m_forwardSpeed = 0.0f;
		}
	}

	m_posZ += cosf(m_rotY) * 3.141 * m_forwardSpeed;
	m_posX += sinf(m_rotY) * 3.141 * m_forwardSpeed;
}

void Input::BackwardMove(float time)
{
	if (m_keys['S'] == 1)
	{

		m_backwardSpeed += time * 0.01f;

		if (m_backwardSpeed > (time * 0.3f))
		{
			m_backwardSpeed = time * 0.3f;
		}
	}

	else
	{
		m_backwardSpeed -= time * 0.006f;

		if (m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	m_posZ -= cosf(m_rotY) * 3.141 * m_backwardSpeed;
	m_posX -= sinf(m_rotY) * 3.141 * m_backwardSpeed;
}


void Input::HorizontalLeftMove(float time)
{
	if (m_keys['A'] == 1)
	{

		m_horizontalLeftSpeed += time * 0.01f;

		if (m_horizontalLeftSpeed > (time * 0.3f))
		{
			m_horizontalLeftSpeed = time * 0.3f;
		}
	}

	else
	{
		m_horizontalLeftSpeed -= time * 0.006f;

		if (m_horizontalLeftSpeed < 0.0f)
		{
			m_horizontalLeftSpeed = 0.0f;
		}
	}

	m_posZ += sinf(m_rotY) * 3.141 * m_horizontalLeftSpeed;
	m_posX -= cosf(m_rotY) * 3.141 * m_horizontalLeftSpeed;
}

void Input::HorizontalRightMove(float time)
{
	if (m_keys['D'] == 1)
	{

		m_horizontalRightSpeed += time * 0.01f;

		if (m_horizontalRightSpeed > (time * 0.3f))
		{
			m_horizontalRightSpeed = time * 0.3f;
		}
	}

	else
	{
		m_horizontalRightSpeed -= time * 0.006f;

		if (m_horizontalRightSpeed < 0.0f)
		{
			m_horizontalRightSpeed = 0.0f;
		}
	}

	m_posZ -= sinf(m_rotY) * 3.141 * m_horizontalRightSpeed;
	m_posX += cosf(m_rotY) * 3.141 * m_horizontalRightSpeed;
}

void Input::UpMove(float time)
{
	if (m_keys['Q'] == 1)
	{

		m_UpSpeed += time * 0.01f;

		if (m_UpSpeed > (time * 0.3f))
		{
			m_UpSpeed = time * 0.3f;
		}
	}

	else
	{
		m_UpSpeed -= time * 0.006f;

		if (m_UpSpeed < 0.0f)
		{
			m_UpSpeed = 0.0f;
		}
	}

	m_posY += 3.141 * m_UpSpeed;
}

void Input::DownMove(float time)
{
	if (m_keys['Y'] == 1)
	{

		m_DownSpeed += time * 0.01f;

		if (m_DownSpeed > (time * 0.3f))
		{
			m_DownSpeed = time * 0.3f;
		}
	}

	else
	{
		m_DownSpeed -= time * 0.006f;

		if (m_DownSpeed < 0.0f)
		{
			m_DownSpeed = 0.0f;
		}
	}

	m_posY -= 3.141 * m_DownSpeed;
}

void Input::LeftRotateMove(float time)
{
	if (m_keys['O'] == 1)
	{

		m_rotLeftSpeed += time * 0.01f;

		if (m_rotLeftSpeed > (time * 0.3f))
		{
			m_rotLeftSpeed = time * 0.3f;
		}
	}

	else
	{
		m_rotLeftSpeed -= time * 0.006f;

		if (m_rotLeftSpeed < 0.0f)
		{
			m_rotLeftSpeed = 0.0f;
		}
	}

	m_rotY -= 3.141 * m_rotLeftSpeed;
}

void Input::RightRotateMove(float time)
{
	if (m_keys['P'] == 1)
	{

		m_rotRightSpeed += time * 0.01f;

		if (m_rotRightSpeed > (time * 0.3f))
		{
			m_rotRightSpeed = time * 0.3f;
		}
	}

	else
	{
		m_rotRightSpeed -= time * 0.006f;

		if (m_rotRightSpeed < 0.0f)
		{
			m_rotRightSpeed = 0.0f;
		}
	}

	m_rotY += 3.141 * m_rotRightSpeed;
}

void Input::UpRotateMove(float time)
{
	if (m_keys['I'] == 1)
	{

		m_rotUpSpeed += time * 0.01f;

		if (m_rotUpSpeed > (time * 0.3f))
		{
			m_rotUpSpeed = time * 0.3f;
		}
	}

	else
	{
		m_rotUpSpeed -= time * 0.006f;

		if (m_rotUpSpeed < 0.0f)
		{
			m_rotUpSpeed = 0.0f;
		}
	}

	m_rotX -= 3.141 * m_rotUpSpeed;
}

void Input::DownRotateMove(float time)
{
	if (m_keys['K'] == 1)
	{

		m_rotDownSpeed += time * 0.01f;

		if (m_rotDownSpeed > (time * 0.3f))
		{
			m_rotDownSpeed = time * 0.3f;
		}
	}

	else
	{
		m_rotDownSpeed -= time * 0.006f;

		if (m_rotDownSpeed < 0.0f)
		{
			m_rotDownSpeed = 0.0f;
		}
	}

	m_rotX += 3.141 * m_rotDownSpeed;
}



float Input::getRotX() { return m_rotX; }
float Input::getRotY() { return m_rotY; }

float Input::getPosX() { return m_posX; }
float Input::getPosY() { return m_posY; }
float Input::getPosZ() { return m_posZ; }


Input::~Input()
{
	delete this;
}