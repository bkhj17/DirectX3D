#include "framework.h"

MainSelectUI::MainSelectUI(Vector3 pos)
	: SelectUI({ 200.0f, 100.0f }, pos)
{
	text = { "�� ����", "���" };
	maxCursor = (int)text.size();
}

void MainSelectUI::ActiveFunc()
{
	switch (cursor)
	{
	case 0:
		TurnManager::Get()->NextTurn();
		Close();
		break;
	default:
		CancelFunc();
		break;
	}
}