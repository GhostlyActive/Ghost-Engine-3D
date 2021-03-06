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
/*
	Takes output of the vertex shader. It draws each pixel on the polygon that will be
	rendered on screen.
*/

Texture2D shaderTexture;
SamplerState SampleType;

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD;
	float3 normal: NORMAL;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	unsigned int m_time;
	float3 ambientColor;
	float ambientPower;
	float3 m_vectorLight;

};

float4 psmain(PS_INPUT input) : SV_TARGET
{


	float3 sampleColor = shaderTexture.Sample(SampleType, input.texcoord * 0.5);

	float3 ambientLight = ambientColor * ambientPower;
	float3 ambient2Diffuse = ambientLight;


	float3 diffuseLightIntense = max(dot(m_vectorLight.xyz, input.normal), 0); 
	float3 diffuseLight = diffuseLightIntense * 2;

	ambient2Diffuse += diffuseLight;



	float3 finalColor = sampleColor * ambient2Diffuse;


return float4(finalColor, 1.0);


	//float4 textureColor;

	//textureColor = shaderTexture.Sample(SampleType, input.texcoord * 0.5);

	//return textureColor;

};