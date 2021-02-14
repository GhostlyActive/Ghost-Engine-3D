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


void Input::RMouseDown(int posX, int posY)
{
	m_RMouseClicked = true;
	m_lastMouseX = posX;
	m_lastMouseY = posY;
}

void Input::RMouseUp(int posX, int posY)
{
	m_RMouseClicked = false;

	m_moveMouseLeft = false;
	m_moveMouseRight = false;
	m_moveMouseDown = false;
	m_moveMouseUp = false;

}

void Input::MouseMove(int posX, int posY)
{
	if (m_RMouseClicked)
	{

		if ((posX - m_lastMouseX) > 0)
		{
			m_moveMouseRight = true;
			m_moveMouseLeft = false;

		}

		else if ((posX - m_lastMouseX) < 0)
		{
			m_moveMouseRight = false;
			m_moveMouseLeft = true;
		}

		else
		{
			m_moveMouseLeft = false;
			m_moveMouseRight = false;
		}
		



		if ((posY - m_lastMouseY) > 0)
		{
			m_moveMouseUp = true;
			m_moveMouseDown = false;

		}

		else if ((posY - m_lastMouseY) < 0)
		{
			m_moveMouseUp = false;
			m_moveMouseDown = true;
		}

		else
		{
			m_moveMouseDown = false;
			m_moveMouseUp = false;
		}
	}
	m_lastMouseX = posX;
	m_lastMouseY = posY;
}

void Input::MouseLeave()
{

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


void Input::setTransform(float Transform[3])
{
	m_posX = Transform[0];
	m_posY = Transform[1];
	m_posZ = Transform[2];
}


void Input::ForwardMove(float time) 
{
	if (m_keys['W'] == 1)
	{

		m_forwardSpeed += time * acc;

		if (m_forwardSpeed > (time * max_speed))
		{
			m_forwardSpeed = time * max_speed;
		}
	}

	else
	{
		m_forwardSpeed -= time * dec;

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

		m_backwardSpeed += time * acc;

		if (m_backwardSpeed > (time * max_speed))
		{
			m_backwardSpeed = time * max_speed;
		}
	}

	else
	{
		m_backwardSpeed -= time * dec;

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
	if (m_keys['A'] == 1 || m_moveMouseLeft == true)
	{

		m_horizontalLeftSpeed += time * acc;

		if (m_horizontalLeftSpeed > (time * max_speed))
		{
			m_horizontalLeftSpeed = time * max_speed;
		}
	}

	else
	{
		m_horizontalLeftSpeed -= time * dec;

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
	if (m_keys['D'] == 1 || m_moveMouseRight == true)
	{

		m_horizontalRightSpeed += time * acc;

		if (m_horizontalRightSpeed > (time * max_speed))
		{
			m_horizontalRightSpeed = time * max_speed;
		}
	}

	else
	{
		m_horizontalRightSpeed -= time * dec;

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
	if (m_keys['Q'] == 1 || m_moveMouseDown == 1)
	{

		m_UpSpeed += time * acc;

		if (m_UpSpeed > (time * max_speed))
		{
			m_UpSpeed = time * max_speed;
		}
	}

	else
	{
		m_UpSpeed -= time * dec;

		if (m_UpSpeed < 0.0f)
		{
			m_UpSpeed = 0.0f;
		}
	}

	m_posY += 3.141 * m_UpSpeed;
}

void Input::DownMove(float time)
{
	if (m_keys['Y'] == 1 || m_moveMouseUp == 1)
	{

		m_DownSpeed += time * acc;

		if (m_DownSpeed > (time * max_speed))
		{
			m_DownSpeed = time * max_speed;
		}
	}

	else
	{
		m_DownSpeed -= time * dec;

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

		m_rotLeftSpeed += time * acc;

		if (m_rotLeftSpeed > (time * max_speed))
		{
			m_rotLeftSpeed = time * max_speed;
		}
	}

	else
	{
		m_rotLeftSpeed -= time * dec;

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

		m_rotRightSpeed += time * acc;

		if (m_rotRightSpeed > (time * max_speed))
		{
			m_rotRightSpeed = time * max_speed;
		}
	}

	else
	{
		m_rotRightSpeed -= time * dec;

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

		m_rotUpSpeed += time * acc;

		if (m_rotUpSpeed > (time * max_speed))
		{
			m_rotUpSpeed = time * max_speed;
		}
	}

	else
	{
		m_rotUpSpeed -= time * dec;

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

		m_rotDownSpeed += time * acc;

		if (m_rotDownSpeed > (time * max_speed))
		{
			m_rotDownSpeed = time * max_speed;
		}
	}

	else
	{
		m_rotDownSpeed -= time * dec;

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

}