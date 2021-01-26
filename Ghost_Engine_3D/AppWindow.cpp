/*
	MIT License

	C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)
	Ghost Engine 3D (https://github.com/GhostlyActive/Ghost-Engine-3D)

	Copyright (c) 2019-2020, PardCode
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

#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;	// new color attribute 
};


/*
	- DirectX handles constant data in VRAM in 16 Bytes, when the structure is above -> modified to be multiple of 16
		__declspec(align(16)) make it works.
*/

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;	// Constant Buffer timer
};


AppWindow::AppWindow()
{
}

void AppWindow::updateQuadPosition()
{
	// getTickCount is a Windows function. output the time since the start of the system in milliseconds
	constant cc;
	cc.m_time = ::GetTickCount();

	m_delta_pos += m_delta_time / 10.0f;	// last value makes the movement slower. Like 1 unit in x seconds
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;


	// translation matrix
	Matrix4x4 temp;

	m_delta_scale += m_delta_time / 0.55f;	// last value makes the movement slower. Like 1 unit in x seconds

	/*
		commented block: combine scale with translation matrix
		1. cc.m_world.setScale()	->	scale in sinus
		2. temp.setTranslation()	->	set translation matrix
		3. cc.m_world *= temp		->	multiply the scale matrix with translation matrix

		- order of multiplication is important (scale * translation)
	*/

	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));
	
	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f,1.5f, 0), m_delta_pos));

	//cc.m_world *= temp;


	cc.m_world.setScale(Vector3D(1, 1, 1));		// front face of cube

	temp.setIdentity();					
	temp.setRotationZ(m_delta_scale);
	cc.m_world *= temp;							// multiplicate with world metrics

	temp.setIdentity();
	temp.setRotationY(m_delta_scale);
	cc.m_world *= temp;

	//temp.setIdentity();
	//temp.setRotationX(m_delta_scale);
	//cc.m_world *= temp;



	cc.m_view.setIdentity();

	// pass the size of the window for the projection
	cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left)/300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top)/300.0f,
		-4.0f,
		4.0f
	);

	// pass it to the Constant buffer update function
	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	// call onCreate in Window
	Window::onCreate();
	GraphicsEngine::get()->init();

	// init inpput
	m_input = GraphicsEngine::get()->createInput();
	m_input->init();

	m_swap_chain=GraphicsEngine::get()->createSwapChain();
	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	// list of vertices
	vertex vertex_list[] = 
	{
		//X - Y - Z
		//FRONT FACE
		{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,0,0),  Vector3D(0.2f,0,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,1,0), Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,0.5f,-0.5f),   Vector3D(1,1,0),  Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,0,0), Vector3D(0.2f,0,0) },

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f),    Vector3D(0,1,0), Vector3D(0,0.2f,0) },
		{ Vector3D(0.5f,0.5f,0.5f),    Vector3D(0,1,1), Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,0.5f,0.5f),   Vector3D(0,1,1),  Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,-0.5f,0.5f),     Vector3D(0,1,0), Vector3D(0,0.2f,0) }

	};

	m_vb=GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);


	unsigned int index_list[]=
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		1,6,5,
		5,2,1,
		//BOTTOM SIDE
		7,0,3,
		3,4,7,
		//RIGHT SIDE
		3,2,5,
		5,4,3,
		//LEFT SIDE
		7,6,1,
		1,0,7
	};

	
	m_ib=GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	// pointer to list and the size of the list for loading 
	m_ib->load(index_list, size_index_list);



	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	// Vertex Shader Compiling with VertexSader.hlsl
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	// create VertexShader
	m_vs=GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

	// Vertex_Buffer Loading
	m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	// when create resources is finished -> release memory Buffer
	GraphicsEngine::get()->releaseCompiledShader();


	// Pixel Shader Compiling with PixelShader.hlsl
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	// create PixelShader
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();


	/*
		- Create constant buffer from GraphicsEngine. -> Create instance of ConstantBuffer
		- load parameter time from structure constant and its size
	*/

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	// call onUpdate in Window
	Window::onUpdate();

	// clear the render target
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0.0f, 0.0f,0.0f, 1);

	// set viewport of render target in which we have to draw 
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	// include timer for transform and animation
	updateQuadPosition();

	// bind constant buffer to the graphics pipeline for each shader (overloading)
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	// set default shader in the graphics pipeline to be able to draw
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	// set the vertices of the triangle to draw
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	//set the indices of the triangle to draw
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);


	// finally draw the triangle
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(),0, 0);
	m_swap_chain->present(true);



	// for timing. m_new_delta is current process time. 
	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	// aligned condition. For beginning when all delta would be zero
	m_delta_time = (m_old_delta)?((m_new_delta - m_old_delta) / 1000.0f):0;
}



void AppWindow::onSize()
{
		RECT rc = this->getClientWindowRect();
		m_swap_chain->Swap_Resize(rc.right - rc.left, rc.bottom - rc.top);
}

void AppWindow::onKeyDown(unsigned int value)
{
	m_input->KeyDown(value);		
}

void AppWindow::onKeyUp(unsigned int value)
{
	m_input->KeyUp(value);
}

void AppWindow::onDestroy()
{
	// call onDestroy in Window
	Window::onDestroy();

	m_vb->release();
	m_ib->release();
	m_cb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	m_input->release();
	GraphicsEngine::get()->release();
}
