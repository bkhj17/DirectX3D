#include "framework.h"
#include "Scene230303.h"
#include "Cube230303.h"
#include "FPS230303.h"

Scene230303::Scene230303()
{
	int row = 10;
	int col = 10;
	const int MAX_CUBE = 300;
	cubes.resize(MAX_CUBE);

	int index = 0;
	for (auto& cube : cubes) {
		cube = new Cube230303;
		cube->SetActive(false);
		cube->SetTag(to_string(index++));
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			cubes[(size_t)i * col + j]->SetActive(true);
			cubes[(size_t)i * col + j]->Pos() = { (float)i , -1.0f, (float)j};
		}
	}

	fps = new FPS230303;
	CAM->SetTarget(fps, Vector3(0, 1, 0));
}

Scene230303::~Scene230303()
{
	for (auto cube : cubes)
		delete cube;

	delete fps;
}

void Scene230303::Update()
{
	//ǥ���� Ž���Ǿ����� Ȯ��
	Ray ray = CAM->ScreenPointToRay({ CENTER_X, CENTER_Y, 0.0f });
	check = nullptr;

	Contact contect;
	contect.distance = FLT_MAX;
	for (auto cube : cubes) {
		if (!cube->Active())
			continue;

		Contact c;
		if (cube->GetCollider()->IsRayCollision(ray, &c)) {
			if (!check || contect.distance > c.distance) {
				check = cube;	//Ž���� ǥ��
				contect = c;
			}
		}
	}

	if (KEY_DOWN('Z')) {
		if (check) {	//Ž���� �� �ִ�
			Ray cRay;	//���� ���� �ִ��� �˻�
			cRay.pos = check->GlobalPos();
			cRay.dir = check->Up();

			bool isOn = false;
			for (auto cube : cubes) {
				if (!cube->Active() || cube == check)
					continue;

				//���� ���� �ִ��� ������ üũ
				Contact cContact;
				if (cube->GetCollider()->IsRayCollision(cRay, &cContact)) {
					if (cContact.distance < (check->GlobalScale().y * 0.5f + 1.0f)) {
						isOn = true;
						break;
					}
				}
			}

			if (!isOn) { //���� �ƹ��͵� ���ٸ� cube ����
				for (auto cube : cubes) {
					if (cube->Active())
						continue;

					cube->SetActive(true);
					cube->Pos() = check->GlobalPos() + check->Up();
					cube->Rot() = check->Rot();
					break;
				}
			}
		}
	}

	if (KEY_DOWN('X')) {
		if (check)
			check->SetActive(false);
	}

	for (auto cube : cubes) {
		if (cube == check)
			cube->GetCollider()->SetColor(1, 0, 0);
		else
			cube->GetCollider()->SetColor(0, 1, 0);

		cube->UpdateWorld();
	}

	//�ٴ� �ݸ��� : Ʀ
	for (auto cube : cubes) {
		fps->StandOn(cube->GetCollider());
	}
	fps->Update();
}

void Scene230303::PreRender()
{
}

void Scene230303::Render()
{
	for (auto cube : cubes) 
		cube->Render();
	if (check)
		check->GetCollider()->Render();
}

void Scene230303::PostRender()
{
	


}

void Scene230303::GUIRender()
{
	if (ImGui::TreeNode("Cubes")) {
		for (auto cube : cubes)
			cube->RenderUI();

		ImGui::TreePop();
	}
}
