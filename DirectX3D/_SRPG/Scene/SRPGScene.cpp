#include "framework.h"
#include "SRPGScene.h"
#include "../Field/GridedTerrain.h"
#include "../Field/MapCursor.h"
#include "../Character/Character.h"
#include "../Character/CharacterManager.h"
#include "../UI/SRPGUIManager.h"
#include "../UI/UIWindow.h"

SRPGScene::SRPGScene()
{
	terrain = new GridedTerrain;
	
	//�������� �ߴµ� ���߿� ���� �� �ؾ��� �� ����
	CharacterManager::Get()->Spawn("test1", Character::Team::PLAYER, terrain, 5, 5);
	CharacterManager::Get()->Spawn("test2", Character::Team::ENEMY, terrain, 6, 8);

	mapCursor = new MapCursor;
	mapCursor->SetGridTerrain(terrain);
	mapCursor->SetPosCoord(5, 6, true);
	Observer::Get()->AddParamEvent("CharacterMoveEnd", bind(&SRPGScene::CharacterMoveEnd, this, placeholders::_1));

	Observer::Get()->AddEvent("InputAction", bind(&SRPGScene::InputAction, this));
	Observer::Get()->AddEvent("InputAttack", bind(&SRPGScene::InputAttackAction, this));

	SRPGUIManager::Get()->OpenUI("Test", Vector3(CENTER_X - 300.0f, CENTER_Y));

	TurnManager::Get();
}

SRPGScene::~SRPGScene()
{
	delete terrain;
	delete mapCursor;

	CharacterManager::Delete();
	SRPGUIManager::Delete();

	TurnManager::Delete();
}

void SRPGScene::Update()
{
	terrain->Update();
	CharacterManager::Get()->Update();

	SRPGUIManager::Get()->Update();
	if (!CharacterManager::Get()->IsActing())
		Control();
}

void SRPGScene::PreRender()
{
}

void SRPGScene::Render()
{
	terrain->Render();

	CharacterManager::Get()->Render();
	if (!CharacterManager::Get()->IsActing())
		mapCursor->Render();
}

void SRPGScene::PostRender()
{
	SRPGUIManager::Get()->Render();
}

void SRPGScene::GUIRender()
{
	terrain->GUIRender();
}

void SRPGScene::Control()
{
	if (TurnManager::Get()->GetCurPlayer() == Character::Team::PLAYER) {
		//�÷��̾� ��Ʈ��
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
	}
	else {
		//���ʹ� ��Ʈ��
		//Ȧ��� ĳ���Ͱ� �ִ�
		auto holded = CharacterManager::Get()->HoldedCharacter();
		if (holded->IsActed())
			CharacterManager::Get()->CharacterUnhold();

		if(holded) {
			vector<Character*> attackableEnemies = terrain->AttackableCharacters(Character::Team::PLAYER);
			
			//���� ��Ÿ� ���� �ִ�

			{
				//�ش� ���� Ÿ������ ����
				//CharacterManager::Get()->BattleStart(holded, );
				return;
			}
			//������
			{
				if(holded->IsMoved())
				{
					//�̵� �ߴٸ�
					CharacterManager::Get()->CharacterUnhold();
				}
				else {
					//�̵� �� �ߴٸ�
					//���� ����� ���� ã��
					//�̵�
				}
			}
			return;
		}

		//�ڱ��� ĳ���� �� �� ������ ĳ���� ã��
		Character* character = CharacterManager::Get()->GetActableCharacter(Character::Team::ENEMY);
		
		//�ൿ �� �� ĳ���� ������
		if(character == nullptr)
		{
			//�� ����
			TurnManager::Get()->NextTurn();
			return;
		}
		else {
			//������ �ش� ĳ���� ����
			auto coord = terrain->PosToCoord(character->Pos());
			terrain->SetSelected(coord.first, coord.second, true);
			terrain->InputAction();
		}
	}
}

void SRPGScene::InputAction()
{
	terrain->InputAction(mapCursor->GetW(), mapCursor->GetH());
}

void SRPGScene::InputAttackAction()
{
	terrain->InputAction(mapCursor->GetW(), mapCursor->GetH(), GridedTerrain::ATTACK);
}

void SRPGScene::CharacterMoveEnd(void* characterPtr)
{
	auto character = (Character*)characterPtr;
	if (!character) //wrong call
		return;

	pair<int, int> characterPosCoord = terrain->PosToCoord(character->GlobalPos());
	mapCursor->SetPosCoord(characterPosCoord.first, characterPosCoord.second, true);
	terrain->CheckMovableArea();

	SRPGUIManager::Get()->CloseUI("MapSelectMove");
}
