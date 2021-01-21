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

SwapChain::SwapChain()
{
}

/*
	SwapChain: Collection of frame buffers to show render frames on the screen. Double Buffering technique is used here. Back_buffer is copied to front Buffer/output Window. Present Fraction is used to flip it.

	HRESULT CreateSwapChain(
	  IUnknown             *pDevice,
	  DXGI_SWAP_CHAIN_DESC *pDesc,
	  IDXGISwapChain       **ppSwapChain
	);

	CreateRenderTargetView: Creates a render-target view for accessing resource data

	HRESULT CreateRenderTargetView(
	  ID3D11Resource                      *pResource,
	  const D3D11_RENDER_TARGET_VIEW_DESC *pDesc,
	  ID3D11RenderTargetView              **ppRTView
	);
*/

bool SwapChain::init(HWND hwnd, UINT width, UINT height)
{
	ID3D11Device*device= GraphicsEngine::get()->m_d3d_device;

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
	HRESULT hr=GraphicsEngine::get()->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);
	
	if (FAILED(hr))
	{
		return false;
	}

	// get the back buffer color and create its render target view
	ID3D11Texture2D* buffer=NULL;
	hr=m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);		// we get a texture
	
	if (FAILED(hr))
	{
		return false;
	}

	hr=device->CreateRenderTargetView(buffer, NULL, &m_rtv);		// output it in m_rtv
	buffer->Release();

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

/*
	sync interval is first parameter. If 0 -> immediately without synchronization
*/

bool SwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync, NULL);

	return true;
}

bool SwapChain::release()
{
	m_swap_chain->Release();
	delete this;			// SwapChain * createSwapChain(){ return new SwapChain();}
	return true;
}

SwapChain::~SwapChain()
{
}
