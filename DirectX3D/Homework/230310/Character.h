#pragma once

class Character : public Transform
{
private:
	friend class CharacterManager;

	enum AnimState {
		IDLE, RUN, ATTACK, HIT, DIE
	};

	struct Status {
		int move = 3;							//�ִ� �̵� �Ÿ�. ���߿� �������ͽ��� ���� 
		pair<int, int> attackRange = { 1, 1 };	//�ּ�, �ִ� ���ݻ�Ÿ�. ���������� ���߿� �������ͽ���
	};

	Character();
	~Character();
public:

	void Update();
	void Render();

	void TurnStart();
	
	void ActEnd() { acted = true; }
	bool IsActed() { return acted; }

	void SetMovePath(vector<Vector3>& path);

	bool IsActing();
	int GetMaxMove() { return moved ? 0 : status.move; }
	pair<int, int> GetAttackRange() { return acted ? make_pair(0, 0) : status.attackRange; }

	void SetDir(Vector3 dir);
private:
	bool IsMoving();
	void Move();

	void SetAnimState(AnimState state);
	void AttackEnd();

	void AttackHit();
	void Damaged(int damage);
private:
	ModelAnimator* body;					//��ġ �׽�Ʈ ��. ���� �� �ִϸ����ͷ� ����
	AnimState animState = IDLE;				//���� �ִϸ��̼�

	bool acted = false;						//�ش� �� �ൿ ���� : ����, Ȥ�� �ൿ �Ϸ� ���� �� true�� ����. �� ���۽� false
	bool moved = false;						//�ش� �� �̵� ����
	Status status;

	float lerpValue = 0.0f;					//��ġ �̵� ���� ������ ���� ���尪. ĭ �������� ���ľ� �ϱ⿡ ������ ���ڿ�������
	float moveSpeed = 0.2f;					//�̵� �ӵ�(������ ��ȭ) 
	Vector3 dir = { 0, 0, -1 };				//���� �ִ� ����
	vector<Vector3> movePath;

	class FloatingDamage* floatingDamage;
};

