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
#include "Vector2D.h"
#include "Matrix4x4.h"

struct vertex
{
	Vector3D position;
	Vector2D texcoord;
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

void AppWindow::updateTransform()
{
	// getTickCount is a Windows function. output the time since the start of the system in milliseconds
	constant cc;
	cc.m_time = ::GetTickCount();

	m_delta_pos += m_delta_time / 10.0f;	// last value makes the movement slower. Like 1 unit in x seconds
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;


	// translation matrix
	Matrix4x4 temp;

	m_delta_scale += m_delta_time / 0.6f;	// last value makes the movement slower. Like 1 unit in x seconds



	cc.m_world.setIdentity();

	// camera matrix
	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_input->getRotX());

	// multiply with camera matrix
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_input->getRotY());
	world_cam *= temp;

	// translate camera backward of two points long the axis. With offset of 2. (left-right, up-down, forward-backward)			
	world_cam.setTranslation(Vector3D(m_input->getPosX(), m_input->getPosY(), m_input->getPosZ()));



	// camera matrix should be a view matrix by inverting
	world_cam.inverse();

	// fill constant buffer with world matrix of cube which is identity matrix
	cc.m_view = world_cam;

	float height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);
	float width = (this->getClientWindowRect().right - this->getClientWindowRect().left);

	cc.m_proj.setPerspectiveFovLH(1.6f, (width / height), 0.1f, 100.0f);



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

	// init input
	m_input = GraphicsEngine::get()->createInput();

	// create Texture from file 
	m_ts = GraphicsEngine::get()->createTextureShader(L"Graphics\\Textures\\ghosty.jpg");


	m_swap_chain=GraphicsEngine::get()->createSwapChain();
	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);



	/*
		next part is an example of a cube where we define vertices(position, Texture coordinates) in drawIndexedTriangleList mode
	
	*/


	// list of vertices (positions)
	Vector3D position_list[] =
	{
		{ Vector3D(-0.5f,-0.5f,-0.5f)},
		{ Vector3D(-0.5f,0.5f,-0.5f) },
		{ Vector3D(0.5f,0.5f,-0.5f) },
		{ Vector3D(0.5f,-0.5f,-0.5f)},

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f) },
		{ Vector3D(0.5f,0.5f,0.5f) },
		{ Vector3D(-0.5f,0.5f,0.5f)},
		{ Vector3D(-0.5f,-0.5f,0.5f) }
	};

	// texture coordinates (u,v) (0,0) -> top left corner and (1,1) -> down right corner
	Vector2D texcoord_list[] =
	{
		{ Vector2D(0.0f,0.0f) },
		{ Vector2D(0.0f,1.0f) },
		{ Vector2D(1.0f,0.0f) },
		{ Vector2D(1.0f,1.0f) }
	};

	// thats the final list
	vertex vertex_list[] =
	{
		//X - Y - Z
		//FRONT FACE
		{ position_list[0],texcoord_list[1] },
		{ position_list[1],texcoord_list[0] },
		{ position_list[2],texcoord_list[2] },
		{ position_list[3],texcoord_list[3] },


		{ position_list[4],texcoord_list[1] },
		{ position_list[5],texcoord_list[0] },
		{ position_list[6],texcoord_list[2] },
		{ position_list[7],texcoord_list[3] },


		{ position_list[1],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[2],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[0],texcoord_list[0] },
		{ position_list[3],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[3],texcoord_list[1] },
		{ position_list[2],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[1],texcoord_list[2] },
		{ position_list[0],texcoord_list[3] }

	};


	m_vb=GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);


	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		8,9,10,
		10,11,8,
		//BOTTOM SIDE
		12,13,14,
		14,15,12,
		//RIGHT SIDE
		16,17,18,
		18,19,16,
		//LEFT SIDE
		20,21,22,
		22,23,20
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
		0.0f, 0.0f, 0.0f, 1);

	// set viewport of render target in which we have to draw 
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	// include timer for transform and animation
	updateTransform();

	// Input
	m_input->Update(m_delta_time);


	// bind constant buffer to the graphics pipeline for each shader (overloading)
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	// set shader in the graphics pipeline to be able to draw
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setTextureShader(m_ts);

	// set the vertices of the triangle to draw
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	//set the indices of the triangle to draw
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);



	// finally draw triangles
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
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

void AppWindow::onMouseDown(int posX, int posY)
{
	m_input->MouseDown(posX, posY);

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
	m_ts->release();
	GraphicsEngine::get()->release();
}
