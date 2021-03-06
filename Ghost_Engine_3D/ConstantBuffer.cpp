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

#include "ConstantBuffer.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"

#include <exception>

/*
	- ConstantBuffer() -> two parameter which one is pointer to a buffer and its size in memory
	- almost the same in VertexBuffer (delete Input Layout Object, size = size_buffer, change BindFlags)
	- make ConstantBuffer a friend class of GraphicsEngine to use private member m_d3d_device for creating a Buffer

	ID3D11Device::CreateBuffer

	HRESULT CreateBuffer(
	  const D3D11_BUFFER_DESC      *pDesc,
	  const D3D11_SUBRESOURCE_DATA *pInitialData,
	  ID3D11Buffer                 **ppBuffer
	);
*/

ConstantBuffer::ConstantBuffer(void* buffer, UINT size_buffer)
{
	if (m_buffer)m_buffer->Release();

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_buffer;
	buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = buffer;

	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
	{
		throw std::exception("Create Constant Buffer was not successful");
	}
}


/*
	- update -> DeviceContext and pointer to a buffer
	- to update the resources after init -> we have to use DeviceContext
	- make ConstantBuffer a friend class of DeviceContext

	ID3D11DeviceContext::UpdateSubresource allow us to upload data in constant buffer

	void UpdateSubresource(
	  ID3D11Resource  *pDstResource,
	  UINT            DstSubresource,
	  const D3D11_BOX *pDstBox,
	  const void      *pSrcData,
	  UINT            SrcRowPitch,
	  UINT            SrcDepthPitch
	);
*/

void ConstantBuffer::update(DeviceContext* context, void* buffer)
{
	context->m_device_context->UpdateSubresource(this->m_buffer, NULL, NULL, buffer, NULL, NULL);
}


ConstantBuffer::~ConstantBuffer()
{
	// release buffer
	if (m_buffer)m_buffer->Release();
}
