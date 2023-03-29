#include "framework.h"
#include "CharacterManager.h"
#include "Character.h"
#include "../Field/GridedTerrain.h"

CharacterManager::CharacterManager()
{
	characterPool.resize(MAX_POOL);
	for (auto& character : characterPool) {
		character = new Character();
		character->SetActive(false);
	}

	Observer::Get()->AddParamEvent("CharacterAttackHit", bind(&CharacterManager::AttackHit, this, placeholders::_1));
	Observer::Get()->AddParamEvent("CharacterAttackEnd", bind(&CharacterManager::AttackEnd, this, placeholders::_1));
}

CharacterManager::~CharacterManager()
{
	for (auto character : characterPool)
		delete character;
	characterPool.clear();
}

void CharacterManager::Update()
{
	for (auto character : characterPool)
		character->Update();

	if (isBattle)
		BattleUpdate();
}

void CharacterManager::Render()
{
	for (auto character : characterPool)
		character->Render();
}

void CharacterManager::CharacterUnhold()
{
	if (holded->moved)
		holded->ActEnd();
	
	CharacterHold(nullptr);
}

bool CharacterManager::IsActing()
{
	if (isBattle)
		return true;

	for (auto character : characterPool) {
		if (character->IsActing())
			return true;
	}

	return false;
}

Character* CharacterManager::Spawn()
{
	for (auto character : characterPool) {
		if (character->Active())
			continue;

		character->SetActive(true);
		return character;
	}
	return nullptr;
}

Character* CharacterManager::Spawn(GridedTerrain* terrain, int w, int h)
{
	Character* spawned = Spawn();

	spawned->Pos() = terrain->CoordToPos(w, h);
	spawned->UpdateWorld();
	terrain->AddObject(spawned);

	return nullptr;
}

void CharacterManager::BattleStart(Character* offense, Character* defense)
{
	if (!offense || !offense->Active()
		|| !defense || !defense->Active())
		return;
	
	isBattle = true;
	curOffense = offense;
	curDefense = defense;
	//�������ͽ��� ���� ���� ����
	attacks.push({ 1, offense, defense });
	attacks.push({ 1, defense, offense });

	//ī�޶� ����
}

void CharacterManager::TurnStart()
{
	for (auto character : characterPool) {
		if (character->Active())
			character->TurnStart();
	}
}

void CharacterManager::BattleUpdate()
{
	if (attacks.empty() || !curOffense->Active() || !curDefense->Active()) {
		BattleEnd();
		return;
	}

	auto attacker = attacks.front().attacker;
	auto defender = attacks.front().hit;
	if (!attacker->IsActing()) {
		//���� ���� ���� �� �� -> ���۸��
		//���� ����
		Vector3 hitPos = defender->Pos();
		Vector3 attackerPos = attacker->Pos();
		Vector3 dir = defender->Pos() - attacker->Pos();
		dir.y = 0.0f;
		attacker->SetDir(dir);
		//defender->SetDir(-dir); //��� ���ϴ� �����ΰ� ���ڴ� �;� ����

		attacker->SetAnimState(Character::ATTACK);
	}
}

void CharacterManager::BattleEnd()
{
	curOffense->ActEnd();

	isBattle = false;
	curOffense = nullptr;
	curDefense = nullptr;
		
	//ī�޶� ����

	Observer::Get()->ExcuteEvent("BattleEnd");

	if (HoldedCharacter()->IsActed())
		CharacterUnhold();
}

void CharacterManager::AttackHit(void* ptr)
{
	auto attacked = (Character*)ptr;
	if (attacks.empty() || attacks.front().attacker != attacked)
		return;

	attacks.front().hit->Damaged(1);
}

void CharacterManager::AttackEnd(void* ptr)
{
	auto attacked = (Character*)ptr;
	if (!attacks.empty() && attacks.front().attacker == attacked)
		attacks.pop();
}
