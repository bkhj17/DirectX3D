#pragma once

class Character : public Transform
{
private:
	enum AnimState {
		IDLE, RUN
	};

public:
	Character();
	~Character();

	void Update();
	void Render();

	bool IsActing();

	void SetMovePath(vector<Vector3>& path);

	int GetMaxMove() { return move; }
private:
	bool IsMoving();

	void SetAnimState(AnimState state);

private:
	ModelAnimator* body;	//��ġ �׽�Ʈ ��. ���� �� �ִϸ����ͷ� ����
	AnimState animState = IDLE;

	int move = 3;
	float lerpValue = 0.0f;
	float moveSpeed = 0.2f;
	Vector3 dir = { 0, 0, -1 };
	vector<Vector3> movePath;
};

