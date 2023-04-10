#include "framework.h"

void EnemyController::Control(void* pack)
{
	if (SRPGUIManager::Get()->IsActing())
		return;

	auto terrain = (GridedTerrain*)pack;
	if (terrain == nullptr)	//����� assert�� �ʿ��ϴ�
		return;

	//���ʹ� ��Ʈ��
	holded = CharacterManager::Get()->HoldedCharacter();
	//�̹� Ȧ��� ĳ���Ͱ� �ִ�
	if (holded) {
		if (holded->IsActed())
			CharacterManager::Get()->CharacterUnhold();
		//���ݹ��� ���� ���� ã��
		auto vAttackables = terrain->AttackableCharacters(SRPGObject::Team::PLAYER);
		if (!vAttackables.empty()) {
			//���� ����� ���� �� ���� ������ ��ġ
			SRPGObject* target = vAttackables.front().first;
			int tile = vAttackables.front().second.second;
			//���� ��Ÿ� ���� ���� �ִ� == ���� ��ġ�� ���� ������ ��ġ
			CharacterAct(terrain, target, tile);
			return;
		}

		auto vObstacles = terrain->AttackableCharacters(SRPGObject::Team::NONE);
		if (!vObstacles.empty()) {
			//���� ���� ���� ��ֹ��� �ִ�
			SRPGObject* target = vObstacles.front().first;
			int tile = vObstacles.front().second.second;
			//���� ��Ÿ� ���� ���� �ִ� == ���� ��ġ�� ���� ������ ��ġ
			CharacterAct(terrain, target, tile);
			return;
		}
		
		//���� ������ ���� ����
		holded->ActEnd();							//�ൿ ����
		CharacterManager::Get()->CharacterUnhold();	//�� ���� ����
	}
	//Ȧ��� �� ���ٸ�
	//�ڱ��� ĳ���� �� �� ������ ĳ���� ã��
	Character* character = CharacterManager::Get()->GetActableCharacter(Character::Team::ENEMY);
	//�ൿ �� �� ĳ���� ������
	if (character == nullptr)
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

void EnemyController::CharacterAct(GridedTerrain* terrain, SRPGObject* target, int tile)
{
	int holdedIndex = terrain->CoordToIndex(terrain->PosToCoord(holded->GlobalPos()));

	if (tile == holdedIndex)
	{
		//�ش� ���� Ÿ������ ����
		auto coord = terrain->PosToCoord(target->GlobalPos());
		terrain->InputAction(coord.first, coord.second, GridedTerrain::ATTACK);
	}
	else {
		if (holded->IsMoved())
		{
			//�̵� �ߴٸ�
			CharacterManager::Get()->CharacterUnhold();
		}
		else {
			//�̵� �� �ߴٸ� ���� ����� ���� ���� �̵�
			auto coord = terrain->IndexToCoord(tile);
			terrain->InputAction(coord.first, coord.second);
		}
	}
}
