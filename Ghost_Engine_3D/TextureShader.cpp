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


#include "TextureShader.h"
#include "GraphicsEngine.h"
#include <DirectXTex.h>

#include <exception>

/*
	with the help of https://github.com/microsoft/DirectXTex/wiki/CreateTexture
	- create direct3D resource from a set of images
*/

TextureShader::TextureShader(const wchar_t* file)
{
	// contains image data
	DirectX::ScratchImage picture;

	// load files in memory
	HRESULT res = DirectX::LoadFromWICFile(file, DirectX::WIC_FLAGS_NONE, nullptr, picture);


	if (SUCCEEDED(res))
	{
		res = DirectX::CreateTexture(GraphicsEngine::get()->m_d3d_device, picture.GetImages(), picture.GetImageCount(), picture.GetMetadata(), &m_picture);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = picture.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = (UINT)picture.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		GraphicsEngine::get()->m_d3d_device->CreateShaderResourceView(m_picture, &desc, &m_ts);
	}
	else
	{
		throw std::exception("Loading Texture Resources was not successful");
	}
}


ID3D11ShaderResourceView* TextureShader::GetTexture()
{
	return m_ts;
}


TextureShader::~TextureShader()
{
	m_ts->Release();
	m_picture->Release();
}