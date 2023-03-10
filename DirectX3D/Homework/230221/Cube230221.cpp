#include "framework.h"
#include "Cube230221.h"

Cube230221::Cube230221(wstring fileName, Vector3 size)
{
	tag = "Cube";
	material->SetShader(L"UV230221.hlsl");
	material->SetDiffuseMap(fileName);

	mesh = new Mesh<VertexUV>;
	vector<VertexUV>& vertices = mesh->GetVertices();
	vertices.reserve(24);
	//front : 0123
	vertices.emplace_back(-size.x, -size.y, -size.z, 0.0f, 1.0f);	//0
	vertices.emplace_back(-size.x, +size.y, -size.z, 0.0f, 0.0f);	//1
	vertices.emplace_back(+size.x, -size.y, -size.z, 0.25f, 1.0f);	//2
	vertices.emplace_back(+size.x, +size.y, -size.z, 0.25f, 0.0f);	//3
	//Up : 1537
	vertices.emplace_back(-size.x, +size.y, -size.z, 0.0f, 1.0f);	//1
	vertices.emplace_back(-size.x, +size.y, +size.z, 0.25f, 1.0f);	//5
	vertices.emplace_back(+size.x, +size.y, -size.z, 0.5f, 1.0f);	//3
	vertices.emplace_back(+size.x, +size.y, +size.z, 0.75f, 1.0f);	//7
	//Right : 2367
	vertices.emplace_back(+size.x, -size.y, -size.z, 0.25f, 1.0f);	//2
	vertices.emplace_back(+size.x, +size.y, -size.z, 0.25f, 0.0f);	//3
	vertices.emplace_back(+size.x, -size.y, +size.z, 0.5f, 1.0f);	//6
	vertices.emplace_back(+size.x, +size.y, +size.z, 0.5f, 0.0f);	//7
	//Back : 4657
	vertices.emplace_back(+size.x, -size.y, +size.z, 0.5f, 1.0f);	//6
	vertices.emplace_back(+size.x, +size.y, +size.z, 0.5f, 0.0f);	//7
	vertices.emplace_back(-size.x, -size.y, +size.z, 0.75f, 1.0f);	//4
	vertices.emplace_back(-size.x, +size.y, +size.z, 0.75f, 0.0f);	//5
	//Down : 4062
	vertices.emplace_back(-size.x, -size.y, +size.z, 0.0f, 0.0f);	//4
	vertices.emplace_back(-size.x, -size.y, -size.z, 0.25f, 0.0f);	//0
	vertices.emplace_back(+size.x, -size.y, +size.z, 0.5f, 0.0f);	//6
	vertices.emplace_back(+size.x, -size.y, -size.z, 0.75f, 0.0f);	//2
	//Left : 4501
	vertices.emplace_back(-size.x, -size.y, +size.z, 0.75f, 1.0f);	//4
	vertices.emplace_back(-size.x, +size.y, +size.z, 0.75f, 0.0f);	//5
	vertices.emplace_back(-size.x, -size.y, -size.z, 1.0f, 1.0f);	//0
	vertices.emplace_back(-size.x, +size.y, -size.z, 1.0f, 0.0f);	//1

	vector<UINT>& indices = mesh->GetIndices();
	indices.reserve(36);
	for (int i = 0; i < 6; i++) {
		indices.push_back(i * 4);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 3);
	}
	mesh->CreateMesh();
}

Cube230221::~Cube230221()
{
	delete mesh;
}

void Cube230221::Update()
{
	UpdateWorld();
}

void Cube230221::Render()
{
	SetRender();
	mesh->Draw();
}
