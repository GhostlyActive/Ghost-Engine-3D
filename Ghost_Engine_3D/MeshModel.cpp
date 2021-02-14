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


#include "MeshModel.h"
#include "GraphicsEngine.h"
#include "VertexMesh.h"
#include <locale>
#include <codecvt>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <exception>

/*
	with the help of https://github.com/tinyobjloader/tinyobjloader
	- parsing of polygons. embedding .obj files to Engine
*/

/*
	Explanation( Everything is based on polygons ):  with the help of https://www.tutorialfor.com/questions-104539.htm
		obj file contains vertices coordinaes (vx, vy, vz), Normal (nx, ny, nz) and Texture coordinates (tx, ty)
*/
MeshModel::MeshModel(const wchar_t* file)
{
	
	tinyobj::attrib_t attrib;

	// pass vector of shapes/formes 
	std::vector<tinyobj::shape_t> shape;

	// vector of materials
	std::vector<tinyobj::material_t> material;

	std::string warning;
	std::string error;

	// no unicode file path -> convert unicode into AscII path with wstring_converter 
	std::string input = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(file);


	bool res = tinyobj::LoadObj(&attrib, &shape, &material, &warning, &error, input.c_str());

	if ((error.empty() && res))
	{
		std::vector<VertexMesh> verticeList;
		std::vector<unsigned int> indiceList;



		for (size_t i = 0; i < shape.size(); i++)
		{
			size_t indexPosition = 0;
			verticeList.reserve(shape[i].mesh.indices.size());
			indiceList.reserve(shape[i].mesh.indices.size());


			for (size_t j = 0; j < shape[i].mesh.num_face_vertices.size(); j++)
			{
				// each face -> number of vertices 
				 int verticeNumber = shape[i].mesh.num_face_vertices[j];

				for (size_t k = 0; k < verticeNumber; k++)
				{
					tinyobj::index_t idx = shape[i].mesh.indices[indexPosition + k];

					// vertices coordinates
					tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

					// vertex normal
					/*
					
					*/

					// texture coordinates
					tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];

					VertexMesh V(Vector3D(vx, vy, vz), Vector2D(tx, ty));
					// add to the Vector list
					verticeList.push_back(V);

					indiceList.push_back((unsigned int)indexPosition + k);

				}

				indexPosition += verticeNumber;
			}
		}

		void* shader_byte_code = nullptr;
		size_t size_shader = 0;
		GraphicsEngine::get()->getMeshModelShader(&shader_byte_code, &size_shader);

		v_Buffer = GraphicsEngine::get()->createVertexBuffer(&verticeList[0], sizeof(VertexMesh), (UINT)verticeList.size(), shader_byte_code, (UINT)size_shader);


		i_Buffer = GraphicsEngine::get()->createIndexBuffer(&indiceList[0], (UINT)indiceList.size());
	}

	else
	{
		throw std::exception("Loading Mesh Resources was not successful");
	}
}


VertexBuffer* MeshModel::getVertex()
{
	return v_Buffer;

}

IndexBuffer* MeshModel::getIndex()
{
	return i_Buffer;

}


MeshModel::~MeshModel()
{
	delete v_Buffer;
	delete i_Buffer;
}