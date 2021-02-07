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

#include "VertexBuffer.h"
#include "GraphicsEngine.h"

#include <exception>

/*
	- CreateBuffer: Creates a buffer (vertex buffer, index buffer, or shader-constant buffer).

		HRESULT CreateBuffer(
		  const D3D11_BUFFER_DESC      *pDesc,
		  const D3D11_SUBRESOURCE_DATA *pInitialData,
		  ID3D11Buffer                 **ppBuffer
		);


	- Create the Vertex Buffer from DirectX Device: GraphicsEngine::get()->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer))

		id3D11Device *m_d3d_device -> is private. Make VertexBuffer a Friend Class in GraphicsEngine Class


	- CreateInputLayout: Create an input-layout object to describe the input-buffer data for the input-assembler stage.
		Describe and define attributes of vertex type. Information about the attributes that we compose our vertex type
*/

VertexBuffer::VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, size_t size_byte_shader) : m_layout(0), m_buffer(0)
{
	// release these to create new ones
	if (m_buffer)m_buffer->Release();
	if (m_layout)m_layout->Release();

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_vertex * size_list;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};

	// pointer to the memory of the vertices location 
	init_data.pSysMem = list_vertices;

	m_size_vertex = size_vertex;
	m_size_list = size_list;

	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
	{
		throw std::exception("Create Vertex Buffer was not successful");
	}

	/*
		this is the layout of the vertex data that will be processed by the shader.
		AlignedByteOffset indicates how the data is spaced in the buffer. First position needs 12 Bytes of space. Next one added 12 or 16 Bytes. D3D11_APPEND_ALIGNED_ELEMENT makes it automatically
	*/

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0,  DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA , 0}
	};

	UINT size_layout = ARRAYSIZE(layout);

	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_layout)))
	{
		throw std::exception("Create Input Layout was not successful");
	}
}


UINT VertexBuffer::getSizeVertexList()
{
	return this->m_size_list;
}


VertexBuffer::~VertexBuffer()
{
	m_layout->Release();
	m_buffer->Release();
}
