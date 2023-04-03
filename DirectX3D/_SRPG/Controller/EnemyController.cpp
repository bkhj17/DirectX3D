#include "framework.h"
#include "EnemyController.h"

void EnemyController::Control(void* pack)
{
	if (SRPGUIManager::Get()->IsActing())
		return;

	auto terrain = (GridedTerrain*)pack;
	if (terrain == nullptr)	//����� assert�� �ʿ��ϴ�
		return;

	//���ʹ� ��Ʈ��
	auto holded = CharacterManager::Get()->HoldedCharacter();
	//�̹� Ȧ��� ĳ���Ͱ� �ִ�
	if (holded) {
		if (holded->IsActed())
			CharacterManager::Get()->CharacterUnhold();
		int holdedIndex = terrain->CoordToIndex(terrain->PosToCoord(holded->GlobalPos()));
		//���ݹ��� ���� ���� ã��
		auto vAttackables = terrain->AttackableCharacters(Character::Team::PLAYER);
		if (vAttackables.empty()) {
			//���� ������ ���� ����
			holded->ActEnd();							//�ൿ ����
			CharacterManager::Get()->CharacterUnhold();	//�� ���� ����
			return;
		}

		//���� ����� ���� �� ���� ������ ��ġ
		Character* target = vAttackables.front().first;
		int tile = vAttackables.front().second.second;
		//���� ��Ÿ� ���� ���� �ִ� == ���� ��ġ�� ���� ������ ��ġ
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
		return;
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
