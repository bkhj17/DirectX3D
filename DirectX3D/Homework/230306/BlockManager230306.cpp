#include "framework.h"
#include "BlockManager230306.h"

BlockManager230306::~BlockManager230306()
{
	for (auto block : blocks)
		delete block;
	blocks.clear();
}

void BlockManager230306::Update()
{
	for (auto block : blocks)
		block->Update();
}

void BlockManager230306::Render()
{
	for (auto block : blocks)
		block->Render();
}

void BlockManager230306::CreateBlocks(UINT width, UINT height)
{
	vector<wstring> textures = {
		L"Textures/Block/Dirt.png",
		L"Textures/Block/Sand.png",
		L"Textures/Block/Stone.png",
		L"Textures/Block/Ice.png",
		L"Textures/Block/Gold.png",
	};

	for (UINT z = 0; z < height; z++) {
		for (UINT x = 0; x < width; x++) {
			Vector3 pos = { (float)x, 0, (float)z };
			wstring file = textures[Random(0, textures.size())];

			Cube* cube = new Cube;
			cube->GetMaterial()->SetDiffuseMap(file);
			cube->Pos() = pos;
			cube->UpdateWorld();

			blocks.push_back(cube);
		}
	}
}

float BlockManager230306::GetHeight(const Vector3& pos) const
{
	Ray ray(pos, Vector3::Down());
	Contact contact;

	float height = 0.0f;
	for (Cube* block : blocks) {
		if (block->GetCollider()->IsRayCollision(ray, &contact))
			height = max(height, contact.hitPoint.y);
	}

	return height;
}

Cube* BlockManager230306::BlockMining(const Ray& ray, OUT Contact* hit)
{
	Cube* check = nullptr;
	Contact contactCheck;
	contactCheck.distance = FLT_MAX;
	for (auto cube : blocks) {
		if (!cube->Active())
			continue;

		Contact c;
		if (cube->GetCollider()->IsRayCollision(ray, &c)) {
			if (!check || contactCheck.distance > c.distance) {
				check = cube;	//Ž���� ǥ��
				contactCheck = c;
			}
		}
	}

	if (hit != nullptr && check)
		*hit = contactCheck;

	return check;
}

bool BlockManager230306::SetupCube(Cube* check, Texture* texture, Contact& contact)
{
	if (!check || !texture) //Ž���� �� ����
		return false;
			
	Vector3 dir = contact.hitPoint - check->GlobalPos();
	//�� �� ���� ū �͸��� �����
	if (abs(dir.y) > abs(dir.x)) {
		if (abs(dir.y) > abs(dir.z))
			dir = { 0, (dir.y > 0) ? 1.0f : -1.0f, 0 };
		else
			dir = { 0, 0, (dir.z > 0) ? 1.0f : -1.0f };
	}
	else {
		if (abs(dir.x) > abs(dir.z))
			dir = { (dir.x > 0) ? 1.0f : -1.0f, 0, 0 };
		else
			dir = { 0, 0, (dir.z > 0) ? 1.0f : -1.0f };
	}

	Ray cRay;	//�� ���⿡ ���� �ִ��� �˻�
	cRay.pos = check->GlobalPos();
	cRay.dir = dir;

	bool isOn = false;
	for (auto cube : blocks) {
		if (!cube->Active() || cube == check)
			continue;

		//���� �ִ��� ������ üũ
		Contact cContact;
		if (cube->GetCollider()->IsRayCollision(cRay, &cContact)) {
			if (cContact.distance < (check->GlobalScale().y * 0.5f + 1.0f)) {
				isOn = true;
				break;
			}
		}
	}

	if (!isOn) { //�ƹ��͵� ���ٸ� cube ����
		for (auto cube : blocks) {
			if (cube->Active())
				continue;

			cube->SetActive(true);
			cube->GetMaterial()->SetDiffuseMap(texture->GetFile());
			cube->Pos() = check->GlobalPos() + dir;
			cube->Rot() = check->Rot();
			return true;
		}
	}
	return false;
}
