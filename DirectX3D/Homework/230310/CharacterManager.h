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

    void BattleStart(Character* offense, Character* defense);

private:
    void BattleEnd();

private:
    Character* holded = nullptr;    
   
    //���� ���� ĳ���͵� : ��Ʋ�� ���� �� ���� ���� �� ���ٸ�
    Character* curOffense = nullptr;
    Character* curDefense = nullptr;

    vector<Character*> characterPool;
};
