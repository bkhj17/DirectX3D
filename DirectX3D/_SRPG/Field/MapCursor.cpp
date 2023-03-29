#include "framework.h"
#include "MapCursor.h"
#include "GridedTerrain.h"

MapCursor::MapCursor()
{
	object = new SphereCollider;

}

MapCursor::~MapCursor()
{
	delete object;
}

void MapCursor::Update()
{
	Control();
	Move();
}

void MapCursor::Render()
{
	object->Render();
}

void MapCursor::SetPosCoord(int w, int h, bool teleport)
{
	if (!terrain)
		return;

	Pos() = terrain->CoordToPos(w, h);
	Pos().y += 3.0f;
	UpdateWorld();
	objectPos = GlobalPos();

	//Ŀ�� �ö� ĭ�� ������Ʈ�� �ִٸ� Ŀ�� ������Ʈ ���� �ø���
	Transform* inPoint = terrain->ObjectOnIndex(terrain->CoordToIndex({ w, h }));
	if (inPoint)
		objectPos.y += 6.0f;

	if (teleport) {
		object->Pos() = objectPos;
		isMoving = true;
	}

	if (isMoved) {
		terrain->SetSelected(w, h);
		CamMove();
	}
}

void MapCursor::SetGridTerrain(GridedTerrain* terrain)
{
	this->terrain = terrain;
	SetMapGrid(terrain->Row(), terrain->Col());

	terrain->SetSelected(w, h);
}

void MapCursor::Control()
{
	if (!isMoving) {
		int nh = h;
		int nw = w;

		if (KEY_PRESS(VK_UP))		nh--;
		if (KEY_PRESS(VK_DOWN))		nh++;
		if (KEY_PRESS(VK_LEFT))		nw--;
		if (KEY_PRESS(VK_RIGHT))	nw++;

		if (terrain && terrain->IsActiveCoord(nw, nh)) {
			w = nw;
			h = nh;
		}
	}
}

void MapCursor::Move()
{
	if (!isMoving)
		SetPosCoord(w, h);

	isMoved = isMoving;
	isMoving = (objectPos - object->Pos()).Length() > 1.0f;
	
	object->Pos() = (isMoving) ? Lerp(object->Pos(), objectPos, 20.0f * DELTA) : objectPos;
	object->UpdateWorld();
}

void MapCursor::CamMove()
{
	//���� ��Ű�� ����. ���� ��Ȯ�� ���� �����غ��� ������....
	//���� ���� ��Ȳ
	Vector3 cursorScreen = CAM->WorldToScreen(GlobalPos());
	Vector3 camMove;
	if (cursorScreen.x <= WIN_WIDTH * 0.1f)
		camMove.x = -terrain->GetTileSize().x;
	if (cursorScreen.x >= WIN_WIDTH * 0.9f)
		camMove.x = terrain->GetTileSize().x;
	if (cursorScreen.y <= WIN_HEIGHT * 0.1f)
		camMove.z = -terrain->GetTileSize().z;
	if (cursorScreen.y >= WIN_HEIGHT * 0.9f)
		camMove.z = terrain->GetTileSize().z;

	CAM->Pos() += camMove;
}
