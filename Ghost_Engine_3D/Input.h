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

#pragma once
#include "Window.h"
#include <math.h>

class Input
{
public:

	Input();
	~Input();

	void KeyDown(unsigned int value);
	void KeyUp(unsigned int value);
	void RMouseDown(int posX, int posY);
	void RMouseUp(int posX, int posY);
	void MouseMove(int posX, int posY);
	void MouseLeave();

	void Update(float time);

	// transform and rotation movement
	void ForwardMove(float time);
	void BackwardMove(float time);
	void HorizontalLeftMove(float time);
	void HorizontalRightMove(float time);
	void UpMove(float time);
	void DownMove(float time);
	void LeftRotateMove(float time);
	void RightRotateMove(float time);
	void UpRotateMove(float time);
	void DownRotateMove(float time);

	// getter of positions
	float getRotX();
	float getRotY();
	float getPosX();
	float getPosY();
	float getPosZ();

private:

	bool m_keys[256];

private:

	// mouse position and status
	bool m_RMouseClicked = false;
	int m_lastMouseX = 0;
	int m_lastMouseY = 0;
	
	bool m_moveMouseRight = 0;
	bool m_moveMouseLeft = 0;
	bool m_moveMouseUp = 0;
	bool m_moveMouseDown = 0;


	// position of the camera
	float m_posX = 1.0f;
	float m_posY = 0.0f;
	float m_posZ = -2.0f;

	float m_rotX = 0.0f;
	float m_rotY = 0.0f;
	float m_rotZ = 0.0f;

	// rotation speed
	float m_rotLeftSpeed = 0.0f;
	float m_rotRightSpeed = 0.0f;
	float m_rotUpSpeed = 0.0f;
	float m_rotDownSpeed = 0.0f;


	// transform speed
	float m_forwardSpeed = 0.0f;
	float m_backwardSpeed = 0.0f;
	float m_horizontalLeftSpeed = 0.0f;
	float m_horizontalRightSpeed = 0.0f;
	float m_UpSpeed = 0.0f;
	float m_DownSpeed = 0.0f;

};

