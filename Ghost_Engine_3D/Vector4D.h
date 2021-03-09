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
#include "Vector3D.h"

class Vector4D
{
public:

	// constructor -> init
	Vector4D() :m_x(0), m_y(0), m_z(0), m_s(0)
	{
	}

	// constructor -> init float values
	Vector4D(float x, float y, float z, float s) :m_x(x), m_y(y), m_z(z), m_s(s)
	{
	}


	// pass Vector4D Object by reference
	Vector4D(const Vector4D& vector) :m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z), m_s(vector.m_s)
	{
	}

	Vector4D(const Vector3D& vector) :m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z), m_s(1.0f)
	{
	}


	// https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvector4cross
	// 4d Cross Product

	void crossPro(Vector4D& v1, Vector4D& v2, Vector4D& v3)
	{
		this->m_x = v1.m_y * (v2.m_z * v3.m_s - v3.m_z * v2.m_s) - v1.m_z * (v2.m_y * v3.m_s - v3.m_y * v2.m_s) + v1.m_s * (v2.m_y * v3.m_z - v2.m_z * v3.m_y);
		this->m_y = -(v1.m_x * (v2.m_z * v3.m_s - v3.m_z * v2.m_s) - v1.m_z * (v2.m_x * v3.m_s - v3.m_x * v2.m_s) + v1.m_s * (v2.m_x * v3.m_z - v3.m_x * v2.m_z));
		this->m_z = v1.m_x * (v2.m_y * v3.m_s - v3.m_y * v2.m_s) - v1.m_y * (v2.m_x * v3.m_s - v3.m_x * v2.m_s) + v1.m_s * (v2.m_x * v3.m_y - v3.m_x * v2.m_y);
		this->m_s = -(v1.m_x * (v2.m_y * v3.m_z - v3.m_y * v2.m_z) - v1.m_y * (v2.m_x * v3.m_z - v3.m_x * v2.m_z) + v1.m_z * (v2.m_x * v3.m_y - v3.m_x * v2.m_y));
	}


	~Vector4D()
	{
	}

public:
	float m_x, m_y, m_z, m_s;		// coordinates
};