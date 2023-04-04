#pragma once


class Character : public Transform
{
private:
	friend class CharacterManager;

	enum AnimState {
		IDLE, RUN, ATTACK, HIT, DIE
	};

	Character();
	~Character();
public:
	enum Team {
		PLAYER, ENEMY, NONE
	};

	struct Status {
		string name = "";
		int teamNum = NONE;

		int maxHp = 2;
		int curHp = 2;

		int attack = 3;
		int defence = 3;

		int move = 3;
		pair<int, int> attackRange = { 1, 1 };	//�ּ�, �ִ� ���ݻ�Ÿ�. ���߿� ���� ��������

	};
public:
	void Init();

	void Update();
	void Render();

	void TurnStart();
	
	void ActEnd() { acted = true; originPos = Pos(); }
	bool IsActed() { return acted; }

	void SetMovePath(vector<Vector3>& path);

	bool IsActing();
	int GetMaxMove() { return IsMovable() ? status.move : 0; }
	pair<int, int> GetAttackRange() { return acted ? make_pair(0, 0) : status.attackRange; }

	void SetDir(Vector3 dir);

	bool IsMoved() { return moved; }
	bool IsMovable() { return !acted && !moved; }
	void CancelMove();

	const Status& GetStatus() { return status; }
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

	Weapon* weapon;

	bool acted = false;						//�ش� �� �ൿ ���� : ����, Ȥ�� �ൿ �Ϸ� ���� �� true�� ����. �� ���۽� false
	bool moved = false;						//�ش� �� �̵� ����
	Vector3 originPos = {};					//�ൿ ���� ���� ��ġ

	Status status;

	float lerpValue = 0.0f;					//��ġ �̵� ���� ������ ���� ���尪. ĭ �������� ���ľ� �ϱ⿡ ������ ���ڿ�������
	float moveSpeed = 0.2f;					//�̵� �ӵ�(������ ��ȭ) 
	Vector3 dir = { 0, 0, -1 };				//���� �ִ� ����
	vector<Vector3> movePath;

	IntValueBuffer* valueBuffer;
};

