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
    Character* HoldedCharacter() { return holded; }
    void CharacterUnhold();


    bool IsActing();

    //����
    //���� ��ǥ ���� : 1. �ȿ��� 2. �ۿ���
    Character* Spawn();

    bool IsBattle() { return isBattle; }
    void BattleStart(Character* offense, Character* defense);

private:
    void BattleUpdate();
    void BattleEnd();

    void AttackHit(void* ptr);
    void AttackEnd(void* ptr);
private:
    Character* holded = nullptr;    
   
    //���� ���� ĳ���͵� : ��Ʋ�� ���� �� ���� ���� �� ���ٸ�
    bool isBattle = false; 
    Character* curOffense = nullptr;
    Character* curDefense = nullptr;
    //���� ����
    queue<Attack> attacks;

    vector<Character*> characterPool;
};
