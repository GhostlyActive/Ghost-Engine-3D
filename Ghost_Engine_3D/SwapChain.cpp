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

#include "SwapChain.h"
#include "GraphicsEngine.h"

#include <exception>

/*
	SwapChain: Collection of frame buffers to show render frames on the screen. Double Buffering technique is used here. Back_buffer is copied to front Buffer/output Window. Present Fraction is used to flip it.

	HRESULT CreateSwapChain(
	  IUnknown             *pDevice,
	  DXGI_SWAP_CHAIN_DESC *pDesc,
	  IDXGISwapChain       **ppSwapChain
	);


	CreateRenderTargetView: Creates a render-target view for accessing resource data.

	HRESULT CreateRenderTargetView(
	  ID3D11Resource                      *pResource,
	  const D3D11_RENDER_TARGET_VIEW_DESC *pDesc,
	  ID3D11RenderTargetView              **ppRTView
	);


	Create a depth-stencil view for accessing resource data.

	HRESULT CreateDepthStencilView(
	  ID3D11Resource                      *pResource,
	  const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc,
	  ID3D11DepthStencilView              **ppDepthStencilView
	);
*/

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height)
{
	ID3D11Device* device = GraphicsEngine::get()->m_d3d_device;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;

	// create the swap chain for the window indicated by HWND parameter
	HRESULT hr = GraphicsEngine::get()->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);

	if (FAILED(hr))
	{
		throw std::exception("Create SwapChain was not successful");
	}

	// get the back buffer color and create its render target view
	ID3D11Texture2D* buffer = NULL;
	hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);		// we get a texture

	if (FAILED(hr))
	{
		throw std::exception("Create SwapChain Buffer was not successful");
	}

	hr = device->CreateRenderTargetView(buffer, NULL, &m_rtv);		// output it in m_rtv
	buffer->Release();

	if (FAILED(hr))
	{
		throw std::exception("Create RenderTargetView was not successful");
	}


	//	The depth buffer is just a 2D texture that stores the depth information (and stencil information if using stenciling). To create a texture,
	//	we need to fill out a D3D11_TEXTURE2D_DESC structure describing the texture to create, and then call the ID3D11Device::CreateTexture2D method.

	D3D11_TEXTURE2D_DESC depth_desc = {};

	depth_desc.Width = width;
	depth_desc.Height = height;
	depth_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_desc.MipLevels = 1;
	depth_desc.SampleDesc.Count = 1;
	depth_desc.SampleDesc.Quality = 0;
	depth_desc.MiscFlags = 0;
	depth_desc.ArraySize = 1;
	depth_desc.CPUAccessFlags = 0;

	hr = device->CreateTexture2D(&depth_desc, nullptr, &buffer);

	if (FAILED(hr))
	{
		throw std::exception("Create Texture2D was not successful");
	}

	hr = device->CreateDepthStencilView(buffer, NULL, &m_dsv);		// output it in m_dsv
	buffer->Release();

	if (FAILED(hr))
	{
		throw std::exception("Create DepthStencilView was not successful");
	}
}


/*
	ResizeBuffers to handle window resizing. Before you have to release of references of swap chain's Buffer.
*/

void SwapChain::Swap_Resize(UINT width, UINT height)
{
	// release the views
	m_rtv->Release();
	m_dsv->Release();

	m_swap_chain->ResizeBuffers(0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	LoadViews(width, height);
}


/*
	sync interval is first parameter. If 0 -> immediately without synchronization
*/

bool SwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync, NULL);

	return true;
}

void SwapChain::Swap_Fullscreen(bool fullscreenState, UINT width, UINT height) 
{
	Swap_Resize(width, height);
	m_swap_chain->SetFullscreenState(fullscreenState, nullptr);
}

void SwapChain::LoadViews(UINT width, UINT height)
{
	 ID3D11Device* device = GraphicsEngine::get()->m_d3d_device;

	// get the back buffer color and create its render target view
	ID3D11Texture2D* buffer = NULL;
	HRESULT hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);		// we get a texture

	if (FAILED(hr))
	{
		throw std::exception("Load SwapChain was not successful");
	}

	hr = device->CreateRenderTargetView(buffer, NULL, &m_rtv);		// output it in m_rtv
	buffer->Release();

	if (FAILED(hr))
	{
		throw std::exception("Load Render TargetView was not successful");
	}

	/*
		The depth buffer is just a 2D texture that stores the depth information (and stencil information if using stenciling). To create a texture,
		we need to fill out a D3D11_TEXTURE2D_DESC structure describing the texture to create, and then call the ID3D11Device::CreateTexture2D method.
	*/

	D3D11_TEXTURE2D_DESC depth_desc = {};

	depth_desc.Width = width;
	depth_desc.Height = height;
	depth_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_desc.MipLevels = 1;
	depth_desc.SampleDesc.Count = 1;
	depth_desc.SampleDesc.Quality = 0;
	depth_desc.MiscFlags = 0;
	depth_desc.ArraySize = 1;
	depth_desc.CPUAccessFlags = 0;


	hr = device->CreateTexture2D(&depth_desc, nullptr, &buffer);

	if (FAILED(hr))
	{
		throw std::exception("Load Texture2D was not successful");
	}

	hr = device->CreateDepthStencilView(buffer, NULL, &m_dsv);		// output it in m_dsv
	buffer->Release();

	if (FAILED(hr))
	{
		throw std::exception("Load DepthStencilView was not successful");
	}
}


SwapChain::~SwapChain()
{
	m_swap_chain->Release();
	m_rtv->Release();
	m_dsv->Release();
}
