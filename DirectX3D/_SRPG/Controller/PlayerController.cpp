#include "framework.h"
#include "PlayerController.h"

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::Control()
{
	/*
	//mapCursor�� ��� �� �� ������ �Ǵµ�...
	//����
	if (SRPGUIManager::Get()->IsActing()) {
		if (!SRPGUIManager::Get()->IsMapControl()) {
			mapCursor->UpdateWorld();
			return;
		}
	}
	else {
		if (KEY_DOWN('Z')) {
			InputAction();
			if (CharacterManager::Get()->HoldedCharacter())
				SRPGUIManager::Get()->OpenUI("ActionSelect");
		}
	}

	mapCursor->Control();
	mapCursor->Update(); 
	*/
}
