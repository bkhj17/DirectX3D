#pragma once

class Character;
class CharacterManager :
    public Singleton< CharacterManager>
{
private:
    friend class Singleton;
    CharacterManager();
    ~CharacterManager();

    UINT MAX_POOL = 30;

    struct Attack {
        int damage;

        Character* attacker;
        Character* hit;
    };

public:
    void Update();
    void Render();

    void CharacterHold(Character* character) { holded = character; }
    void CharacterHold(Character* character, int w, int h) { holded = character; holdedW = w; holdedH = h; }
    Character* HoldedCharacter() { return holded; }
    void CharacterUnhold();

    bool IsActing();

    //����
    //���� ��ǥ ���� : 1. �ȿ��� 2. �ۿ���
    Character* Spawn();
    Character* Spawn(class GridedTerrain* terrain, int w, int h);

    bool IsBattle() { return isBattle; }
    void BattleStart(Character* offense, Character* defense);

    void TurnStart();

private:
    void BattleUpdate();
    void BattleEnd();

    void AttackHit(void* ptr);
    void AttackEnd(void* ptr);
private:
    int holdedW = -1, holdedH = -1;
    Character* holded = nullptr;    
   
    //���� ���� ĳ���͵� : ��Ʋ�� ���� �� ���� ���� �� ���ٸ�
    bool isBattle = false; 
    Character* curOffense = nullptr;
    Character* curDefense = nullptr;
    //���� ����
    queue<Attack> attacks;

    vector<Character*> characterPool;
};
