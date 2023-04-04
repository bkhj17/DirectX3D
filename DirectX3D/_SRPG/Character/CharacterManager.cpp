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

	Observer::Get()->AddEvent("CharacterUnhold", bind(&CharacterManager::CharacterUnhold, this));
	Observer::Get()->AddParamEvent("CharacterAttackHit", bind(&CharacterManager::AttackHit, this, placeholders::_1));
	Observer::Get()->AddParamEvent("CharacterAttackEnd", bind(&CharacterManager::AttackEnd, this, placeholders::_1));

	Observer::Get()->AddEvent("TurnStart", bind(&CharacterManager::TurnStart, this));

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
	if (holded == nullptr)
		return;

	if (holded->moved)
		holded->ActEnd();
	
	CharacterHold(nullptr, -1, -1);
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

		character->Init();
		return character;
	}
	return nullptr;
}

Character* CharacterManager::Spawn(GridedTerrain* terrain, int w, int h)
{
	assert(terrain != nullptr);

	Character* spawned = Spawn();
	if (spawned == nullptr)
		return nullptr;

	spawned->originPos = spawned->Pos() = terrain->CoordToPos(w, h);
	spawned->UpdateWorld();
	terrain->AddObject(spawned);
	return spawned;
}

Character* CharacterManager::Spawn(string name, int teamNum, GridedTerrain* terrain, int w, int h)
{
	Character* character = Spawn(terrain, w, h);
	if(character == nullptr)
		return nullptr;

	character->status.name = name;
	character->status.teamNum = teamNum;
	return character;
}

void CharacterManager::BattleStart(Character* offense, Character* defense)
{
	if (!offense || !offense->Active()
		|| !defense || !defense->Active())
		return;
	
	isBattle = true;
	curOffense = offense;
	curDefense = defense;
	//�������ͽ��� ���� �̸� ������ ����


	//��Ÿ��� ���� ���� �Ұ����ϰԵ� �ؾ� �Ѵ�
	pair<Vector3, pair<int, int>> pack = {};
	pack.first = offense->Pos();
	Observer::Get()->ExcuteParamEvent("CallPosToCoord", &pack);

	pair<int, int> coordO = pack.second;			//������ ��ġ coord

	pack.first = offense->Pos();
	Observer::Get()->ExcuteParamEvent("CallPosToCoord", &pack);
	pair<int, int> coordD = pack.second;

	int dist = abs(coordO.first - coordD.first) + abs(coordO.second - coordD.second);

	//����ڰ� ������ ��Ÿ� �� => ���ʿ� ��Ÿ� ���̶� ���� ��ü�� ���� �� ��
	if (offense->GetAttackRange().first > dist || offense->GetAttackRange().second < dist)
		return;
	attacks.push({ CalcDamage(offense, defense), offense, defense });		//������ ����
	
	//�����ڰ� ����� ��Ÿ� �ȿ� ���� ������ �ݰ��� ����
	if (offense->GetAttackRange().first <= dist && offense->GetAttackRange().second >= dist)
		attacks.push({ CalcDamage(defense, offense),defense, offense });	//����� �ݰ�

	//ī�޶� ����
}

void CharacterManager::TurnStart()
{
	for (auto character : characterPool) {
		if (character->Active())
			character->TurnStart();
	}
}

void CharacterManager::CancelMove()
{
	if (holded == nullptr)
		return;

	holded->CancelMove();
}

Character* CharacterManager::GetActableCharacter(Character::Team team)
{
	for (auto character : characterPool) {
		if (!character->Active() || character->IsActed())
			continue;

		if (character->GetStatus().teamNum == team)
			return character;
	}
	return nullptr;
}

int CharacterManager::NumActiveCharactersByTeam(Character::Team team)
{
	int cnt = 0;
	for (auto character : characterPool) {
		if (!character->Active())
			continue;

		if (character->GetStatus().teamNum == team)
			cnt++;
	}
	return cnt;
}

void CharacterManager::BattleUpdate()
{
	if (attacks.empty() || !curOffense->Active() || !curDefense->Active()) {
		BattleEnd();
		return;
	}

	auto attacker = attacks.front().attacker;
	auto defender = attacks.front().hit;
	if (!attacker->IsActing() && !defender->IsActing()) {
		//���� ���� ���� �� �� -> ���۸��
		//���� ����
		Vector3 hitPos = defender->Pos();
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

	while(!attacks.empty())
		attacks.pop();

	//ī�޶� ����
	Observer::Get()->ExcuteEvent("BattleEnd");
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

int CharacterManager::CalcDamage(Character* attacker, Character* defender)
{
	//���� = �� + ���� ���� * (��ȿ ? 2 : 1) 
	 
	//���ط� = (���� - ����)*(1 or 3(ũ��Ƽ��) )

	//�ӽ÷� 1
	return 1;
}
