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

#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Input.h"
#include "TextureShader.h"
#include "MeshModel.h"

#include <d3dcompiler.h>
#include <exception>

#include "Libs/ImGui/imgui.h"
#include "Libs/ImGui/imgui_impl_win32.h"
#include "Libs/ImGui/imgui_impl_dx11.h"

/*
	- D3D11CreateDevice: Creates a device that represents the display adapter. Get us access to all ressources to draw on screen. Last three parameters are output

		HRESULT D3D11CreateDevice(
		  IDXGIAdapter            *pAdapter,
		  D3D_DRIVER_TYPE         DriverType,
		  HMODULE                 Software,
		  UINT                    Flags,
		  const D3D_FEATURE_LEVEL *pFeatureLevels,
		  UINT                    FeatureLevels,
		  UINT                    SDKVersion,
		  ID3D11Device            **ppDevice,						* m_d3d_device	- - -> &m_d3d_device	- output
		  D3D_FEATURE_LEVEL       *pFeatureLevel,					m_feature_level - - -> &m_feature_level	- output
		  ID3D11DeviceContext     **ppImmediateContext				* m_imm_context - - -> &m_imm_context	- output
		);

	- D3D_DRIVER_TYPE: allow directX to execute drawing functions. Make a Vector with these driver types from best to worst:
		1) best -> execute mainly in GPU.(best performance)
		2) average -> warp drivers drawing calls all in high performance CPU.(SSE instructions)
		3) worst
*/

GraphicsEngine::GraphicsEngine()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;

	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
			num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);
		if (SUCCEEDED(res))
			break;
		++driver_type_index;
	}
	if (FAILED(res))
	{
		throw std::exception("Create Device was not successful");
	}

	//instance of DeviceContext where we pass (immediate Context)-> m_imm_context of class ID3D11DeviceContext*
	m_imm_device_context = new DeviceContext(m_imm_context);


	/*
		 - To create a SwapChain -> call the dxgi factory, from which we call the SwapChain
			1. m_d3d_device->QueryInterface -> return instance of IDXGIDevice class, which takes care  of low-level tasks
			2. m_dxgi_device->GetParent -> return instance of IDXGIAdapter class
			3. m_dxgi_adapter->GetParent -> return instance of IDXGIFactory class

			4. make SwapChain a friend class in GraphicsEngine. To have access to private member dxgi_factory in SwapChain
	*/

	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);
}

void GraphicsEngine::InitGui(HWND hwnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(m_d3d_device, m_imm_context);
	ImGui::StyleColorsClassic();

	/*
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;


	GraphicsEngine::get()->compileVertexShader(L"MeshModelShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	::memcpy(m_mesh_byte, shader_byte_code, size_shader);
	m_mesh_size = size_shader;
	GraphicsEngine::get()->releaseCompiledShader();

	*/
}


/*
	allocate new instance
*/

SwapChain* GraphicsEngine::createSwapChain(HWND hwnd, UINT width, UINT height)
{
	SwapChain* swap = nullptr;
	try
	{
		swap = new SwapChain(hwnd, width, height);
	}
	catch (...) {}

	return swap;
}


DeviceContext* GraphicsEngine::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}


VertexBuffer* GraphicsEngine::createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, size_t size_byte_shader)
{
	VertexBuffer* vertex = nullptr;
	try
	{
		vertex = new VertexBuffer(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader);
	}
	catch (...) {}

	return vertex;
}


IndexBuffer * GraphicsEngine::createIndexBuffer(void* list_indices, UINT size_list)
{
	IndexBuffer* index = nullptr;
	try
	{
		index = new IndexBuffer(list_indices, size_list);
	}
	catch (...) {}

	return index;
}


ConstantBuffer* GraphicsEngine::createConstantBuffer(void* buffer, UINT size_buffer)
{
	ConstantBuffer* constant = nullptr;
	try
	{
		constant = new ConstantBuffer(buffer, size_buffer);
	}
	catch (...) {}

	return constant;
}


Input* GraphicsEngine::createInput()
{
	return new Input();
}


/*
	- Create new instance of VertexShader class -> vertex
	- make Graphicsengine friend class in Vertex shader to call private method init
*/

VertexShader* GraphicsEngine::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShader* vertex = nullptr;
	try
	{
		vertex = new VertexShader(shader_byte_code, byte_code_size);
	}
	catch (...) {}

	return vertex;
}


PixelShader* GraphicsEngine::createPixelShader(const void * shader_byte_code, size_t byte_code_size)
{
	PixelShader* pixel = nullptr;
	try
	{
		pixel = new PixelShader(shader_byte_code, byte_code_size);
	}
	catch (...) {}

	return pixel;
}


TextureShader* GraphicsEngine::createTextureShader(const wchar_t* file)
{
	TextureShader* texture = nullptr;
	try
	{
		texture = new TextureShader(file);
	}
	catch (...) {}

	return texture;
}


MeshModel* GraphicsEngine::createMeshModel(const wchar_t* file)
{
	MeshModel* mesh = nullptr;
	try
	{
		mesh = new MeshModel(file);

	}
	catch (...) {}


	return mesh;
}

void GraphicsEngine::setMeshModel()
{
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;


	GraphicsEngine::get()->compileVertexShader(L"MeshModelShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	::memcpy(m_mesh_byte, shader_byte_code, size_shader);
	m_mesh_size = size_shader;
	GraphicsEngine::get()->releaseCompiledShader();
}

void GraphicsEngine::getMeshModelShader(void** byte_code, size_t* size)
{
	*byte_code = m_mesh_byte;
	*size = m_mesh_size;
}


/*
	1. #include <d3dcompiler.h>   -> allow us to compile our shader code
	2. add static library (properties->Linker->Input->d3dcompiler.lib)

	3. 3DCompileFromFile()
		first parameter:-> file name of source file.
		entry_point_name:->  name function of the shader.
		"vs_5_0":->shader version we want to compile
		&m_blob, &error_blob:-> first is a data structure in which we replace buffer with compiled shader and its size in memory. Second contains errors, when fails
	
	4. if everything is ok -> return shader_byte_code and size
*/

bool GraphicsEngine::compileVertexShader(const wchar_t* file_name,const char* entry_point_name,void** shader_byte_code,size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob)))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}


bool GraphicsEngine::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob)))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}


void GraphicsEngine::releaseCompiledShader()
{
	if (m_blob)m_blob->Release();
}


GraphicsEngine::~GraphicsEngine()
{
	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	// destroy device context
	delete m_imm_device_context;

	// destroy DirectX device
	m_d3d_device->Release();

	// destroy ImGui
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


/*
	 this will take it to the stack and will called once in initialization
	 each time this get function will always get the same instance -> no more instances of this class will be created at runtime
*/

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}