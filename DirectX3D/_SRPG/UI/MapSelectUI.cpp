#include "framework.h"

MapSelectUI::MapSelectUI(string eventKey, Vector2 size, Vector3 pos)
	: UIWindow(size, pos), eventKey(eventKey)
{
	mapControl = true;
}

MapSelectUI::MapSelectUI(string eventKey, float left, float right, float up, float down)
	: UIWindow(left, right, up, down), eventKey(eventKey)
{
	mapControl = true;
}

void MapSelectUI::Render()
{
	//â ��� �� �Ϸ��� ���� ��
	//Window�� ������ ���� �ְ����� �ϴ� ����
}

void MapSelectUI::ActiveFunc()
{
	Observer::Get()->ExcuteEvent(eventKey);
}

void MapSelectUI::CancelFunc()
{
	auto holded = CharacterManager::Get()->HoldedCharacter();
	if (holded != nullptr)
		Observer::Get()->ExcuteParamEvent("FocusPos", &holded->Pos());

	__super::CancelFunc();
}
