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

#pragma once
#include <d3d11.h>

class DeviceContext;
class SwapChain
{
public:
	SwapChain();
	// Initialize SwapChain for a window
	bool init(HWND hwnd,UINT width,UINT height);

	// Resize swap chain when window size is changed
	void Swap_Resize(UINT width, UINT height);
	bool LoadViews(UINT width, UINT height);

	bool present(bool vsync);

	// Release the swap chain
	bool release();
	~SwapChain();
private:
	IDXGISwapChain * m_swap_chain;		// output
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilView* m_dsv;

private:
	friend class DeviceContext;
};

