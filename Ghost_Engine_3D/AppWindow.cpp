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

#include "Libs/ImGui/imgui.h"
#include "Libs/ImGui/imgui_impl_win32.h"
#include "Libs/ImGui/imgui_impl_dx11.h"

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
	Vector3D ambientColor;
	float ambientPower;
	
};


AppWindow::AppWindow()
{
}

void AppWindow::updateTransform()
{
	// getTickCount is a Windows function. Output the time since the start of the system in milliseconds
	constant cc;
	cc.m_time = ::GetTickCount64();

	m_delta_pos += m_delta_time / 10.0f;	// last value makes the movement slower. Like 1 unit in x seconds
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;


	m_delta_scale += m_delta_time / 0.6f;	// last value makes the movement slower. Like 1 unit in x seconds


	// ambient light
	cc.ambientColor = Vector3D(1.0f, 1.0f, 1.0f);
	cc.ambientPower = 1.0f;


	// update camera
	
	// translation matrix
	Matrix4x4 temp;


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





	cc.m_world.setIdentity();



	// pass it to the Constant buffer update function
	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}


void AppWindow::UpdateGui()
{
	float CameraTranslation[3] = { m_input->getPosX(), m_input->getPosY(), m_input->getPosZ() };
	static int counter = 0;

	// start ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow();

	ImGui::Begin("fps");
	ImGui::Text(" (%.1f FPS)", ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::DragFloat3("Translation", CameraTranslation, 0.1f, -10.0f, 10.0f);
	m_input->setTransform(CameraTranslation);


	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	// call onCreate in Window
	Window::onCreate();

	// init the singleton GraphicsEngine
	GraphicsEngine::get();

	// init ImGui
	GraphicsEngine::get()->InitGui(this->m_hwnd);


	// init input
	m_input = GraphicsEngine::get()->createInput();

	// set MeshModel()
	GraphicsEngine::get()->setMeshModel();

	// create Texture from file 
	m_ts = GraphicsEngine::get()->createTextureShader(L"Graphics\\Textures\\marmor2.jpg");
	

	// create mesh from file
	m_mesh = GraphicsEngine::get()->createMeshModel(L"Graphics\\Objects\\temple.obj");



	// init SwapChain
	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);



	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	// vertex Shader Compiling with VertexSader.hlsl
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	// create VertexShader
	m_vs=GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	// when create resources is finished -> release memory Buffer
	GraphicsEngine::get()->releaseCompiledShader();


	// Pixel Shader Compiling with PixelShader.hlsl
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	// create PixelShader
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();


	constant cc;
	cc.m_time = 0.0f;

	m_cb = GraphicsEngine::get()->createConstantBuffer(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	// call onUpdate in Window
	Window::onUpdate();

	// clear the render target
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0.0f, 0.0f, 0.0f, 1);

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
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_mesh->getVertex());


	//set the indices of the triangle to draw
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_mesh->getIndex());




	// finally draw triangles
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_mesh->getIndex()->getSizeIndexList(), 0, 0);


	UpdateGui();

	m_swap_chain->present(true);


	// for timing. m_new_delta is current process time. 
	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount64();

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


void AppWindow::onRMouseDown(int posX, int posY)
{
	m_input->RMouseDown(posX, posY);
}


void AppWindow::onRMouseUp(int posX, int posY)
{
	m_input->RMouseUp(posX, posY);
}


void AppWindow::onMouseMove(int posX, int posY)
{
	m_input->MouseMove(posX, posY);
}


void AppWindow::onMouseLeave()
{
	m_input->MouseLeave();
}

// should be solved by smart pointers
void AppWindow::onDestroy()
{
	// call onDestroy in Window
	Window::onDestroy();

	delete m_mesh;
	delete m_cb;
	delete m_swap_chain;
	delete m_input;
	delete m_vs;
	delete m_ps;
	delete m_ts;
}
